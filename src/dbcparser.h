#ifndef __CANDBC_H
#define __CANDBC_H

#include "candb.h"

namespace CANdb {

struct DBCParser : public ParserInterface {
    DBCParser();
    virtual bool parse(const std::string& filename) override;
    virtual CANdb_t& getDb() override;

private:
    void processFile(std::ifstream& stream);
};
}

#endif /* !__CANDBC_H */
