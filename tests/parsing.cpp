#include <spdlog/spdlog.h>
#include <candbc.h>

#include <regex>
#include <iostream>

int main()
{
    spdlog::set_level(spdlog::level::debug);
    spdlog::set_pattern("[%n][%L] %v");
    auto log = spdlog::stdout_color_mt("CANdbc"); 
    CANdbc dbc(log);

    std::regex regTst("^BO\\_ (\\w+) (\\w+) *: (\\w+) (\\w+)");

    std::string strTst("BO_ 2027 RawChannel11_IocTx: 8 IviBrd2");

    std::smatch res;

    if(std::regex_match(strTst, res, regTst)) {
        log->info("Match! Size={}", res.size()-1);

        for(unsigned long i = 1; i < res.size(); ++i) {
            log->info("Match {}, str: {}", i, res[i].str());
        }
    }

    return 0;
}

