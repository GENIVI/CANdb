#include "log.hpp"
#include <gtest/gtest.h>
#include <peglib.h>

std::shared_ptr<spdlog::logger> kDefaultLogger
    = []() -> std::shared_ptr<spdlog::logger> {
    auto z = std::getenv("CDB_LEVEL");
    auto logger = spdlog::stdout_color_mt("cdb");

    if (z == nullptr) {
        logger->set_level(spdlog::level::err);
    } else {
        const std::string ll{ z };

        auto it = std::find_if(std::begin(spdlog::level::level_names),
            std::end(spdlog::level::level_names),
            [&ll](const char* name) { return std::string{ name } == ll; });

        if (it != std::end(spdlog::level::level_names)) {
            int i = std::distance(std::begin(spdlog::level::level_names), it);
            logger->set_level(static_cast<spdlog::level::level_enum>(i));
        }
    }

    return logger;
}();
struct PegParser : public ::testing::Test {
    PegParser()
    {
        parser.log = [](size_t l, size_t k, const std::string& s) {
            cdb_error("Parser log {}:{} {}", l, k, s);
        };
    }
    peg::parser parser;
};

TEST_F(PegParser, ns_grammar)
{
    ASSERT_TRUE(parser.load_grammar(
        R"(grammar <- symbols EndOfFile
            symbols   <- ('NS_' s* ':' s* NewLine (symbol_name)* _)?
            symbol_name <- '  ' TOKEN

            s         <- [ \t]
            TOKEN     <- < [a-zA-Z0-9'_']+ > _
            sign      <- < [-+]? > _
            NewLine   <- [\r\n]
            phrase    <- < '"' (!'"' .)* '"' >
            number    <- < sign [0-9]+ > _
            EndOfFile <- !.
            ~_        <- [\t\r\n]*
            ~__       <- ![a-zA-Z0-9]

            )"));

    {
        auto bo = R"(NS_: 


)";
        EXPECT_TRUE(parser.parse(bo));
    }
    {
        auto bo = R"(NS_: 
  NS_DESC
  NS_DESC2


)";
        EXPECT_TRUE(parser.parse(bo));
    }

    {
        auto bo = R"(NS_: 
  NS_DESC


)";
        EXPECT_TRUE(parser.parse(bo));
    }
    {
        auto bo = R"(NS_: 
  NS_DESC


)";
        EXPECT_TRUE(parser.parse(bo));
    }
}

TEST_F(PegParser, parse_message_grammar)
{
    ASSERT_TRUE(parser.load_grammar(
        R"(grammar <- message* _ EndOfFile
            message   <- 'BO_' s* number s* TOKEN ':' s number s TOKEN _ signal*
            signal <- (s* 'SG_' s* TOKEN s* ':' s* number '|' number '@' number sign s* '(' number ',' number ')' s* '[' number '|' number ']' s* phrase s* TOKEN _)

            s         <- [ \t]
            TOKEN     <- < [a-zA-Z0-9'_']+ > _
            sign      <- < [-+]? > _
            NewLine   <- [\r\n]
            phrase    <- < '"' (!'"' .)* '"' >
            number    <- float / integer 
            integer   <- < sign? [0-9]+ > _
            float     <- < sign? [0-9]+'.'[0-9]+ > _
            EndOfFile <- !.
            ~_        <- [\t\r\n]*
            ~__       <- ![a-zA-Z0-9]

            )"));

    auto bo = R"(BO_ 257 GTW_epasControl: 3 NEO
  SG_ GTW_epasControlChecksum : 16|8@1+ (1,0) [0|255] "" NEO
  SG_ GTW_epasControlChecksum : 16|8@1+ (1,0) [0|255] "" NEO
  SG_ GTW_epasEmergencyOn : 0|1@1+ (1,0) [2|-1] "" NEO

BO_ 257 GTW_epasControl: 3 NEO
  SG_ DAS_steeringAngleRequest : 6|15@0+ (0.1,-1638.35) [-1638.35|1638.35] "deg" EPAS
  SG_ GTW_epasControlChecksum : 16|8@1+ (1,0) [0|255] "" NEO)";

    EXPECT_TRUE(parser.parse(bo));
}

TEST_F(PegParser, parse_float)
{
    ASSERT_TRUE(parser.load_grammar(
        R"(grammar <- float
          float     <- < sign? [0-9]+'.'[0-9]+ > _
          sign      <- < [-+] > _
          ~_     <- [\t\r\n]*

        )"));

    EXPECT_TRUE(parser.parse("-10.151414141414"));
}

TEST_F(PegParser, parse_number)
{
    ASSERT_TRUE(parser.load_grammar(
        R"(grammar <- number
            number    <- float / integer
            integer   <- < sign? [0-9]+ > _
            float     <- < sign? [0-9]+'.'[0-9]+ > _
            sign      <- < [-+] > _
            ~_     <- [\t\r\n]*

        )"));

    EXPECT_TRUE(parser.parse("-10.151414141414"));
    EXPECT_TRUE(parser.parse("-1638.35"));
    EXPECT_TRUE(parser.parse("15"));
}

TEST_F(PegParser, parse_number_dash_number)
{
    ASSERT_TRUE(parser.load_grammar(
        R"(grammar <- '[' number '|' number ']'
            number    <- float / integer
            integer   <- < sign? [0-9]+ > _
            float     <- < sign? [0-9]+'.'[0-9]+ > _
            sign      <- < [-+] > _
            ~_     <- [\t\r\n]*

        )"));

    EXPECT_TRUE(parser.parse("[-1638.35|-1638.35]"));
}

TEST_F(PegParser, comments)
{

    auto grammar = 
        R"(grammar                 <- comment* 

           comment                 <- '//' s* '*' ('*'* / s* (TOKEN s)*) s* '*' NewLine
           TOKEN     <- < [a-zA-Z0-9'_']+ > _
           s         <- [ \t]
           ~_     <- [\t\r\n]*
           NewLine   <- [\r\n]
    )";
    ASSERT_TRUE(parser.load_grammar(grammar));

    auto data = 
        R"(// ****************************** *
// *                                                                           *
// *                      Mentor Graphics Corporation                          *
)";

    EXPECT_TRUE(parser.parse(data));
}
