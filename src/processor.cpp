#include<string>
#include<vector>

#include "processor.h"
#include "linux_parser.h"

using namespace LinuxParser;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    std::vector<long> jiffies = Jiffies();

    long userTime = jiffies[CPUStates::kUser_] - jiffies[CPUStates::kGuest_];
    long niceTime =  jiffies[CPUStates::kNice_] - jiffies[CPUStates::kGuestNice_];

    long idleAllTime = jiffies[CPUStates::kIdle_] + jiffies[CPUStates::kIOwait_];
    
    long systemAllTime = jiffies[CPUStates::kSystem_] + jiffies[CPUStates::kIRQ_] + jiffies[CPUStates::kSoftIRQ_];
    long virtualAllTime = jiffies[CPUStates::kGuest_] + jiffies[CPUStates::kGuestNice_];
    long totalTime = userTime + niceTime + systemAllTime + idleAllTime + jiffies[CPUStates::kSteal_] + virtualAllTime;

    long totalDelta = totalTime - prevTotal;
    long idleDelta = idleAllTime - prevIdle;

    prevTotal = totalTime;
    prevIdle = idleAllTime;

    return ((float)(totalDelta - idleDelta))/totalDelta  ; 
}