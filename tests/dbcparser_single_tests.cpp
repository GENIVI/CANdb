#include <gtest/gtest.h>
#include <iterator>

#include "dbc_parser_data.hpp"
#include "dbcparser.h"
#include "log.hpp"

TEST(ValuesTableTest, val_table_single)
{
    std::string dbc =
        R"(VERSION ""

NS_ :
	NS_DESC_
	CM_
	BA_DEF_
	BA_
	VAL_
	CAT_DEF_
	CAT_
	FILTER
	BA_DEF_DEF_
	EV_DATA_
	ENVVAR_DATA_
	SGTYPE_
	SGTYPE_VAL_
	BA_DEF_SGTYPE_
	BA_SGTYPE_
	SIG_TYPE_REF_
	VAL_TABLE_
	SIG_GROUP_
	SIG_VALTYPE_
	SIGTYPE_VALTYPE_
	BO_TX_BU_
	BA_DEF_REL_
	BA_REL_
	BA_DEF_DEF_REL_
	BU_SG_REL_
	BU_EV_REL_
	BU_BO_REL_
	SG_MUL_VAL_

BS_:

VAL_TABLE_ ALARMS 3 "HIGH" 2 "MEDIUM" 1 "LOW" 0 "OFF" ;
VAL_TABLE_ BOOLEAN 1 "TRUE" 0 "FALSE" ;

)";
    CANdb::DBCParser parser;
    const auto db = parser.parse(dbc);
    ASSERT_TRUE(db);

    ASSERT_EQ(db->val_tables.size(), 2);
    EXPECT_EQ(db->val_tables.at(0).identifier, "ALARMS");

    const std::vector<CANdb_t::ValTable::ValTableEntry> expected{ CANdb_t::ValTable::ValTableEntry{ 3, "HIGH" },
        CANdb_t::ValTable::ValTableEntry{ 2, "MEDIUM" }, CANdb_t::ValTable::ValTableEntry{ 1, "LOW" },
        CANdb_t::ValTable::ValTableEntry{ 0, "OFF" } };
    EXPECT_EQ(db->val_tables.at(0).entries, expected);
    EXPECT_EQ(db->val_tables.at(1).identifier, "BOOLEAN");
}
