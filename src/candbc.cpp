#include <fstream>
#include <spdlog/spdlog.h>
#include "candbc.h"

CANdbc::CANdbc(const std::shared_ptr<spdlog::logger> &logger) :
    mLogger(logger)
{
    mLogger->debug(__FUNCTION__);
}

bool CANdbc::load(const std::string &filename)
{
    std::string line;
    std::ifstream file;

    file.open(filename);

    while (std::getline(file, line)) {


    }

    return true;
}

