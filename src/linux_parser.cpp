#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
// Return name of the os
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_'); // replace space with _
      std::replace(line.begin(), line.end(), '=', ' '); // replace = with space
      std::replace(line.begin(), line.end(), '"', ' '); // replace " with space
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
// Return the version of os kernel
string LinuxParser::Kernel() {
  string os, kernel, verison;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> verison >> kernel;
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
        //pids.push_back(pid);
        pids.emplace_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Done: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
    string line1, line2;
    string key;
    string memtotal;
    string memfree;
    std::ifstream filestream(kProcDirectory + kMeminfoFilename);
    if (filestream.is_open()){
        std::getline(filestream, line1);
        std::istringstream  linestream1(line1);
        linestream1 >> key >> memtotal;

        std::getline(filestream, line2);
        std::istringstream linestream2(line2);
        linestream2 >> key >> memfree;
        filestream.close();
    }
    return (stof(memtotal) - stof(memfree)) / stof(memtotal);
}

// Done: Read and return the system uptime
long LinuxParser::UpTime() {
    string uptime, idletime;
    string line;
    std::string::size_type sz;   // alias of size_t
    std::ifstream stream(kProcDirectory + kUptimeFilename);
    if (stream.is_open()){
        std::getline(stream, line);
        std::istringstream linestream(line);
        linestream >> uptime >> idletime;
    }
    return std::stol(uptime, &sz);
}

// Done: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
    long sys_Jiffies = ActiveJiffies() + IdleJiffies();
    return sys_Jiffies;
}

// Done: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// Done: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
    std::vector<string> Cpu_Vector = LinuxParser::CpuUtilization();
    long active_Jiffies = std::stol(Cpu_Vector[CPUStates::kUser_]) + std::stol(Cpu_Vector[CPUStates::kGuest_])
            + std::stol(Cpu_Vector[CPUStates::kGuestNice_]) + std::stol(Cpu_Vector[CPUStates::kIRQ_])
            + std::stol(Cpu_Vector[CPUStates::kNice_]) + std::stol(Cpu_Vector[CPUStates::kSoftIRQ_])
            + std::stol(Cpu_Vector[CPUStates::kSteal_]) + std::stol(Cpu_Vector[CPUStates::kSystem_]);
    return active_Jiffies;
}

// Done: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
    std::vector<string> Cpu_Vector =  LinuxParser::CpuUtilization();
    long idleJiffies = std::stol(Cpu_Vector[CPUStates::kIdle_]) + std::stol(Cpu_Vector[CPUStates::kIOwait_]);
    return idleJiffies;
}

// Done: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
    string User_;
    std::vector<string> cpu_vector;
    string line;
    string value;
    std::ifstream filestream(kProcDirectory + kStatFilename);

    if (filestream.is_open()) {
        while (std::getline(filestream, line)) {
            std::istringstream linestream(line);
            while (linestream >> value) {
                if(value == "cpu") {
                    while(linestream >> value)
                        cpu_vector.push_back(value);
                    return cpu_vector;
                }
            }
        }
    }
    return cpu_vector;
}

// Done: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
    string line;
    string key;
    string value;
    std::ifstream filestream(kProcDirectory + kStatFilename);
    if (filestream.is_open()){
        while (std::getline(filestream, line)){
            std::istringstream linestream(line);
            while (linestream >> key >> value){
                if (key == "processes"){
                    return std::stoi(value);
                }
            }
        }
    }
    return std::stoi(value);
}

// Done: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
    string line, key, value;
    std::ifstream filestream(kProcDirectory + kStatFilename);
    if (filestream.is_open()){
        while (std::getline(filestream, line)){
            std::istringstream linestream(line);
            while (linestream >> key >> value){
                if (key == "procs_running"){
                    return std::stoi(value);
                }
            }
        }
    }
    return std::stoi(value);
}

// Done: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
    string line;
    std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
    if (filestream.is_open()){
        std::getline(filestream, line);
        filestream.close();
    }
    return line;
}
// Done: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
    string line, key, value, units;
    std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
    if (filestream.is_open()){
        while (std::getline(filestream, line)){
            std::istringstream linestream(line);
            while (linestream >> key >> value >> units){
                if (key == "VmSize:"){
                    return std::to_string(stoi(value) / 1024);
                }
            }
        }
    }
    return string();
}

// Done: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
    string line, key, value;
    std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
    if (filestream.is_open()){
        while (std::getline(filestream, line)){
            std::istringstream linestream(line);
            while (linestream >> key >> value){
                if (key == "Uid:"){
                    return value;
                }
            }
        }
    }
    return string();
}

// Done: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
    string line;
    string token = ":x:" + to_string(pid) + ":";
    //std::cout << "token: " << token << "\n";
    std::ifstream filestream(kPasswordPath);

    if (filestream.is_open()) {
        while (std::getline(filestream, line)) {
            std::istringstream linestream(line);
            std::size_t found = line.find(token);

            //std::cout << "found: " << found << "\n";
            if (found!=std::string::npos) {
                //std::cout << "line: " << line << "\n";
                std::string user = line.substr (0,found);
                //std::cout << "found: " << found << ", user: " << user << '\n';
                return user;
            }
        }
    }
    return string();
}

// Done: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
    long int uptime{0};
    string value;
    std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
    if (filestream.is_open()){
        for (int i = 0; filestream >> value; ++i) {
            if (i == 13){
                uptime = stol(value) / sysconf(_SC_CLK_TCK);
                return uptime;
            }
        }
    }
    return uptime;
}
// Done: Return pid's cpu utilization
float LinuxParser::pidCpuUtilization(int pid){
    float sysuptime = 0;
    long utime = 0;    // CPU time spent in user code in clock ticks
    long stime = 0;    // CPU time spent in kernel code in clock ticks
    long cutime = 0;   // Waited for children's cpu time spent in user code
    long cstime = 0;   // Waited for children's cpu time spent in kernel code
    long starttime = 0;// Time of the process started in clock ticks
    long value{0};
    float total_time{0.0}, seconds{0.0}, CPU_Utilization{0.0};
    string line;
    long Hertz = sysconf(_SC_CLK_TCK);
    std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
    if (filestream.is_open()){
        std::getline(filestream, line);
        std::istringstream linestream(line);
        for (int i = 1; i < 23; i++) {
            value = 0;
            linestream >> value;
            if (i == 14) utime = value;
            if (i == 15) stime = value;
            if (i == 16) cutime = value;
            if (i == 17) cstime = value;
            if (i == 22) starttime = value;
        }
    }
    total_time = (float)(utime + stime + cutime + cstime);
    sysuptime = LinuxParser::UpTime();
    seconds = (float)sysuptime - ((float)starttime / (float)Hertz);
    CPU_Utilization = (total_time / (float)Hertz) / (float)seconds;
    return CPU_Utilization;
}