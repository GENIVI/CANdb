#pragma once

#include <string>
#include <vector>

namespace StringUtils {

std::string replace_all(const std::string_view& original, const std::string& what, const std::string& withWhat);
std::vector<std::string> split(const std::string& original, const std::string& delim);

std::string erase_all(const std::string_view& original, const std::string& what);

} // namespace StringUtils
