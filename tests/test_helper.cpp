#include "test_helper.hpp"
#include "spdlog/common.h"

#include <fstream>
#include <stdexcept>

#include <spdlog/fmt/fmt.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

std::shared_ptr<spdlog::logger> kDefaultLogger = []() -> std::shared_ptr<spdlog::logger> {
    auto z = std::getenv("CDB_LEVEL");
    auto logger = spdlog::stdout_color_mt("cdb");

    if (z == nullptr) {
        logger->set_level(spdlog::level::err);
    } else {
        const std::string ll{ z };
        logger->set_level(spdlog::level::from_str(std::string{ z }));
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
} // namespace test_helper
