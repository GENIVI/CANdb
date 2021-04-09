#include <fstream>
#include <gtest/gtest.h>

#include "dbcparser.h"
#include "log.hpp"
#include "opendbc_tests_expected_data.hpp"
#include "test_helper.hpp"

#include <filesystem>

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

TEST(OpenDBCTest, all_tests)
{
    GTEST_SKIP();
    const auto path = fs::path{ fs::path{ DBC_DIR } / fs::path{ "opendbc" } };

    for (const auto& p : std::filesystem::directory_iterator(path)) {
        CANdb::DBCParser parser;
        if (p.is_regular_file()) {
            const auto filepath = "opendbc/" + p.path().filename().string();
            auto file = test_helper::loadDBCFile(filepath);
            const auto db = parser.parse(file);
            if (db) {
                std::cout << "Success " << filepath << "\n";
            } else {
                std::cout << "Failure " << filepath << "\n";
            }
        }
    }
}

INSTANTIATE_TEST_SUITE_P(OpenDBC, OpenDBCTest,
    ::testing::Values("acura_ilx_2016_nidec.dbc", "gm_global_a_chassis.dbc", "gm_global_a_lowspeed.dbc",
        "gm_global_a_powertrain.dbc", "honda_accord_touring_2016_can.dbc", "hyundai_kia_generic.dbc",
        "cadillac_ct6_powertrain.dbc", "volvo_v40_2017_pt.dbc", "hyundai_2015_ccan.dbc", "gm_global_a_powertrain.dbc",
        "nissan_leaf_2018.dbc", "toyota_tss2_adas.dbc", "hyundai_2015_mcan.dbc", "ford_fusion_2018_adas.dbc",
        "cadillac_ct6_chassis.dbc", "ESR.dbc", "chrysler_pacifica_2017_hybrid.dbc", "acura_ilx_2016_nidec.dbc",
        "gm_global_a_lowspeed.dbc", "toyota_prius_2010_pt.dbc", "gm_global_a_chassis.dbc",
        "mercedes_benz_e350_2010.dbc", "chrysler_pacifica_2017_hybrid_private_fusion.dbc",
        "ford_cgea1_2_bodycan_2011.dbc", "toyota_adas.dbc", "luxgen_s5_2015.dbc", "mazda_2017.dbc",
        "toyota_2017_ref_pt.dbc", "vw_golf_mk4.dbc", "nissan_x_trail_2017.dbc", "volvo_v60_2015_pt.dbc",
        "honda_accord_touring_2016_can.dbc", "gm_global_a_lowspeed_1818125.dbc", "mazda_3_2019.dbc", "bmw_e9x_e8x.dbc",
        "cadillac_ct6_object.dbc"));
