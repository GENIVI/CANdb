#include <gtest/gtest.h>
#include <iterator>

#include "dbc_parser_data.hpp"
#include "dbcparser.h"
#include "log.hpp"

using strings = std::vector<std::string>;

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
    // EXPECT_TRUE(parser.getDb().messages.empty());
}

TEST_F(DBCParserTests, comments_only)
{
    auto data = R"(// *************************************************************************** *
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
    const auto candb = parser.parse(data);
    ASSERT_TRUE(candb);
    EXPECT_TRUE(candb->messages.empty());
    EXPECT_EQ(candb->version, "KORAB");
}

TEST_F(DBCParserTests, one_liner)
{
    const auto db = parser.parse("VERSION \"\"\n");
    ASSERT_TRUE(db);
    EXPECT_TRUE(db->messages.empty());
    EXPECT_EQ(db->version, "");
}

TEST_F(DBCParserTests, correct_version_number)
{
    const auto db = parser.parse("VERSION \"123 aa\" \n\n\n\n");
    ASSERT_TRUE(db);

    EXPECT_TRUE(db->messages.empty());
    EXPECT_EQ(db->version, "123 aa");
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

    const auto db = parser.parse(dbc);
    ASSERT_TRUE(db);
    EXPECT_EQ(db->symbols, params);
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
    const auto db = parser.parse(dbc);
    ASSERT_TRUE(db);
    EXPECT_EQ(db->ecus, params);
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
    const auto db = parser.parse(dbc);
    ASSERT_TRUE(db);
    EXPECT_EQ(db->ecus, params);
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
    const auto db = parser.parse(dbc);
    ASSERT_TRUE(db);

    EXPECT_EQ(db->val_tables.at(0).identifier, "StW_AnglHP_Spd");
    EXPECT_EQ(db->val_tables.size(), values.size());
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
    std::vector<std::string> values{ test_data::bo1, test_data::bo2, test_data::bo_with_window_endline,
        test_data::bo_signed_sigs, test_data::bo_utf };
    for (const auto& value : values) {
        dbc += value;
        dbc += "\n";
        dbc += "\n";
    }
    ASSERT_TRUE(parser.parse(dbc));
    const auto db = parser.parse(dbc);
    ASSERT_TRUE(db);

    EXPECT_EQ(db->messages.size(), values.size());
    EXPECT_EQ(db->messages.at(CANmessage{ 1160 }).size(), 7u);

    // bo1
    std::vector<CANsignal> expectedSignals;
    expectedSignals.push_back(CANsignal{
        "DAS_steeringControlType", 23, 2, CANsignal::Motorola, CANsignal::Unsigned, 1, 0, 0, 0, "", { { "EPAS" } } });
    expectedSignals.push_back(CANsignal{ "DAS_steeringControlChecksum", 31, 8, CANsignal::Motorola, CANsignal::Unsigned,
        1, 0, 0, 0, "", { { "EPAS" } } });
    expectedSignals.push_back(CANsignal{
        "DAS_steeringControlType", 19, 4, CANsignal::Motorola, CANsignal::Unsigned, 1, 0, 0, 0, "", { "EPAS" } });
    expectedSignals.push_back(CANsignal{
        "DAS_steeringControlType", 7, 2, CANsignal::Motorola, CANsignal::Unsigned, 1, 0, 0, 0, "", { "EPAS" } });

    CANmessage msg{ 1160, "DAS_steeringControl", 4, "NEO" };
    auto expSig = CANsignal{ "DAS_steeringControlType", 23, 2, CANsignal::Motorola, CANsignal::Unsigned, 1, 0, 0, 0, "",
        { "EPAS" } };

    ASSERT_EQ(db->messages.size(), values.size());
    ASSERT_EQ(db->messages.at(msg).size(), 7u);
    EXPECT_EQ(db->messages.at(msg).at(0), expSig);

    expSig = CANsignal{ "DAS_steeringControlChecksum", 31, 8, CANsignal::Motorola, CANsignal::Unsigned, 1, 0, 0, 0, "",
        { "EPAS" } };
    EXPECT_EQ(db->messages.at(msg).at(1), expSig);

    expSig = CANsignal{ "DAS_steeringControlCounter", 14, 7, CANsignal::Motorola, CANsignal::Unsigned, 15.0, 0, 0,
        1425.0, "", { "EPAS" } };
    EXPECT_EQ(db->messages.at(msg).at(2), expSig);

    expSig = CANsignal{ "DAS_steeringHapticRequest", 7, 1, CANsignal::Motorola, CANsignal::Unsigned, 1, 0, 0, 0, "",
        { "EPAS" } };
    EXPECT_EQ(db->messages.at(msg).at(3), expSig);

    msg = CANmessage{ 257, "GTW_epasControl", 3, "NEO" };
    ASSERT_EQ(db->messages.at(msg).size(), 7u);

    expSig
        = CANsignal{ "GTW_epasEmergencyOn", 0, 1, CANsignal::Intel, CANsignal::Unsigned, 1, 0, 2, -1, "", { "EPAS" } };
    EXPECT_EQ(db->messages.at(msg).at(3), expSig);

    // testing for correct signal signage handling

    CANmessage gyro{ 1091, "FKD_Gyro_04", 6, "NEO" };

    expSig
        = CANsignal{ "FKD_GyroRoll", 32, 9, CANsignal::Intel, CANsignal::Signed, 0.1, 0, -25.6, 25.5, "", { "NEO" } };
    EXPECT_EQ(db->messages.at(gyro).at(0), expSig);

    expSig
        = CANsignal{ "FKD_GyroPitch", 16, 9, CANsignal::Intel, CANsignal::Signed, 0.1, 0, -25.6, 25.5, "", { "NEO" } };
    EXPECT_EQ(db->messages.at(gyro).at(1), expSig);

    expSig = CANsignal{ "FKD_GyroHead", 0, 12, CANsignal::Intel, CANsignal::Unsigned, 0.1, 0, 0, 409.5, "", { "NEO" } };
    EXPECT_EQ(db->messages.at(gyro).at(2), expSig);
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

INSTANTIATE_TEST_SUITE_P(Ecus, EcusTest, ::testing::Values(strings{ "NEO" }, strings{ "NEO", "MCU" }));

INSTANTIATE_TEST_SUITE_P(
    Symbols, SymbolsTest, ::testing::Values(strings{}, strings{ "NS_DESC" }, strings{ "NS_DESC2", "NS_DESC" }));
INSTANTIATE_TEST_SUITE_P(Value, ValuesTest,
    ::testing::Values(strings{ "VAL_ 880 EPAS_steeringReduced 0 \"NORMAL_ASSIST\" "
                               "1 \"REDUCED_ASSIST\" ;" }));

INSTANTIATE_TEST_SUITE_P(Values, ValuesTableTest,
    ::testing::Values(strings{ "StW_AnglHP_Spd 16383 \"SNA\" ;",
        "DI_aebFaultReason 15 "
        "\"DI_AEB_FAULT_DAS_REQ_DI_UNAVAIL\" 14 "
        "\"DI_AEB_FAULT_ACCEL_REQ_INVALID\" ;" }));
