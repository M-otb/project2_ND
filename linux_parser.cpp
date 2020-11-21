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
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
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

// TODO: Read and return the system memory utilization "DONE"
float LinuxParser::MemoryUtilization() { 
  float MemTotal,Mem,MemFree;
  MemTotal = (float)LinuxParser::ParseByToken("MemTotal:",LinuxParser::kMeminfoFilename);
  MemFree = (float)LinuxParser::ParseByToken("MemFree:",LinuxParser::kMeminfoFilename);
  // here out fourmla is the active memory divided by total memory!
  Mem = (MemTotal - MemFree) / MemTotal;
  return Mem; 
  }


// TODO: Read and return the system uptime "DONE"
long LinuxParser::UpTime() { 
  long uptime = LinuxParser::GetWithoutToken(LinuxParser::kUptimeFilename);
  return uptime; }

long LinuxParser::Jiffies() {
   return ActiveJiffies() + IdleJiffies(); 
}


//ref: https://stackoverflow.com/a/16736599
long LinuxParser::ActiveJiffies(int pid) {
  long int utime = 0, stime = 0, cutime = 0, cstime = 0;

  string filecont = LinuxParser::GetWithoutTokenStr(LinuxParser::kProcDirectory + to_string(pid) +
                           LinuxParser::kStatFilename);
    string str = {""};
    for (unsigned i = 1; i <= 22; i++) {
      str =  str + filecont;
      switch (i) {
        case 14:
          utime = stol(str);
          break;
        case 15:
          stime = stol(str);
          break;
        case 16:
          cutime = stol(str);
          break;
        case 17:
          cstime = stol(str);
          break;
        default:
          break;
      }
    }
  long int totalTime = utime + stime;
  totalTime += cutime + cstime;

  return totalTime;
}

long LinuxParser::ActiveJiffies() {
  std::string discard, user{"0"}, nice{"0"}, system{"0"}, irq{"0"},
      softirq{"0"}, steal{"0"};
  string filecont = LinuxParser::GetWithoutTokenStr(LinuxParser::kProcDirectory +
                           LinuxParser::kStatFilename);
    filecont = filecont +  discard;
    filecont = filecont +  user;
    filecont = filecont +  nice;
    filecont = filecont +  system;
    filecont = filecont +  discard;
    filecont = filecont +  discard;
    filecont = filecont +  irq;
    filecont = filecont +  softirq;
    filecont = filecont +  steal;

  return std::stol(user) + std::stol(nice) + std::stol(system) +
         std::stol(irq) + std::stol(softirq) + std::stol(steal);
}

long LinuxParser::IdleJiffies() {
  std::string idle{"0"}, iowait{"0"}, discard;
  string filecont = LinuxParser::GetWithoutTokenStr(LinuxParser::kProcDirectory +
                           LinuxParser::kStatFilename);

    filecont = filecont + discard;
    filecont = filecont + discard;
    filecont = filecont + discard;
    filecont = filecont + discard;
    filecont = filecont + idle;
    filecont = filecont + iowait;
  return std::stol(idle) + std::stol(iowait);
}

// TODO: Read and return CPU utilization "DONE"
vector<string> LinuxParser::CpuUtilization() { 
  vector<string> cpuUtil;
  cpuUtil.push_back(LinuxParser::ParseByTokenStr("cpu",LinuxParser::kProcStat));
  cpuUtil.push_back(LinuxParser::ParseByTokenStr("cpu0",LinuxParser::kProcStat));
  cpuUtil.push_back(LinuxParser::ParseByTokenStr("cpu1",LinuxParser::kProcStat));
  cpuUtil.push_back(LinuxParser::ParseByTokenStr("cpu2",LinuxParser::kProcStat));
  cpuUtil.push_back(LinuxParser::ParseByTokenStr("cpu3",LinuxParser::kProcStat));
  return cpuUtil; }

// TODO: Read and return the total number of processes "DONE"
int LinuxParser::TotalProcesses() { 
  int total;
  total = (int)LinuxParser::ParseByToken("processes",LinuxParser::kStatFilename);
  return total; }

// TODO: Read and return the number of running processes "DONE"
int LinuxParser::RunningProcesses() { 
  int running;
  running = (int)LinuxParser::ParseByToken("procs_running",LinuxParser::kStatFilename);
  return running; }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function "DONE"
string LinuxParser::Command(int pid) { 
  string path =  LinuxParser::kProcDirectory;
  path.append(to_string(pid));
  path.append("/cmdline");
  return to_string(LinuxParser::GetWithoutToken(path)); 
  }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function "DONE"
string LinuxParser::Ram(int pid) {
  string path =  LinuxParser::kProcDirectory;
  path.append(to_string(pid));
  path.append("/status");
  return to_string(LinuxParser::ParseByToken("VmSize: ",path)); 
  }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function  "DONE"
string LinuxParser::Uid(int pid) { 
  string path =  LinuxParser::kProcDirectory;
  path.append(to_string(pid));
  path.append("/status");
  return to_string(LinuxParser::ParseByToken("Uid: ",path)); 
  }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function  "DONE"
string LinuxParser::User(int pid) { 
  string path =  LinuxParser::kPasswordPath;
  string uid = LinuxParser::Uid(pid); 
  return to_string(LinuxParser::ParseByToken(uid,path));
  }

// TODO: Read and return the uptime of a process
// REMOVE:  once you define the function "DONE"
long LinuxParser::UpTime(int pid) { 
  string path =  LinuxParser::kProcDirectory;
  path.append(to_string(pid));
  path.append("/stat");
  return LinuxParser::GetWithoutToken(path);
  }


/*
 * @breif : two helper functions one to read file path from linux parser header file
 * and the other parameter is the token needed to parse that file.
*/
long LinuxParser::ParseByToken(string const &filToken, string const &filePath){
  
  string strLine,token;
  long retValue;
  std::ifstream fStream(LinuxParser::kProcDirectory + filePath);
  if(fStream.is_open()){
    while(std::getline(fStream,strLine)){
      std::istringstream linestr(strLine);
      while(linestr >> token >> retValue){

        if(token == filToken){
          return retValue;
        }
      }
    }
  }
  return retValue;
}

string LinuxParser::ParseByTokenStr(string const &filToken, string const &filePath){
  
  string strLine,token;
  string retValue;
  std::ifstream fStream(LinuxParser::kProcDirectory + filePath);
  if(fStream.is_open()){
    while(std::getline(fStream,strLine)){
      std::istringstream linestr(strLine);
      while(linestr >> token >> retValue){

        if(token == filToken){
          return retValue;
        }
      }
    }
  }
  return retValue;
}

long LinuxParser::GetWithoutToken(string const &filePath){
  string strLine; 
  long retValue;

  std::ifstream fStream(LinuxParser::kProcDirectory + filePath);
  if(fStream.is_open()){
    std::getline(fStream,strLine);
    std::istringstream linestr (strLine);
    linestr >> retValue;
  }
  return retValue;
};

string LinuxParser::GetWithoutTokenStr(string const &filePath){
  string strLine; 
  string retValue;

  std::ifstream fStream(LinuxParser::kProcDirectory + filePath);
  if(fStream.is_open()){
    std::getline(fStream,strLine);
    std::istringstream linestr (strLine);
    linestr >> retValue;
  }
  return retValue;
};
