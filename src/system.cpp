#include "system.h"

#include <unistd.h>

#include <cstddef>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

using namespace std;

// DONE: Return the system's CPU
Processor& System::Cpu() {
  cpu_.Utilization();
  return cpu_;
}

// DONE: Return a container composed of the system's processes
vector<Process>& System::Processes() {
  const vector<int>& pids = LinuxParser::Pids();
  for (const int& pid : pids) {
    processes_.emplace_back(pid);
  }
  return processes_;
}

// DONE: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// Done: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// DONE: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// Done: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// Done: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// Done: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }