#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  long memTotal, memFree, memAvailable, buffers, cached; 
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  string line, key;
  long value;
  if (filestream.is_open()) {
    while (std::getline(filestream, line)){
      std::istringstream linestream(line);
      while (linestream >> key >> value){
        if (key == "MemTotal:"){
          memTotal = value;
        }else if (key == "MemFree:"){
          memFree = value;
        }else if (key == "MemAvailable:"){
          memAvailable = value;
        }else if (key == "Buffers:"){
          buffers = value;
        }else if (key == "Cached:"){
          cached = value;
        }
      } 
    }
  }
  return ((float) (memTotal - memFree - buffers - cached))/memTotal; 
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  float suspend, idle;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  string line;
  std::getline(filestream, line);
  std::istringstream linestream(line);
  linestream >> suspend >> idle;
  return suspend + idle;
}

// // TODO: Read and return the number of jiffies for the system
// long LinuxParser::Jiffies() { 
// return 0;
// }

// // TODO: Read and return the number of active jiffies for a PID
// // REMOVE: [[maybe_unused]] once you define the function
// long LinuxParser::ActiveJiffies(int pid) {
//   std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
//   std::string line, value;
//   std::getline(filestream, line);
//   std::istringstream linestream(line);
//   long utime{0}, stime{0}, cutime{0}, cstime{0};
//   for(int i(1); linestream >> value && i<=17; i++){
//     if (i==14)
//       utime = std::stoi(value);
//     else if (i==15)
//       stime = std::stoi(value);
//     else if (i==16)
//       cutime = std::stoi(value);
//     else if (i==17)
//       cstime = std::stoi(value);
//   }
//   return utime + stime + cutime + cstime; 
// }

// // TODO: Read and return the number of active jiffies for the system
// long LinuxParser::ActiveJiffies() { return 0; }

// // TODO: Read and return the number of idle jiffies for the system
// long LinuxParser::IdleJiffies() { return 0; }

// // TODO: Read and return CPU utilization
// vector<string> LinuxParser::CpuUtilization() { 
//   std::string line, cpu, value;
//   std::ifstream filestream(kProcDirectory + kStatFilename);
//   std::getline(filestream, line);
//   vector<string> cpuString;
//   cpuString.push_back(line);
//   return cpuString;
//   // std::istringstream linestream(line);
//   // vector<string> cpuValues;
//   // linestream >> cpu;
//   // while(linestream >> value){
//   //   cpuValues.push_back(value);
//   // }
//   //return cpuValues; 
// }

std::vector<long> LinuxParser::Jiffies(){
  std::string line, cpu;
  long value;
  std::vector<long> jiffies;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  std::getline(filestream, line);
  std::istringstream linestream(line);
  linestream >> cpu;
  while(linestream >> value){
    jiffies.push_back(value);
  }
  return jiffies;
}
std::vector<long> LinuxParser::Jiffies(int pid){
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  std::string line, value;
  std::getline(filestream, line);
  std::istringstream linestream(line);
  std::vector<long> jiffies;
  for(int i(1); linestream >> value && i<=17; i++){
    if(i>=14 && i<=17)
      jiffies.push_back(std::stoi(value));
  }
  return jiffies;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  int totalProcesses;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  std::string line, key;
  int value;
  while(std::getline(filestream, line)){
    std::istringstream linestream(line);
    while(linestream >> key >> value){
      if(key == "processes"){
        totalProcesses = value;
        return totalProcesses;
      }
    }
  }
  return totalProcesses; 
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  std::ifstream filestream(kProcDirectory + kStatFilename);
  std::string line, key;
  int value;
  while(std::getline(filestream, line)){
    std::istringstream linestream(line);
    while(linestream >> key >> value){
      if(key == "procs_running"){
        return value;
      }
    }
  }
  return value; 
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  std::string line;
  std::getline(filestream, line);
  return line;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  std::ifstream filestream(kProcDirectory+std::to_string(pid)+kStatusFilename);
  std::string line, key, value;
  while(std::getline(filestream, line)){
    std::istringstream linestream(line);
    while(linestream >> key>> value){
      if(key == "VmSize:"){
        std::ostringstream ramFormating;
        ramFormating << std::setfill('0') << std::setw(3)<< (std::stoi(value)/1024);
        return ramFormating.str();
      }
    }
  }
  return "  "; 
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  std::ifstream filestream(kProcDirectory+std::to_string(pid)+kStatusFilename);
  std::string line, key, value;
  while(std::getline(filestream, line)){
    std::istringstream linestream(line);
    while(linestream >> key>> value){
      if(key == "Uid:"){
        return value;
      }
    }
  }
  return value; 
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  std::string uid = LinuxParser::Uid(pid);
  std::ifstream filestream(kPasswordPath);
  std::string line, username, pass, value;
  while(std::getline(filestream, line)){
    std::replace(line.begin(), line.end(), ' ', '_');
    std::replace(line.begin(), line.end(), ':', ' ');
    std::istringstream linestream(line);
    while(linestream >> username>> pass>> value){
      if(value == uid){
        return username;
      }
    }
  }
  return username; 
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  std::string line, value;
  std::getline(filestream, line);
  std::istringstream linestream(line);
  for(int i(1); linestream >> value && i<=22; i++){
    if(i==22)
      return (std::stoi(value)/sysconf(_SC_CLK_TCK)) ;
  }
  return 0; 
}