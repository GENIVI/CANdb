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
    std::uint8_t factor;
    std::uint8_t offset;
    std::int8_t min;
    std::int8_t max;
    std::string unit;
    std::string receiver;
    CANsignalType type;

    bool operator==(const CANsignal& rhs) const
    {
        return signal_name == rhs.signal_name;
    }

    template <typename Archive> void serialize(Archive& ar)
    {
        ar(CEREAL_NVP(signal_name));
    }
};

struct CANmessage {
    std::uint32_t id;
    std::string name;
    std::uint32_t dlc;
    std::string ecu;

    template <typename Archive> void serialize(Archive& ar)
    {
        ar(CEREAL_NVP(id), CEREAL_NVP(name), CEREAL_NVP(dlc), CEREAL_NVP(ecu));
    }
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

    template <typename Archive> void serialize(Archive& ar)
    {
        ar(CEREAL_NVP(version), CEREAL_NVP(nodes), CEREAL_NVP(symbols),
            CEREAL_NVP(ecus), CEREAL_NVP(messages));
    }
};

#endif /* end of include guard: CANTYPES_HPP_ML9DFK7A */
