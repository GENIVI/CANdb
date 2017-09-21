#ifndef __CANDBC_H
#define __CANDBC_H

#include "cantypes.hpp"
#include "parserinterface.hpp"

namespace CANdb {

struct DBCParser : public ParserInterface {
    DBCParser();
    virtual bool parse(const std::string& data) noexcept override;
    virtual CANdb_t getDb() const override;

private:
    CANdb_t can_database;
};
} // namespace CANdb

#endif /* !__CANDBC_H */
