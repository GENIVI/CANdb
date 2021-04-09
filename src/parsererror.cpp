#include "parsererror.hpp"

namespace CANdb {

const char* ParserErrorCategory::name() const noexcept { return "ParserError"; }
std::string ParserErrorCategory::message(int condition) const { return "ParserError"; }

std::string ParserError::message() const { return _mess; }
ParserError make_error_code(ErrorType t, const std::string& msg) { return ParserError{ t, msg }; }
} // namespace CANdb
