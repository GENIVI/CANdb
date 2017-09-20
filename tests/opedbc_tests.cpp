#include <gtest/gtest.h>

#include <fstream>

#include "dbcparser.h"
#include "log.hpp"
#include "opendbc_tests_expected_data.hpp"

extern const char _resource_tesla_can_dbc[];
extern const size_t _resource_tesla_can_dbc_len;
using strings = std::vector<std::string>;

std::string loadDBCFile(const std::string& filename)
{
    const std::string path = std::string{ OPENDBC_DIR } + filename;

    std::fstream file{ path.c_str() };

    std::string buff;
    std::copy(std::istreambuf_iterator<char>(file),
        std::istreambuf_iterator<char>(), std::back_inserter(buff));

    file.close();
    return buff;
}

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

struct OpenDBCTest : public ::testing::TestWithParam<std::string> {
    CANdb::DBCParser parser;
};

TEST_P(OpenDBCTest, parse_dbc_file)
{
    auto dbc_file = GetParam();
    auto file = loadDBCFile(dbc_file);
    ASSERT_TRUE(parser.parse(file));

    if (dbc_file == "tesla_can.dbc") {
        EXPECT_EQ(
            parser.getDb().symbols, test_data::dbc_data.at(dbc_file).symbols);
        EXPECT_EQ(parser.getDb().ecus, test_data::dbc_data.at(dbc_file).bu);
    }
}

INSTANTIATE_TEST_CASE_P(TeslaDBC, OpenDBCTest,
    ::testing::Values("tesla_can.dbc", "acura_ilx_2016_can.dbc",
        "acura_ilx_2016_can.dbc", "acura_ilx_2016_nidec.dbc",
        "gm_global_a_chassis.dbc", "gm_global_a_lowspeed.dbc",
        "gm_global_a_object.dbc", "gm_global_a_powertrain.dbc",
        "honda_accord_touring_2016_can.dbc", "honda_civic_touring_2016_can.dbc",
        "honda_crv_ex_2017_can.dbc", "honda_crv_touring_2016_can.dbc",
        "subaru_outback_2016_eyesight.dbc", "tesla_can.dbc",
        "toyota_prius_2017_can0.dbc", "toyota_prius_2017_can1.dbc"));
