#ifndef PARSER_HPP_IEWKLXBS
#define PARSER_HPP_IEWKLXBS

#include "cantypes.hpp"
#include <string>

namespace CANdb {

template <typename Derived> struct Parser {

    bool parse(const std::string& data) noexcept
    {
        Derived* d = static_cast<Derived*>(this);
        return d->parse(data);
    }

    CANdb_t getDb() const noexcept { return can_db; }

    template <typename T> void fetchData(T&&) {}

protected:
    CANdb_t can_db;
};

} // namespace CANdb

#endif /* end of include guard: PARSER_HPP_IEWKLXBS */
