#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : pid_(pid){};

// Done: Return this process's ID
int Process::Pid() const { return pid_;}

// Done: Return this process's CPU utilization
float Process::CpuUtilization() const {
    return cpu_;
}
void Process::CpuUtilization(long activeJeff, long systemHeff) {
    long active_duration = activeJeff - lastActiveJeff;
    long duration = systemHeff - lasSystemJeff;

    cpu_ = static_cast<float > (active_duration) / duration;
    lastActiveJeff = activeJeff;
    lasSystemJeff = systemHeff;
}

// Done: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(Pid()); }

// Done: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(Pid()); }

// Done: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(Pid()); }

// Done: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(Pid()); }

// Done: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
    return a.cpu_ < this->cpu_;
}