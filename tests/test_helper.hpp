#ifndef TEST_HELPER_HPP_LRQG1DTL
#define TEST_HELPER_HPP_LRQG1DTL

#include <string>

#include <filesystem>
namespace fs = std::filesystem;

namespace test_helper {
std::string loadDBCFile(const fs::path& dbcFileName);
}

#endif /* end of include guard: TEST_HELPER_HPP_LRQG1DTL */
