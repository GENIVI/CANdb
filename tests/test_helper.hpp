#ifndef TEST_HELPER_HPP_LRQG1DTL
#define TEST_HELPER_HPP_LRQG1DTL

#include <string>

#ifdef HAS_STD_FILESYSTEM
#include <filesystem>
namespace fs = std::filesystem;
#else
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#endif

namespace test_helper {
std::string loadDBCFile(const fs::path& dbcFileName);
}

#endif /* end of include guard: TEST_HELPER_HPP_LRQG1DTL */
