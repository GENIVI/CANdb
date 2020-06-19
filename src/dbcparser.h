#ifndef __CANDB_H
#define __CANDB_H

#include "parser.hpp"
#include <system_error>

namespace CANdb {

struct DBCParser : public Parser<DBCParser> {
    CanDbOrError parse(const std::string& data) noexcept;
};
} // namespace CANdb

#endif /* !__CANDB_H */
