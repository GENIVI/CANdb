#include "dbcparser.h"
#include "log.hpp"
#include "parser.hpp"
#include "parsererror.hpp"
#include "stringutils.h"

#include <dbc_grammar.hpp>
#include <exception>
#include <ratio>
#include <tl/expected.hpp>

#include <codecvt>
#include <deque>
#include <fstream>
#include <locale>
#include <peglib.h>
#include <string>
#include <system_error>

namespace {
template <typename T> auto take_first(T& container) -> typename T::value_type
{
    if (container.empty()) {
        throw std::runtime_error("container is empty");
    }
    auto v = container.front();
    container.pop_front();

    return v;
}

template <typename T> auto take_back(T& container) -> typename T::value_type
{
    if (container.empty()) {
        throw std::runtime_error("empty container");
    }
    const auto v = container.back();
    container.pop_back();

    return v;
}

template <typename T> auto to_vector(const T& container) -> std::vector<typename T::value_type>
{
    std::vector<typename T::value_type> ret;
    std::transform(
        std::begin(container), std::end(container), std::back_inserter(ret), [](const auto& v) { return v; });
    return ret;
}

using namespace CANdb;
using strings = std::vector<std::string>;

// Changes \r\n to \n
std::string dos2unix(const std::string& data)
{
    std::string noWindowsShit = StringUtils::replace_all(data, "\r\n", "\n");

    return noWindowsShit;
}

tl::expected<peg::parser, CANdb::ParserError> loadPegParser()
{
    peg::parser pegParser;

    pegParser.log = [](size_t l, size_t k, const std::string& s) { cdb_error("Parser log {}:{} {}", l, k, s); };

    if (!pegParser.load_grammar(dbc_grammar.c_str(), dbc_grammar.length())) {
        cdb_error("Unable to parse grammar");
        return tl::make_unexpected(CANdb::make_error_code(CANdb::ErrorType::GrammarNotCorrent));
    }

    return pegParser;
}

CANdb::CanDbOrError parse(peg::parser& pegParser, const std::string& data)
{
    if (data.empty()) {
        return tl::make_unexpected(CANdb::make_error_code(ErrorType::DataEmpty));
    }

    const auto noTabsData = dos2unix(data);
    const auto traceEnter = [](const peg::Ope&, const char* name, std::size_t, const peg::SemanticValues&,
                                const peg::Context&, const std::any&) { cdb_trace(" Parsing {} ", name); };
    const auto traceLeave = [](const peg::Ope&, const char*, std::size_t, const peg::SemanticValues&,
                                const peg::Context&, const std::any&, std::size_t) {};
    pegParser.enable_trace(traceEnter, traceLeave);
    strings phrases;
    std::deque<std::string> idents, signs, sig_sign, ecu_tokens;
    std::deque<double> numbers;

    bool mux = false;
    int muxNdx = -1;

    using PhrasePair = std::pair<std::uint32_t, std::string>;
    std::vector<PhrasePair> phrasesPairs;
    CANdb_t can_db;

    pegParser["version"] = [&can_db, &phrases](const peg::SemanticValues&) {
        if (phrases.empty()) {
            throw peg::parse_error("Version phrase not found");
        }
        can_db.version = take_back(phrases);
    };

    pegParser["phrase"]
        = [&phrases](const peg::SemanticValues& sv) { phrases.push_back(StringUtils::erase_all(sv.token(), "\"")); };

    pegParser["ns"] = [&can_db, &idents](const peg::SemanticValues& sv) {
        can_db.symbols = to_vector(idents);
        cdb_debug("Found symbols {}", sv.token());
        idents.clear();
    };

    pegParser["TOKEN"]
        = [&idents](const peg::SemanticValues& sv) { idents.push_back(StringUtils::erase_all(sv.token(), "\n")); };

    pegParser["ECU_TOKEN"] = [&ecu_tokens](const peg::SemanticValues& sv) {
        // auto s = sv.token();
        ecu_tokens.push_back(StringUtils::erase_all(sv.token(), "\n"));
    };

    pegParser["bs"] = [](const peg::SemanticValues&) {
        // TODO: Implement me
        cdb_warn("TAG BS Not implemented");
    };

    pegParser["ev"] = [](const peg::SemanticValues& sv) {
        // TODO: Implement me
        cdb_warn("TAG EV Not implemented");
        cdb_debug("Found environment variable [ev] {}", sv.token());
    };

    pegParser["sign"] = [&signs](const peg::SemanticValues& sv) {
        cdb_debug("Found sign {}", sv.token());
        signs.push_back(std::string{ sv.token() });
    };

    pegParser["sig_sign"] = [&sig_sign](const peg::SemanticValues& sv) {
        cdb_debug("Found sig_sign {}", sv.token());
        sig_sign.push_back(std::string{ sv.token() });
    };

    pegParser["bu"] = [&can_db, &idents](const peg::SemanticValues& sv) {
        can_db.ecus = to_vector(idents);
        cdb_debug("Found ecus [bu] {}", sv.token());
        idents.clear();
    };

    pegParser["bu_sl"] = [&can_db, &idents](const peg::SemanticValues& sv) {
        can_db.ecus = to_vector(idents);
        cdb_debug("Found ecus [bu] {}", sv.token());
        idents.clear();
    };

    pegParser["number"] = [&numbers](const peg::SemanticValues& sv) {
        try {
            cdb_debug("Found number {}", sv.token());
            auto number = std::stod(std::string{ sv.token() });
            cdb_trace("Found number {}", number);
            numbers.push_back(number);
        } catch (const std::exception& ex) {
            cdb_error("Unable to parse {} to a number from {}", sv.token(), sv.name());
        }
    };

    pegParser["number_phrase_pair"] = [&phrasesPairs, &numbers, &phrases](const peg::SemanticValues&) {
        phrasesPairs.push_back(std::make_pair(take_back(numbers), take_back(phrases)));
    };

    pegParser["val_entry"] = [&idents, &can_db, &phrasesPairs](const peg::SemanticValues&) {
        std::vector<CANdb_t::ValTable::ValTableEntry> tab;
        std::transform(phrasesPairs.begin(), phrasesPairs.end(), std::back_inserter(tab), [](const auto& p) {
            return CANdb_t::ValTable::ValTableEntry{ p.first, p.second };
        });
        can_db.val_tables.push_back(CANdb_t::ValTable{ take_back(idents), tab });
        phrasesPairs.clear();
    };

    pegParser["mux"] = [&mux](const peg::SemanticValues&) { mux = true; };
    pegParser["mux_ndx"] = [&muxNdx](const peg::SemanticValues& sv) { muxNdx = std::stoi(std::string{ sv.token() }); };
    pegParser["vals"] = [](const peg::SemanticValues&) { std::vector<CANdb_t::ValTable::ValTableEntry> tab; };

    std::string muxName;
    std::vector<CANsignal> signals;
    pegParser["message"] = [&can_db, &numbers, &signals, &idents, &mux, &muxNdx, &muxName](const peg::SemanticValues&) {
        cdb_debug("Found a message {} signals = {}", idents.size(), signals.size());
        if (numbers.size() < 2 || idents.size() < 2) {
            return;
        }
        auto dlc = take_back(numbers);
        auto id = take_back(numbers);
        auto ecu = take_back(idents);
        auto name = take_back(idents);

        const CANmessage msg{ static_cast<std::uint32_t>(id), name, static_cast<std::uint32_t>(dlc), ecu };
        cdb_debug("Found a message with id = {}", msg.id);
        can_db.messages[msg] = signals;
        signals.clear();
        numbers.clear();
        idents.clear();
        mux = false;
        muxNdx = -1;
        muxName = "";
    };

    pegParser["signal"] = [&idents, &numbers, &phrases, &signals, &sig_sign, &ecu_tokens, &mux, &muxNdx,
                              &muxName](const peg::SemanticValues& sv) {
        cdb_debug("Found signal {}", sv.token());

        const std::vector<std::string> receiver{ ecu_tokens.begin(), ecu_tokens.end() };
        auto unit = take_back(phrases);

        auto max = take_back(numbers);
        auto min = take_back(numbers);
        auto offset = take_back(numbers);
        auto factor = take_back(numbers);
        CANsignal::SignType valueSigned = (take_back(sig_sign) == "-") ? CANsignal::Signed : CANsignal::Unsigned;

        std::string sigMuxName;
        std::uint8_t sigMuxNdx = 0xff;

        if (muxNdx != -1) {
            sigMuxName = muxName;
            sigMuxNdx = static_cast<std::uint8_t>(muxNdx);
            muxNdx = -1;
            cdb_debug("Signal: muxName {}, muxNdx {}", muxName, sigMuxNdx);
        }

        CANsignal::ByteOrder byteOrder = (take_back(numbers) == 0) ? CANsignal::Motorola : CANsignal::Intel;

        auto signalSize = take_back(numbers);
        auto startBit = take_back(numbers);

        auto signal_name = take_back(idents);

        if (mux) {
            sigMuxName = muxName;
            sigMuxNdx = static_cast<std::uint8_t>(muxNdx);
            muxNdx = -1;
        }

        signals.push_back(
            CANsignal{ signal_name, static_cast<std::uint8_t>(startBit), static_cast<std::uint8_t>(signalSize),
                byteOrder, valueSigned, static_cast<float>(factor), static_cast<float>(offset), static_cast<float>(min),
                static_cast<float>(max), unit, receiver, sigMuxName, sigMuxNdx });
    };
    return pegParser.parse(noTabsData.c_str())
        ? CANdb::CanDbOrError(can_db)
        : tl::make_unexpected(CANdb::make_error_code(CANdb::ErrorType::ParsingFailed));
}

} // namespace

CANdb::CanDbOrError DBCParser::parse(const std::string& data) noexcept
{
    return loadPegParser().and_then(
        [&data](peg::parser pegParser) -> CANdb::CanDbOrError { return ::parse(pegParser, data); });
}
