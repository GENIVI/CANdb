#ifndef __CANDB_H
#define __CANDB_H

#include "parser.hpp"

namespace CANdb {

struct DBCParser : public Parser<DBCParser> {
    bool parse(const std::string& data) noexcept;
};
} // namespace CANdb

#endif /* !__CANDB_H */
