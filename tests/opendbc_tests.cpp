#include <fstream>
#include <gtest/gtest.h>

#include "dbcparser.h"
#include "log.hpp"
#include "opendbc_tests_expected_data.hpp"
#include "test_helper.hpp"

struct OpenDBCTest : public ::testing::TestWithParam<std::string> {
    CANdb::DBCParser parser;
};

TEST_P(OpenDBCTest, parse_dbc_file)
{
    auto dbc_file = GetParam();
    auto file = test_helper::loadDBCFile("opendbc/" + dbc_file);
    const auto db = parser.parse(file);
    ASSERT_TRUE(db);

    if (dbc_file == "tesla_can.dbc") {
        EXPECT_EQ(db->symbols, test_data::dbc_data.at(dbc_file).symbols);
        EXPECT_EQ(db->ecus, test_data::dbc_data.at(dbc_file).bu);
    }
}

INSTANTIATE_TEST_SUITE_P(OpenDBC, OpenDBCTest,
    ::testing::Values("tesla_can.dbc", "acura_ilx_2016_can.dbc", "acura_ilx_2016_can.dbc", "acura_ilx_2016_nidec.dbc",
        "gm_global_a_chassis.dbc", "gm_global_a_lowspeed.dbc", "gm_global_a_object.dbc", "gm_global_a_powertrain.dbc",
        "honda_accord_touring_2016_can.dbc", "honda_civic_touring_2016_can.dbc", "honda_crv_ex_2017_can.dbc",
        "honda_crv_touring_2016_can.dbc", "subaru_outback_2016_eyesight.dbc", "tesla_can.dbc",
        "toyota_prius_2017_can0.dbc", "toyota_prius_2017_can1.dbc"));
