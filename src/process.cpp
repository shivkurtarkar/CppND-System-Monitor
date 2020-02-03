#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
  // long utime;
  // long stime;
  // long cutime;
  // long cstime;
  //long total_time = utime + stime + cutime + cstime;
  std::vector<long> jiffies = LinuxParser::Jiffies(Pid());
  long total_time = 0;
  for(long jiffie: jiffies){
    total_time += jiffie;
  }
  long startTime = LinuxParser::UpTime(Pid());
  long system_uptime = LinuxParser::UpTime();
  float seconds = ((float)system_uptime) -((float)startTime / sysconf(_SC_CLK_TCK));
  cpu_usage_ = 100*(((float)total_time / sysconf(_SC_CLK_TCK))/seconds);
  return cpu_usage_; 
}

// TODO: Return the command that generated this process
string Process::Command() { return command_; }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(Pid()) ; }

// TODO: Return the user (name) that generated this process
string Process::User() { return user_.substr(0,9);}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(Pid()); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
  return (cpu_usage_ < a.cpu_usage_);
}