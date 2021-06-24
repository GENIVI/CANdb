
#ifndef PARSERERROR_HPP_U0EHPBRB
#define PARSERERROR_HPP_U0EHPBRB

#include <system_error>
namespace CANdb {
enum class ErrorType { GrammarNotCorrent, DataEmpty, ParsingFailed, DBCEncodedIncorrectly, StdErrorCode };

struct ParserErrorCategory : std::error_category {

    const char* name() const noexcept override;
    std::string message(int condition) const override;
};

struct ParserError {
    ParserError(ErrorType, const std::string& mess)
        : _mess(mess)
    {
    }
    ParserError(std::error_code)
    {
    }

    std::string message() const;

private:
    const std::string _mess;
};

ParserError make_error_code(ErrorType t, const std::string& mes = "");

} // namespace CANdb

#endif /* end of include guard: PARSERERROR_HPP_U0EHPBRB */
