#ifndef CANTYPES_HPP_ML9DFK7A
#define CANTYPES_HPP_ML9DFK7A

#include <cstdint>
#include <map>
#include <string>
#include <vector>

struct CANsignal {

    enum ByteOrder { Motorola = 0, Intel = 1 };
    enum SignType { Unsigned = false, Signed = true};
    enum MuxIndex { /*mux slot 0 .. 253 */ MuxMaster = 0xFE, NonMuxed = 0xFF };

    // Constructor required for vs2015
    CANsignal(std::string _signal_name, std::uint8_t _startBit,
        std::uint8_t _signalSize, CANsignal::ByteOrder _byteOrder, CANsignal::SignType _valueSigned,
        float _factor, float _offset, float _min, float _max, std::string _unit,
        std::vector<std::string> _receiver, std::uint8_t _muxNdx = NonMuxed)
        : signal_name(_signal_name)
        , startBit(_startBit)
        , signalSize(_signalSize)
        , byteOrder(_byteOrder)
        , valueSigned(_valueSigned)
        , factor(_factor)
        , offset(_offset)
        , min(_min)
        , max(_max)
        , unit(_unit)
        , receiver(_receiver)
        , muxNdx(_muxNdx)
    {
    }

    std::string signal_name;
    std::uint8_t startBit;
    std::uint8_t signalSize;
    CANsignal::ByteOrder byteOrder;
    CANsignal::SignType valueSigned;
    float factor;
    float offset;
    float min;
    float max;
    std::string unit;
    std::vector<std::string> receiver;
    std::uint8_t muxNdx;

    bool operator==(const CANsignal& rhs) const
    {
        return (signal_name == rhs.signal_name) &&
               (startBit == rhs.startBit) &&
               (signalSize == rhs.signalSize) &&
               (byteOrder == rhs.byteOrder) &&
               (valueSigned == rhs.valueSigned) &&
               (factor == rhs.factor) &&
               (offset == rhs.offset) &&
               (min == rhs.min) &&
               (max == rhs.max) &&
               (muxNdx == rhs.muxNdx);
        // Theres is more to compare, yet this is the logical minimum without too much
        // compromise on performance as the following arguably do not taint the signals
        // structure
        // (unit == rhs.unit)
        // (receiver == rhs.receiver)
    }
};

struct CANmessage {
    // Constructor required for vs2015
    CANmessage(std::uint32_t _id, const std::string& _name = "",
        std::uint32_t _dlc = 0, const std::string& _ecu = "", bool _muxed = false)
        : id(_id)
        , name(_name)
        , dlc(_dlc)
        , ecu(_ecu)
        , muxed(_muxed)
    {
    }

    std::uint32_t id;
    std::string name;
    std::uint32_t dlc;
    std::string ecu;
    bool muxed;
    std::uint32_t updateCycle{ 0 };
    std::string initValue{ "" };
};

namespace std {
template <> struct less<CANmessage> {
    bool operator()(const CANmessage& lhs, const CANmessage& rhs) const
    {
        return lhs.id < rhs.id;
    }
};
} // namespace std

using CANmessages_t = std::map<CANmessage, std::vector<CANsignal>>;

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
