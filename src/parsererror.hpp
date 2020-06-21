
#ifndef PARSERERROR_HPP_U0EHPBRB
#define PARSERERROR_HPP_U0EHPBRB

#include <system_error>
namespace CANdb {
enum class ErrorType {
    GrammarNotCorrent,
    DataEmpty,
    ParsingFailed,
    StdErrorCode
};

struct ParserErrorCategory : std::error_category {

    const char* name() const noexcept override;
    std::string message(int condition) const override;
};

struct ParserError {
    ParserError(ErrorType t)
        : _t(t)
    {
    }
    ParserError(std::error_code ec)
        : _t(ErrorType::StdErrorCode)
    {
    }

    std::string message() const;

private:
    ErrorType _t;
};

ParserError make_error_code(ErrorType t);

} // namespace CANdb

#endif /* end of include guard: PARSERERROR_HPP_U0EHPBRB */
