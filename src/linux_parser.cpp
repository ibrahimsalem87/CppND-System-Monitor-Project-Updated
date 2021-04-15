#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <vector>

using std::stof;
using std::stoi;
using std::stol;
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

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  float memory_utilization, mem_free, mem_total, mem_available, buffers;
  string line, name_field, value;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> name_field >> value;
      if (name_field == "MemTotal:") mem_total = stof(value);
      if (name_field == "MemFree:") mem_free = stof(value);
      if (name_field == "MemAvailable:") mem_available = stof(value);
      if (name_field == "Buffers:") buffers = stof(value);
    }
    memory_utilization = 1.0 - (mem_free / (mem_total - buffers));
  }
  return memory_utilization;
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() {
  long uptime;
  string line, value;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> value;
    uptime = stol(value);
  }
  return uptime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// Done: Process CPU utilization
float LinuxParser::CpuUtilization(int pid) {
  long SystemUptime = UpTime();
  float cpu_utilization, seconds, utime, stime, cutime, cstime, StartTime,
      TotalTime;
  float hertz = sysconf(_SC_CLK_TCK);
  string line, name_field, value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    int i = 0;
    while (linestream >> value) {
      i++;
      if (i == 14) utime = stof(value);
      if (i == 15) stime = stof(value);
      if (i == 16) cutime = stof(value);
      if (i == 17) cstime = stof(value);
      if (i == 22) StartTime = stof(value);
    }
  }
  TotalTime = utime + stime + cutime + cstime;
  seconds = SystemUptime - (StartTime / hertz);
  cpu_utilization = 100 * ((TotalTime / hertz) / seconds);
  return cpu_utilization;
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  int total_processes;
  string line, name_field, value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> name_field >> value;
      if (name_field == "processes") total_processes = stoi(value);
    }
  }
  return total_processes;
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  int running_processes;
  string line, name_field, value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> name_field >> value;
      if (name_field == "procs_running") running_processes = stoi(value);
    }
  }
  return running_processes;
}

// DONE: Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string command;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, command);
  }
  return command;
}

// DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  int ram;
  string line, Ram, field_name, value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> field_name >> value;
      if (field_name == "VmSize:") ram = stoi(value);
    }
  }
  ram = ram / 1000;
  Ram = to_string(ram);
  return Ram;
}

// DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string line, Uid, field_name, value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> field_name >> value;
      if (field_name == "Uid:") Uid = value;
    }
  }
  return Uid;
}

// Done: Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string uid = Uid(pid);
  string line, User, field_name, value;
  int i=0;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      if (line.find(uid) != string::npos) {
        User = line.substr(0, (line.find(':')));
      }
    }
    return User;
  }
}

// DONE: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  long uptime;
  string line, time, value;
  int i = 0;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);

    while (linestream >> value) {
      i++;
      if (i == 22) uptime = stol(value);
    }
  }
  uptime = uptime / sysconf(_SC_CLK_TCK);
  return uptime;
}
