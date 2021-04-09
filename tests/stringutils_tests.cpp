#include "stringutils.h"
#include <gtest/gtest.h>
#include <vector>

TEST(StringUtils, replace_all)
{
    EXPECT_EQ(StringUtils::replace_all("aabbcc", "a", "x"), "xxbbcc");
    EXPECT_EQ(StringUtils::replace_all("aabbcc", "a", ""), "bbcc");
    EXPECT_EQ(StringUtils::replace_all("test123", "test", ""), "123");
    EXPECT_EQ(StringUtils::replace_all("abababba", "ab", "xXx"), "xXxxXxxXxba");
}

TEST(StringUtils, erase_all)
{
    EXPECT_EQ(StringUtils::erase_all("aabbcc", "a"), "bbcc");
    EXPECT_EQ(StringUtils::erase_all("", "1"), "");
    EXPECT_EQ(StringUtils::erase_all("abcdefg", "abc"), "defg");
}

TEST(StringUtils, split)
{
    EXPECT_EQ(StringUtils::split("aa bb cc", " "), (std::vector<std::string>{ "aa", "bb", "cc" }));
    EXPECT_EQ(StringUtils::split("abc", " "), (std::vector<std::string>{ "abc" }));
    EXPECT_EQ(StringUtils::split("a bcd", " "), (std::vector<std::string>{ "a", "bcd" }));
}
