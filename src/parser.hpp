#ifndef PARSER_HPP_IEWKLXBS
#define PARSER_HPP_IEWKLXBS

#include "cantypes.hpp"
#include <clocale>
#include <tl/expected.hpp>
#include <string>
#include <system_error>

#include "parsererror.hpp"

namespace CANdb {

using CanDbOrError = tl::expected<CANdb_t, ParserError>;
template <typename Derived> struct Parser {

    CanDbOrError parse(const std::string& data) noexcept
    {
        // Store current LOCALE
        std::string prevLocale = std::setlocale(LC_ALL, nullptr);
        // To assure that std::stoX functions will produced the same results
        // on systems with different LOCALE settings
        std::setlocale(LC_ALL, "C");

        Derived* d = static_cast<Derived*>(this);
        const auto ret = d->parse(data);

        // Restore previous LOCALE for the application
        std::setlocale(LC_ALL, prevLocale.c_str());

        return ret;
    }
};

} // namespace CANdb

#endif /* end of include guard: PARSER_HPP_IEWKLXBS */
