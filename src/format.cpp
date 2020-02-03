#include <string>
#include <sstream>
#include <iomanip>

#include "format.h"

using std::string;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds[[maybe_unused]]) { 
    long minutes = seconds/60;
    long hours = minutes/60;
    minutes = minutes % 60;
    seconds = seconds % 60;
    std::ostringstream formatedTime;
    formatedTime << std::setfill('0')<<std::setw(2) <<hours << ':' 
        << std::setfill('0')<<std::setw(2)<< minutes << ':' 
        << std::setfill('0')<<std::setw(2)<< seconds;
    return formatedTime.str();
    //(to_string(hours) + ':' + to_string(minutes) + ':' + to_string(seconds)); 
}