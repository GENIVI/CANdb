#include "candb.h"
#include "dbcparser.h"
#include "log.hpp"

using namespace CANdb;

ParserInterface* Parser::create(Format format)
{
    switch (format) {
    case Format::DBC:
        return new DBCParser();
        break;

    default:
        cdb_error("Parser not supporte: {}", static_cast<int>(format));
    }

    return nullptr;
}
