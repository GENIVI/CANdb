#include <candb.h>
#include <iostream>
#include <log.hpp>
#include <memory>
#include <regex>

std::shared_ptr<spdlog::logger> kDefaultLogger;

int main()
{
    using namespace CANdb;

    kDefaultLogger = spdlog::stdout_color_mt("cdb");
    kDefaultLogger->set_level(spdlog::level::debug);

    // std::unique_ptr<ParserInterface> parser(Parser::create(Format::DBC));

    auto iface = Parser::create<CANdb::DBCFormat>();

    iface->parse("/home/remol/.vimrc");

    // std::regex regTst("^BO\\_ (\\w+) (\\w+) *: (\\w+) (\\w+)");

    // std::string strTst("BO_ 2027 RawChannel11_IocTx: 8 IviBrd2");

    // std::smatch res;

    // if(std::regex_match(strTst, res, regTst)) {
    // cdb_debug("Match! Size={}", res.size()-1);

    // for(unsigned long i = 1; i < res.size(); ++i) {
    // cdb_debug("Match {}, str: {}", i, res[i].str());
    //}
    //}

    return 0;
}
