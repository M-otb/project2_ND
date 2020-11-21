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

// TODO: Return this process's ID "DONE"
int Process::Pid() { return pid_; }

// change the current pid to the passed one "DONE"
void Process::Pid(int pid) { this->pid_ =  pid; }

// TODO: Return this process's CPU utilization "DONE"
float Process::CpuUtilization() { return cpuUtil_; }

// TODO: Return the command that generated this process "DONE"
string Process::Command() { return LinuxParser::Command(pid_); }

// TODO: Return this process's memory utilization "DONE"
string Process::Ram() { return LinuxParser::Ram(pid_); }

// TODO: Return the user (name) that generated this process "DONE"
string Process::User() { return LinuxParser::User(pid_); }

// TODO: Return the age of this process (in seconds) "DONE"
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function "DONE"
bool Process::operator<(Process const& a) const { 
    bool result = (cpuUtil_ > a.cpuUtil_) ? 1 : 0;
    return result;
    
}

// "DONE to calclute the cpuUtil variable "
void Process::CalcCpuUtilz(const long totalJiffies){
    long jiffies = LinuxParser::ActiveJiffies(pid_);
    cpuUtil_ =  jiffies * 1.0 / totalJiffies;
}