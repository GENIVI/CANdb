#include <cstring>

// Keeping this header in a separate file for the windows newline not to
// be altered accidentially by the IDE/text editor when saving changes on linux.
// vi/nano is save, QT Creator will implicitly 'correct' the newline without notice

namespace test_data {

// This is windows newline
const std::string bo_with_window_endline = R"(BO_ 666 GTW_stupidSt: 3 NEO SG_ GTW_epasTuneRequest : 5|3@1+ (1,0) [8|-1] "" NEO)";

} // namespace test_data
