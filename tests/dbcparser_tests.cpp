#include <gtest/gtest.h>
#include <iterator>

#include "dbc_parser_data.hpp"
#include "dbcparser.h"
#include "log.hpp"

using strings = std::vector<std::string>;
std::shared_ptr<spdlog::logger> kDefaultLogger
    = []() -> std::shared_ptr<spdlog::logger> {
    auto z = std::getenv("CDB_LEVEL");
    auto logger = spdlog::stdout_color_mt("cdb");

    if (z == nullptr) {
        logger->set_level(spdlog::level::err);
    } else {
        const std::string ll{ z };

        auto it = std::find_if(std::begin(spdlog::level::level_names),
            std::end(spdlog::level::level_names),
            [&ll](const char* name) { return std::string{ name } == ll; });

        if (it != std::end(spdlog::level::level_names)) {
            int i = std::distance(std::begin(spdlog::level::level_names), it);
            logger->set_level(static_cast<spdlog::level::level_enum>(i));
        }
    }

    return logger;
}();

struct DBCParserTests : public ::testing::Test {
    CANdb::DBCParser parser;
};

struct SymbolsTest : public ::testing::TestWithParam<strings> {
    CANdb::DBCParser parser;
};

struct EcusTest : public ::testing::TestWithParam<strings> {
    CANdb::DBCParser parser;
};

struct ValuesTableTest : public ::testing::TestWithParam<strings> {
    CANdb::DBCParser parser;
};

struct ValuesTest : public ::testing::TestWithParam<strings> {
    CANdb::DBCParser parser;
};

struct MessageTests : public ::testing::Test {
    CANdb::DBCParser parser;
};

TEST_F(DBCParserTests, empty_data)
{
    EXPECT_FALSE(parser.parse(""));
    EXPECT_TRUE(parser.getDb().messages.empty());
}

TEST_F(DBCParserTests, comments_only)
{
    auto data
        = R"(// *************************************************************************** *
// *                                                                           *
// *                      Some Corporation Limited                             *
// *                                                                           *
// *************************************************************************** *
// *                                                                           *
// * Description:         This is a file for BORAT                             *
// * Node                 KORAB                                                *
// *                                                                           *
// *************************************************************************** *
// * Author: Geralt                                                            *
// *************************************************************************** *


VERSION "KORAB"
// comment

)";
    EXPECT_TRUE(parser.parse(data));
    EXPECT_TRUE(parser.getDb().messages.empty());
    EXPECT_EQ(parser.getDb().version, "KORAB");
}

TEST_F(DBCParserTests, one_liner)
{
    EXPECT_TRUE(parser.parse("VERSION \"\"\n"));
    EXPECT_TRUE(parser.getDb().messages.empty());
    EXPECT_EQ(parser.getDb().version, "");
}

TEST_F(DBCParserTests, correct_version_number)
{
    EXPECT_TRUE(parser.parse("VERSION \"123 aa\" \n\n\n\n"));
    EXPECT_TRUE(parser.getDb().messages.empty());
    EXPECT_EQ(parser.getDb().version, "123 aa");
}

TEST_P(SymbolsTest, one_symbol)
{
    auto params = GetParam();
    std::string dbc =
        R"(VERSION ""
NS_ :
)";

    for (const auto& param : params) {
        dbc += "  ";
        dbc += param;
        dbc += "\n";
    }
    dbc += "\n";

    ASSERT_TRUE(parser.parse(dbc));
    EXPECT_EQ(parser.getDb().symbols, params);
}

TEST_P(EcusTest, single_line)
{
    auto params = GetParam();
    std::string dbc =
        R"(VERSION ""
NS_ :
  NS_DESC
  NS_DESC2

BU_ :)";
    for (const auto& ecu : params) {
        dbc += "  " + ecu;
    }
    dbc += "\n";
    dbc += "\n";
    ASSERT_TRUE(parser.parse(dbc));
    EXPECT_EQ(parser.getDb().ecus, params);
}

TEST_P(EcusTest, multi_line)
{
    auto params = GetParam();
    std::string dbc =
        R"(VERSION ""
NS_ :
  NS_DESC
  NS_DESC2

BS_:

BU_ :)";
    for (const auto& ecu : params) {
        dbc += std::string{ "\n" } + std::string{ "  " } + ecu;
    }
    dbc += "\n";
    dbc += "\n";
    ASSERT_TRUE(parser.parse(dbc));
    EXPECT_EQ(parser.getDb().ecus, params);
}

TEST_P(ValuesTableTest, val_table)
{
    auto values = GetParam();
    std::string dbc =
        R"(VERSION ""
NS_ :
  NS_DESC
  NS_DESC2

BU_ :
  NEO
  MCU
  GTW

)";
    for (const auto& value : values) {
        dbc += std::string{ "VAL_TABLE_ " } + value;
        dbc += "\n";
    }
    dbc += "\n";
    ASSERT_TRUE(parser.parse(dbc));

    EXPECT_EQ(parser.getDb().val_tables.size(), values.size());
}

TEST_F(MessageTests, messages)
{
    std::string dbc =
        R"(VERSION ""

NS_ :
  NS_DESC
  NS_DESC2

BU_ :
  NEO
  MCU
  GTW

)";
    // std::vector<std::string> values { test_data::bo1, test_data::bo2 };
    std::vector<std::string> values{ test_data::bo1, test_data::bo2 };
    for (const auto& value : values) {
        dbc += value;
        dbc += "\n";
        dbc += "\n";
    }
    ASSERT_TRUE(parser.parse(dbc));

    EXPECT_EQ(parser.getDb().messages.size(), values.size());
    EXPECT_EQ(parser.getDb().messages.at(CANmessage{ 1160 }).size(), 6u);

    // bo1
    std::vector<CANsignal> expectedSignals;
    expectedSignals.push_back(CANsignal{ "DAS_steeringControlType", 23, 2, 0,
        "+", 1, 0, 0, 0, "", { { "EPAS" } } });
    expectedSignals.push_back(CANsignal{ "DAS_steeringControlChecksum", 31, 8,
        0, "+", 1, 0, 0, 0, "", { { "EPAS" } } });
    expectedSignals.push_back(CANsignal{
        "DAS_steeringControlType", 19, 4, 0, "+", 1, 0, 0, 0, "", { "EPAS" } });
    expectedSignals.push_back(CANsignal{
        "DAS_steeringControlType", 7, 2, 0, "+", 1, 0, 0, 0, "", { "EPAS" } });
    CANmessage msg{ 1160, "DAS_steeringControl", 4, "NEO" };
    auto expSig = CANsignal{ "DAS_steeringControlType", 23, 2, 0, "+", 1, 0, 0,
        0, "", { "EPAS" } };

    ASSERT_EQ(parser.getDb().messages.size(), values.size());
    ASSERT_EQ(parser.getDb().messages.at(msg).size(), 6u);
    EXPECT_EQ(parser.getDb().messages.at(msg).at(0), expSig);

    expSig = CANsignal{ "DAS_steeringControlChecksum", 31, 8, 0, "+", 1, 0, 0,
        0, "", { "EPAS" } };
    EXPECT_EQ(parser.getDb().messages.at(msg).at(1), expSig);

    expSig = CANsignal{ "DAS_steeringControlCounter", 19, 4, 0, "+", 1, 0, 0, 0,
        "", { "EPAS" } };
    EXPECT_EQ(parser.getDb().messages.at(msg).at(2), expSig);

    expSig = CANsignal{ "DAS_steeringHapticRequest", 7, 1, 0, "+", 1, 0, 0, 0,
        "", { "EPAS" } };
    EXPECT_EQ(parser.getDb().messages.at(msg).at(3), expSig);

    msg = CANmessage{ 257, "GTW_epasControl", 3, "NEO" };
    ASSERT_EQ(parser.getDb().messages.at(msg).size(), 7u);

    expSig = CANsignal{ "GTW_epasEmergencyOn", 0, 1, 0, "+", 1, 0, 2, -1, "",
        { "EPAS" } };
    EXPECT_EQ(parser.getDb().messages.at(msg).at(3), expSig);
}

TEST_P(ValuesTest, vals)
{
    auto values = GetParam();
    std::string dbc =
        R"(VERSION ""
NS_ :
  NS_DESC
  NS_DESC2

BU_ :
  NEO
  MCU
  GTW

)";
    for (const auto& value : values) {
        dbc += value;
        dbc += "\n";
    }
    ASSERT_TRUE(parser.parse(dbc));
}

// test case instantiations

INSTANTIATE_TEST_CASE_P(Ecus, EcusTest,
    ::testing::Values(strings{ "NEO" }, strings{ "NEO", "MCU" }));

INSTANTIATE_TEST_CASE_P(Symbols, SymbolsTest,
    ::testing::Values(
        strings{}, strings{ "NS_DESC" }, strings{ "NS_DESC2", "NS_DESC" }));
INSTANTIATE_TEST_CASE_P(Value, ValuesTest,
    ::testing::Values(
        strings{ "VAL_ 880 EPAS_steeringReduced 0 \"NORMAL_ASSIST\" "
                 "1 \"REDUCED_ASSIST\" ;" }));

INSTANTIATE_TEST_CASE_P(Values, ValuesTableTest,
    ::testing::Values(strings{ "StW_AnglHP_Spd 16383 \"SNA\" ;",
        "DI_aebFaultReason 15 "
        "\"DI_AEB_FAULT_DAS_REQ_DI_UNAVAIL\" 14 "
        "\"DI_AEB_FAULT_ACCEL_REQ_INVALID\" ;" }));
