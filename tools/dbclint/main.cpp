#include <cxxopts.hpp>
#include <fstream>
#include <regex>
#include <system_error>

#include <spdlog/fmt/fmt.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <termcolor/termcolor.hpp>
#include <tl/expected.hpp>

#include "dbcparser.h"
#include "log.hpp"
#include "parser.hpp"
#include "parsererror.hpp"

extern std::string dbc_grammar;

namespace {
tl::expected<std::string, std::error_code> loadDBCFile(const std::string& filename)
{
    const std::string path = filename;

    std::fstream file{ path.c_str() };

    if (!file.good()) {
        return tl::make_unexpected(std::make_error_code(std::errc::no_such_file_or_directory));
    }

    std::string buff;
    std::copy(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>(), std::back_inserter(buff));

    file.close();
    return buff;
}

std::string getInputFileFromCli(const cxxopts::ParseResult& r) { return r["i"].as<std::string>(); }

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
    return fmt::format("name={},startBit={},signalSize={}", sig.signal_name, sig.startBit, sig.signalSize);
}

std::string dumpMessages(const CANdb_t& dbc, const std::string& regex, bool dumpMessages = false)
{
    std::string buff;
    buff += "messages: \n";
    for (const auto& msg : dbc.messages) {
        const bool isMatch = std::regex_match(msg.first.name, std::regex{ regex });
        if (isMatch) {
            buff += fmt::format("  id= {}, name= {:<30}, dlc= {}, ecu={} \n", green(msg.first.id), red(msg.first.name),
                blue(msg.first.dlc), magenta(msg.first.ecu));
        }

        if (dumpMessages) {
            for (const auto& signal : msg.second) {
                buff += fmt::format("     {}\n", dumpSignal(signal));
            }
        }
    }
    return buff;
}

CANdb::CanDbOrError parse(const std::string& dbc)
{
    CANdb::DBCParser parser;
    return parser.parse(dbc);
}

} // namespace

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

int main(int argc, char* argv[])
{
    cxxopts::Options options(argv[0], "dbc lint");
    std::string regex;
    // clang-format off
    options.add_options()
    ("i,input", "Input file",cxxopts::value<std::string>(),"[path to file]")
    ("d, dump-peg", "Dump DBC grammar")
    ("m, messages", "Dump messages from DBC", cxxopts::value<bool>())
    ("t, tree", "Dump messages and signals")
    ("f, filter", "filter by messages/signals", cxxopts::value<std::string>(regex)->default_value(".*"), "regexp")
    ("h,help", "show help message");
    ;
    // clang-format on

    try {

        const auto res = options.parse(argc, argv);
        if (res.count("h") != 0) {
            std::cout << options.help({ "" }) << std::endl;
            return EXIT_SUCCESS;
        }

        if (res.count("dump-peg") != 0) {
            std::cout << "Dumping\n";
            std::cout << dbc_grammar << std::endl;
            return EXIT_SUCCESS;
        }

        if (res.count("i") == 0) {
            std::cerr << options.help({ "" }) << std::endl;
            return EXIT_FAILURE;
        }

        bool success = true;
        const auto file = res["i"].as<std::string>();
        const bool alsoDumpMessages = res.count("t");
        loadDBCFile(getInputFileFromCli(res))
            .and_then(parse)
            .map([&regex, &alsoDumpMessages](
                     const CANdb_t& db) { std::cout << dumpMessages(db, regex, alsoDumpMessages) << "\n"; })
            .or_else([&success](const CANdb::ParserError& ec) {
                std::cerr << ec.message() << "\n";
                success = false;
            });
        ;

        return success ? EXIT_SUCCESS : EXIT_FAILURE;

    } catch (const cxxopts::option_not_exists_exception& ex) {
        std::cerr << ex.what() << std::endl;
        std::cerr << options.help({ "" }) << std::endl;
        return EXIT_FAILURE;
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        std::cerr << options.help({ "" }) << std::endl;
        return EXIT_FAILURE;
    }
}
