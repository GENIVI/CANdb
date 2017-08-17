#include "dbcparser.h"
#include "log.hpp"
#include <fstream>

using namespace CANdb;

DBCParser::DBCParser()
{
}

bool DBCParser::parse(const std::string& filename)
{
    std::ifstream file;
    file.open(filename);

    if (file.is_open()) {
        processFile(file);
    } else {
        cdb_error("Failed to open: {}", filename);
    }

    return false;
}

CANdb_t& DBCParser::getDb()
{
    CANdb_t ret;
    return ret;
}

void DBCParser::processFile(std::ifstream& stream)
{
    std::string line;

    while (stream.good()) {
        std::getline(stream, line);
        cdb_debug("Line: {}", line);
    }
}
