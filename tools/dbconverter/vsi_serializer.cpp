#include "vsi_serializer.hpp"
#include "cantypes.hpp"

#include <spdlog/fmt/fmt.h>

std::string dumpMessages(
    const std::map<CANmessage, std::vector<CANsignal>>& msgs)
{
    std::string buff;
    for (const auto& p : msgs) {

        for (const auto& signal : p.second) {
            auto signal_c_desc =
                R"({{
                .canId = 0x{can_id:x}
                .sigName = "{signal_name}"
                .start = {start},
                .size = {size},
                .byteOrder = {order},
                .valueSigned = {type},
                .factor = {factor},
                .offset = {offset},
                .min = {min},
                .max = {max},
                .unit = {unit},
                .receiver = {receiver}
}},)";
            using namespace fmt::literals;
            auto ss = fmt::format(signal_c_desc, "can_id"_a = p.first.id,
                "signal_name"_a = signal.signal_name, "start"_a = signal.startBit,
                "size"_a = signal.signalSize, "factor"_a = signal.factor, "offset"_a = signal.offset,
                "min"_a = signal.min, "max"_a = signal.max, "order"_a = signal.byteOrder,
                "type"_a = signal.valueSigned, "unit"_a = signal.unit, "receiver"_a = signal.receiver);
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
    _os << fmt::format(dbc_c_file, "messages"_a = dumpMessages(messages),
               "struct_name"_a = "can_signal_array")
        << std::endl;
}
