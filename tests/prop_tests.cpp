#include <fstream>
#include <gtest/gtest.h>

#include "dbcparser.h"
#include "log.hpp"
#include "opendbc_tests_expected_data.hpp"
#include "test_helper.hpp"

struct PropDBCTest : public ::testing::Test {
    CANdb::DBCParser parser;
};

TEST_F(PropDBCTest, extended_ids)
{
    const auto file = test_helper::loadDBCFile("prop/extended-ids.dbc");
    ASSERT_TRUE(parser.parse(file));
}

TEST_F(PropDBCTest, vector_file)
{
    const auto file = test_helper::loadDBCFile("prop/comfort.dbc");
    ASSERT_TRUE(parser.parse(file));
}
