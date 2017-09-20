#ifndef OPENDBC_TESTS_EXPECTED_DATA_HPP_MASKKZNF
#define OPENDBC_TESTS_EXPECTED_DATA_HPP_MASKKZNF

#include <map>
#include <string>
#include <vector>

using strings = std::vector<std::string>;
namespace test_data {
struct Data {
    strings symbols;
    strings bs;
    strings bu;
};
namespace tesla_dbc {
    const strings symbols
        = { "NS_DESC_", "CM_", "BA_DEF_", "BA_", "VAL_", "CAT_DEF_", "CAT_",
              "FILTER", "BA_DEF_DEF_", "EV_DATA_", "ENVVAR_DATA_", "SGTYPE_",
              "SGTYPE_VAL_", "BA_DEF_SGTYPE_", "BA_SGTYPE_", "SIG_TYPE_REF_",
              "VAL_TABLE_", "SIG_GROUP_", "SIG_VALTYPE_", "SIGTYPE_VALTYPE_",
              "BO_TX_BU_", "BA_DEF_REL_", "BA_REL_", "BA_DEF_DEF_REL_",
              "BU_SG_REL_", "BU_EV_REL_", "BU_BO_REL_", "SG_MUL_VAL_" };
    const strings bs{};
    const strings bu{ "NEO", "MCU", "GTW", "EPAS", "DI", "ESP", "SBW", "STW" };
}; // namespace tesla_dbc

const std::map<std::string, Data> dbc_data{ std::make_pair("tesla_can.dbc",
    Data{ tesla_dbc::symbols, tesla_dbc::bs, tesla_dbc::bu }) };

}; // namespace test_data

#endif /* end of include guard: OPENDBC_TESTS_EXPECTED_DATA_HPP_MASKKZNF */
