#include <cxxopts.hpp>
#include <fstream>
#include <regex>
#include <spdlog/fmt/fmt.h>

#include "dbcparser.h"
#include "log.hpp"
#include "termcolor.hpp"

extern std::string dbc_grammar;

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

template <typename T> std::string red(T&& t)
{
    std::stringstream ss;
    ss << termcolor::colorize << termcolor::red << t << termcolor::reset;
    return ss.str();
}

template <typename T> std::string green(T&& t)
{
    std::stringstream ss;
    ss << termcolor::colorize << termcolor::green << t << termcolor::reset;
    return ss.str();
}

template <typename T> std::string blue(T&& t)
{
    std::stringstream ss;
    ss << termcolor::colorize << termcolor::blue << t << termcolor::reset;
    return ss.str();
}
template <typename T> std::string magenta(T&& t)
{
    std::stringstream ss;
    ss << termcolor::colorize << termcolor::magenta << t << termcolor::reset;
    return ss.str();
}

std::string dumpSignal(const CANsignal& sig)
{
    return fmt::format("name={},startBit={},signalSize={}", sig.signal_name,
        sig.startBit, sig.signalSize);
}

std::string dumpMessages(
    const CANdb_t& dbc, const std::string& regex, bool dumpMessages = false)
{
    std::string buff;
    buff += "messages: \n";
    for (const auto& msg : dbc.messages) {
        const bool isMatch
            = std::regex_match(msg.first.name, std::regex{ regex });
        if (isMatch) {
            buff += fmt::format("  id= {}, name= {:<30}, dlc= {}, ecu={} \n",
                green(msg.first.id), red(msg.first.name), blue(msg.first.dlc),
                magenta(msg.first.ecu));
        }

        if (dumpMessages) {
            for (const auto& signal : msg.second) {
                buff += fmt::format("     {}\n", dumpSignal(signal));
            }
        }
    }
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
    std::string regex;
    // clang-format off
    options.add_options()
    ("i,input", "Input file",cxxopts::value<std::string>(),"[path to file]")
    ("d, dump-peg", "Dump DBC grammar")
    ("m, messages", "Dump messages from DBC")
    ("t, tree", "Dump messages and signals")
    ("f, filter", "filter by messages/signals", cxxopts::value<std::string>(regex)->default_value(".*"), "regexp")
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
        std::cout << dbc_grammar << std::endl;
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
        if (options.count("m")) {
            std::cout << dumpMessages(parser.getDb(), regex);
        } else if (options.count("t")) {
            std::cout << dumpMessages(parser.getDb(), regex, true);
        }

    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }

    return success ? EXIT_SUCCESS : EXIT_FAILURE;
}
