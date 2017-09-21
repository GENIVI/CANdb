#ifndef __CANDB_H
#define __CANDB_H

#include <map>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

#include "dbcparser.h"
#include "parserinterface.hpp"

template <typename T, typename Tuple> struct tuple_has;

template <typename T> struct tuple_has<T, std::tuple<>> : std::false_type {
};

template <typename T, typename T2, typename... Ts>
struct tuple_has<T, std::tuple<T2, Ts...>> : tuple_has<T, std::tuple<Ts...>> {
};

template <typename T, typename... Ts>
struct tuple_has<T, std::tuple<T, Ts...>> : std::true_type {
};

namespace CANdb {

struct DBCFormat {
    using ParserType = DBCParser;
};

struct Parser {
    template <class... T> struct type_list {
    };
    using supported = std::tuple<DBCFormat>;

    template <typename ParserType>
    static std::unique_ptr<ParserInterface> create()
    {
        static_assert(tuple_has<ParserType, supported>::value, "");
        return std::make_unique<typename ParserType::ParserType>();
    }
};
} // namespace CANdb
#endif /* !__CANDB_H */
