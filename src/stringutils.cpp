#include "stringutils.h"
#include <string_view>

namespace StringUtils {

std::string replace_all(const std::string_view& original, const std::string& what, const std::string& withWhat)
{
    std::string result{ original };
    std::size_t index = 0;
    auto it = result.find(what, index);

    while (it != std::string::npos) {
        if (withWhat.empty()) {
            result.erase(it, what.size());
        } else {
            result.replace(it, what.size(), withWhat);
            index = it + what.size();
        }
        it = result.find(what, index);
    }

    return result;
}

std::vector<std::string> split(const std::string& original, const std::string& delim)
{
    std::vector<std::string> result{};

    std::size_t index = original.find(delim, 0), oldIndex = 0;

    while (index != std::string::npos) {
        result.push_back(original.substr(oldIndex, index - oldIndex));
        oldIndex = index + delim.size();
        index = original.find(delim, oldIndex);
    }

    result.push_back(original.substr(oldIndex, original.size() - oldIndex));

    return result;
}

std::string erase_all(const std::string_view& original, const std::string& what)
{
    return replace_all(original, what, "");
}
} // namespace StringUtils
