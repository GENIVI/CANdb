#ifndef __CANDBC_H
#define __CANDBC_H

#include "parser.hpp"

namespace CANdb {

struct DBCParser : public Parser<DBCParser> {
    bool parse(const std::string& data) noexcept;
};
} // namespace CANdb

#endif /* !__CANDBC_H */
