#ifndef __CANDBC_H
#define __CANDBC_H

#include <memory>

namespace spdlog {
    class logger;
}

class CANdbc
{
public:
    CANdbc(const std::shared_ptr<spdlog::logger> &logger);
    bool load(const std::string &filename);

private:
    std::shared_ptr<spdlog::logger> mLogger;
};

#endif /* !__CANDBC_H */
