#include <fstream>
#include <iostream>

#include "dbcparser.h"
#include "log.hpp"
#include "vsi_serializer.hpp"

#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/vector.hpp>
#include <cxxopts.hpp>
#include <spdlog/fmt/fmt.h>

namespace {
std::string loadDBCFile(const std::string& filename)
{
    const std::string path = filename;

    std::fstream file{ path.c_str() };

    if (!file.good()) {
        throw std::runtime_error(fmt::format("File {} does not exist", filename));
    }

    std::string buff;
    std::copy(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>(), std::back_inserter(buff));

    file.close();
    return buff;
}

template <typename Archive> void serialize(const std::string& filename, CANdb_t& db)
{
    Archive ar{ std::cout };
    ar(db);
}

} // namespace

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

int main(int argc, char* argv[])
{
    cxxopts::Options options(argv[0], "dbc to json converter");
    // clang-format off
    options.add_options()
    ("i,input", "Input file",cxxopts::value<std::string>(),"[path to file]")
    ("d, debug", "Enable debug output")
    ("f, format", "Format to use", cxxopts::value<std::string>()->default_value("json"),"[xml|json|binary|cvsi]")
    ("h,help", "show help message");
    // clang-format on

    options.parse(argc, argv);

    if (options.count("h") != 0) {
        std::cout << options.help({ "" }) << std::endl;
        return EXIT_FAILURE;
    }

    if (options.count("d") != 0) {
        kDefaultLogger->set_level(spdlog::level::debug);
    }

    if (options.count("i") == 0) {
        std::cerr << options.help({ "" }) << std::endl;
        return EXIT_FAILURE;
    }

    try {
        CANdb::DBCParser parser;
        parser.parse(loadDBCFile(options["i"].as<std::string>()));
        auto db = parser.getDb();
        if (options["f"].as<std::string>() == "xml") {
            serialize<cereal::XMLOutputArchive>("dbc.xml", db);
        } else if (options["f"].as<std::string>() == "json") {
            serialize<cereal::JSONOutputArchive>("dbc.json", db);
        } else if (options["f"].as<std::string>() == "cvsi") {
            serialize<VSISerializer>("dbc.c", db);
        } else {
            // serialize<cereal::BinaryOutputArchive>("dbc.bin", db);
        }

    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
