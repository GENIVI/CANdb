#include "test_helper.hpp"
#include "spdlog/fmt/bundled/format.h"

#include <filesystem>
#include <fstream>
#include <stdexcept>

#include <spdlog/fmt/fmt.h>

namespace test_helper {
std::string loadDBCFile(const std::filesystem::path& dbcFileName)
{
    const auto path = std::filesystem::path{ DBC_DIR } / dbcFileName;

    std::fstream ff{ path.c_str() };

    std::string buff;
    std::copy(std::istreambuf_iterator<char>(ff),
        std::istreambuf_iterator<char>(), std::back_inserter(buff));

    ff.close();
    if (buff.empty())
        throw std::runtime_error(
            fmt::format("File {} does not exists", path.string()));
    return buff;
}
} // namespace test_helper
