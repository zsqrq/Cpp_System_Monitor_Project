#include <string>
#include <iomanip>
#include <sstream>
#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
    string value;
    long hour = seconds / (60 * 60);
    long minute = (seconds/ 60) % 60;
    long secs = seconds % 60;
    std::stringstream outstring;
    outstring     << std::setfill('0') << std::setw(2) << hour << ":"
                  << std::setfill('0') << std::setw(2) << minute << ":"
                  << std::setfill('0') << std::setw(2) << secs;
    outstring >> value;

    return value;
}