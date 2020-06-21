#include "test_helper.hpp"
#include "spdlog/fmt/bundled/format.h"

#include <fstream>
#include <stdexcept>

#include <spdlog/fmt/fmt.h>
#include <spdlog/spdlog.h>

std::shared_ptr<spdlog::logger> kDefaultLogger = []() -> std::shared_ptr<spdlog::logger> {
    auto z = std::getenv("CDB_LEVEL");
    auto logger = spdlog::stdout_color_mt("cdb");

    if (z == nullptr) {
        logger->set_level(spdlog::level::err);
    } else {
        const std::string ll{ z };

        auto it = std::find_if(std::begin(spdlog::level::level_names), std::end(spdlog::level::level_names),
            [&ll](const char* name) { return std::string{ name } == ll; });

        if (it != std::end(spdlog::level::level_names)) {
            int i = std::distance(std::begin(spdlog::level::level_names), it);
            logger->set_level(static_cast<spdlog::level::level_enum>(i));
        }
    }

    return logger;
}();

namespace test_helper {
std::string loadDBCFile(const fs::path& dbcFileName)
{
    const auto path = fs::path{ DBC_DIR } / dbcFileName;

    std::fstream ff{ path.c_str() };

    std::string buff;
    std::copy(std::istreambuf_iterator<char>(ff), std::istreambuf_iterator<char>(), std::back_inserter(buff));

    ff.close();
    if (buff.empty())
        throw std::runtime_error(fmt::format("File {} does not exists", path.string()));
    return buff;
}

std::vector<fs::path> readAllDbcFiles(const fs::path& dbc_dir)
{
    std::vector<fs::path> allFiles;

    fs::directory_iterator it{ fs::path{ DBC_DIR } / dbc_dir };

    std::copy_if(it, fs::directory_iterator{}, std::back_inserter(allFiles), [](const fs::path& path) -> bool {
        return fs::is_regular_file(path);
    });

    return allFiles;
}
} // namespace test_helper
