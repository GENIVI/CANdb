#ifndef PARSER_HPP_IEWKLXBS
#define PARSER_HPP_IEWKLXBS

#include "cantypes.hpp"
#include <string>
#include <clocale>

namespace CANdb {

template <typename Derived> struct Parser {

    bool parse(const std::string& data) noexcept
    {
        // Store current LOCALE
        std::string prevLocale = std::setlocale(LC_ALL, nullptr);
        // To assure that std::stoX functions will produced the same results
        // on systems with different LOCALE settings
        std::setlocale(LC_ALL, "C");

        Derived* d = static_cast<Derived*>(this);
        auto ret = d->parse(data);

        // Restore previous LOCALE for the application
        std::setlocale(LC_ALL, prevLocale.c_str());

        return ret;
    }

    CANdb_t getDb() const noexcept { return can_db; }

    template <typename T> void fetchData(T&&) {}

protected:
    CANdb_t can_db;
};

} // namespace CANdb

#endif /* end of include guard: PARSER_HPP_IEWKLXBS */
