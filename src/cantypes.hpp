#ifndef CANTYPES_HPP_ML9DFK7A
#define CANTYPES_HPP_ML9DFK7A

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include <cereal/archives/xml.hpp>

enum class CANsignalType { Int, Float, String };

struct CANsignal {
    std::string signal_name;
    std::uint8_t startBit;
    std::uint8_t signalSize;
    std::uint8_t byteOrder;
    std::string value_type;
    float factor;
    float offset;
    float min;
    float max;
    std::string unit;
    std::string receiver;

    bool operator==(const CANsignal& rhs) const
    {
        return signal_name == rhs.signal_name;
    }
};

struct CANmessage {
    std::uint32_t id;
    std::string name;
    std::uint32_t dlc;
    std::string ecu;
};

namespace std {
template <> struct less<CANmessage> {
    bool operator()(const CANmessage& lhs, const CANmessage& rhs) const
    {
        return lhs.id < rhs.id;
    }
};
} // namespace std

struct CANdb_t {
    struct ValTable {
        std::string identifier;

        struct ValTableEntry {
            std::uint32_t id;
            std::string ident;
        };
        std::vector<ValTableEntry> entries;
    };

    std::map<CANmessage, std::vector<CANsignal>> messages;
    std::string version;
    std::vector<std::string> nodes;
    std::vector<std::string> symbols;
    std::vector<std::string> ecus;
    std::vector<ValTable> val_tables;
};

#endif /* end of include guard: CANTYPES_HPP_ML9DFK7A */
