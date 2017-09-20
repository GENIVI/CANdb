#ifndef PARSERINTERFACE_HPP_IEWKLXBS
#define PARSERINTERFACE_HPP_IEWKLXBS

#include "cantypes.hpp"
#include <string>

namespace CANdb {

struct ParserInterface {
    virtual bool parse(const std::string& data) noexcept = 0;
    virtual CANdb_t getDb() const = 0;
};

} // namespace CANdb

#endif /* end of include guard: PARSERINTERFACE_HPP_IEWKLXBS */
