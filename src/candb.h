#ifndef __CANDB_H
#define __CANDB_H

#include <string>
#include <map>
#include <vector>

namespace CANdb {

enum class Format {
    DBC
};

enum class CANsignalType {
    Int,
    Float,
    String
};

struct CANsignal {
    std::string name;
    uint8_t startBit;
    uint8_t len;
    CANsignalType type;
};

struct CANmessage {
    uint32_t id;
    std::string name;
};

typedef std::map<CANmessage, std::vector<CANsignal> > CANdb_t;

struct ParserInterface {
    virtual bool parse(const std::string& filename) = 0;
    virtual CANdb_t& getDb() = 0;
};

struct Parser {
    static ParserInterface* create(Format);
};
}
#endif /* !__CANDB_H */
