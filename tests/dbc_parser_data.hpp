#include <cstring>

namespace test_data {
const std::string bo1 = R"(BO_ 1160 DAS_steeringControl: 4 NEO
  SG_ DAS_steeringControlType : 23|2@0+ (1,0) [0|0] "" EPAS
  SG_ DAS_steeringControlChecksum : 31|8@0+ (1,0) [0|0] "" EPAS
  SG_ DAS_steeringControlCounter : 14|7@0+ (15.0, 0.0) [0.0|1425.0] "Mins" Vector_XXX
  SG_ DAS_steeringHapticRequest : 7|1@0+ (1,0) [0|0] "" EPAS
  SG_ DAS_BOOT : 14|7@0+ (15.0, 0.0) [0.0|1.84467440737096E+019] "Mins" EPAS
  SG_ DAS_ASD : 30|28@1+ (1E-008,0) [0|180] "Unit_ZLO"  Vector_XXX
)";

const std::string bo2 = R"(BO_ 257 GTW_epasControl: 3 NEO
  SG_ GTW_epasControlChecksum : 16|8@1+ (1,0) [0|255] "" NEO
  SG_ GTW_epasControlCounter : 12|4@1+ (1,0) [0|15] "" NEO
  SG_ GTW_epasControlType : 8|2@1+ (1,0) [4|-1] "" NEO
  SG_ GTW_epasEmergencyOn : 0|1@1+ (1,0) [2|-1] "" NEO
  SG_ GTW_epasLDWEnabled : 11|1@1+ (1,0) [2|-1] "" NEO
  SG_ GTW_epasPowerMode : 1|4@1+ (1,0) [4|14] "" NEO
  SG_ GTW_epasTuneRequest : 5|3@1+ (1,0) [8|-1] "" NEO)";

const std::string bo_signed_sigs = R"(BO_ 1091 FKD_Gyro_04: 6 NEO
  SG_ FKD_GyroRoll : 32|9@1- (0.1,0) [-25.6|25.5] "" Vector__XXX
  SG_ FKD_GyroPitch : 16|9@1- (0.1,0) [-25.6|25.5] "" Vector__XXX
  SG_ FKD_GyroHead : 0|12@1+ (0.1,0) [0|409.5] "" Vector__XXX
)";

const std::string bo_muxed_signals = R"(BO_ 2589283496 MN_MuxMsg_01: 8 MYNODE
  SG_ MN_MUX M  : 0|2@1+ (1.0,0.0) [0.0|3] ""  Vector__XXX
  SG_ MN_NonMuxed : 2|1@1+ (1.0,0.0) [0.0|1] ""  Vector__XXX
  SG_ MN_MuxSig0_1 m0  : 3|4@1+ (1.0,0.0) [0.0|15] ""  Vector__XXX
  SG_ MN_MuxSig1_1 m1  : 4|12@1+ (0.1,0) [0.0|409.5] ""  Vector__XXX
  SG_ MN_MuxSig0_2 m0  : 7|1@1+ (1.0,0.0) [0.0|1] ""  Vector__XXX
  SG_ MN_MuxSig0_3 m0  : 8|27@1+ (0.000001,0) [0.000000|90.000000] ""  Vector__XXX
  SG_ MN_MuxSig1_2 m1  : 16|14@1+ (1,0) [0|16383] ""  Vector__XXX
  SG_ MN_MuxSig1_3 m1  : 30|2@1+ (1.0,0.0) [0.0|3] ""  Vector__XXX
  SG_ MN_MuxSig1_4 m1  : 32|32@1+ (1,0) [0|4294967295] ""  Vector__XXX
  SG_ MN_MuxSig0_4 m0  : 35|1@1+ (1.0,0.0) [0.0|1] ""  Vector__XXX
  SG_ MN_MuxSig0_5 m0  : 36|28@1+ (0.000001,0) [0.000000|180.000000] ""  Vector__XXX
)";

} // namespace test_data

// This is windows newline
#include "dbc_parser_data_win.hpp"

