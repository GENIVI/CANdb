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

// This is windows newline
const std::string bo_with_window_endline = R"(BO_ 666 GTW_stupidSt: 3 NEO SG_ GTW_epasTuneRequest : 5|3@1+ (1,0) [8|-1] "" NEO)";

} // namespace test_data
