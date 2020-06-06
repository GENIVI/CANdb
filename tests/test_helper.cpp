#include "test_helper.hpp"
#include "spdlog/fmt/bundled/format.h"

#include <fstream>
#include <stdexcept>

#include <spdlog/fmt/fmt.h>

namespace test_helper {
std::string loadDBCFile(const std::string& dbcFileName)
{
    const std::string path = std::string{ DBC_DIR } + dbcFileName;

    std::fstream ff{ path.c_str() };

    std::string buff;
    std::copy(std::istreambuf_iterator<char>(ff),
        std::istreambuf_iterator<char>(), std::back_inserter(buff));

    ff.close();
    if (buff.empty())
        throw std::runtime_error(
            fmt::format("File {} does not exists", path));
    return buff;
}
} // namespace test_helper
