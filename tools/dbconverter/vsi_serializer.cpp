#include "vsi_serializer.hpp"
#include "cantypes.hpp"

#include <spdlog/fmt/fmt.h>

std::string getType(const CANsignal& signal)
{
    if (signal.byteOrder < 2) {
        if (signal.value_type == "+") {
            return "UNSIGNED_INT";
        }
        return "SIGNED_INT";
    }
    switch (signal.byteOrder) {
    case 2:
        return "SP_FLOAT";
        break;
    case 3:
        return "DP_FLOAT";
        break;
    case 4:
        return "STRING";
        break;
    }

    throw std::runtime_error("Can't map signal to type");
    return "";
}

std::string dumpMessages(const std::map<CANmessage, std::vector<CANsignal>>& msgs)
{
    std::string buff;
    for (const auto& p : msgs) {

        for (const auto& signal : p.second) {
            auto signal_c_desc =
                R"({{
                .canId = 0x{can_id:x}
                .sigId = {signal_id}
                .sigName = "{signal_name}"
                .start = {start},
                .end = 0,
                .min = {min},
                .max = {max},
                .type = {type}
}},)";
            using namespace fmt::literals;
            auto ss = fmt::format(signal_c_desc, "can_id"_a = p.first.id, "signal_id"_a = 0,
                "signal_name"_a = signal.signal_name, "start"_a = signal.startBit, "min"_a = signal.min,
                "max"_a = signal.max, "type"_a = getType(signal));
            buff += ss + std::string{ "\n" };
        }
    }
    return buff;
}

VSISerializer::VSISerializer(std::ostream& os)
    : _os(os)
{
}

void VSISerializer::operator()(const CANdb_t& db)
{
    const auto messages = db.messages;
    auto dbc_c_file =
        R"(#include <stdint.h>
#include "can-signals.h"

struct CanSignal {struct_name}[] = {{
    {messages}
}}
)";

    using namespace fmt::literals;
    _os << fmt::format(dbc_c_file, "messages"_a = dumpMessages(messages), "struct_name"_a = "can_signal_array")
        << std::endl;
}
