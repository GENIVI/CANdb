#include <cxxopts.hpp>
#include <fstream>

#include "Resource.h"
#include "candb.h"
#include "log.hpp"
#include <spdlog/fmt/fmt.h>

extern const char _resource_dbc_grammar_peg[];
extern const size_t _resource_dbc_grammar_peg_len;

namespace {
std::string loadDBCFile(const std::string& filename)
{
    const std::string path = filename;

    std::fstream file{ path.c_str() };

    if (!file.good()) {
        throw std::runtime_error(
            fmt::format("File {} does not exists", filename));
    }

    std::string buff;
    std::copy(std::istreambuf_iterator<char>(file),
        std::istreambuf_iterator<char>(), std::back_inserter(buff));

    file.close();
    return buff;
}
} // namespace

std::shared_ptr<spdlog::logger> kDefaultLogger
    = []() -> std::shared_ptr<spdlog::logger> {
    auto z = std::getenv("CDB_LEVEL");
    auto logger = spdlog::stdout_color_mt("cdb");

    if (z == nullptr) {
        logger->set_level(spdlog::level::err);
    } else {
        const std::string ll{ z };

        auto it = std::find_if(std::begin(spdlog::level::level_names),
            std::end(spdlog::level::level_names),
            [&ll](const char* name) { return std::string{ name } == ll; });

        if (it != std::end(spdlog::level::level_names)) {
            int i = std::distance(std::begin(spdlog::level::level_names), it);
            logger->set_level(static_cast<spdlog::level::level_enum>(i));
        }
    }

    return logger;
}();

int main(int argc, char* argv[])
{
    cxxopts::Options options(argv[0], "dbc lint");
    // clang-format off
    options.add_options()
    ("i,input", "Input file",cxxopts::value<std::string>(),"[path to file]")
    ("d, dump-peg", "Dump DBC grammar")
    ("h,help", "show help message");
    // clang-format on

    try {
        options.parse(argc, argv);
    } catch (const cxxopts::option_not_exists_exception& ex) {
        std::cerr << ex.what() << std::endl;
        std::cerr << options.help({ "" }) << std::endl;
        return EXIT_FAILURE;
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        std::cerr << options.help({ "" }) << std::endl;
        return EXIT_FAILURE;
    }

    if (options.count("h") != 0) {
        std::cout << options.help({ "" }) << std::endl;
        return EXIT_SUCCESS;
    }

    if (options.count("d") != 0) {
        Resource dbc{ _resource_dbc_grammar_peg,
            _resource_dbc_grammar_peg_len };
        auto str = dbc.toString();
        std::cout << str << std::endl;
        return EXIT_SUCCESS;
    }

    if (options.count("i") == 0) {
        std::cerr << options.help({ "" }) << std::endl;
        return EXIT_FAILURE;
    }

    bool success = false;
    try {
        CANdb::DBCParser parser;
        const auto file = options["i"].as<std::string>();
        success = parser.parse(loadDBCFile(file));

        if (success) {
            std::cout << fmt::format("DBC file {} successfully parsed", file)
                      << std::endl;
        }

    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }

    return success ? EXIT_SUCCESS : EXIT_FAILURE;
}
