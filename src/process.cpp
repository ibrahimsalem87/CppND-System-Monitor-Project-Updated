#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid_) : pid(pid_) {}

// DONE: Return this process's ID
int Process::Pid() { return this->pid; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return LinuxParser::CpuUtilization(Pid()); }

//DONE: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(Pid()); }

// DONE: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(Pid()); }

// DONE: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(Pid()); }

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(Pid()); }

// Done?
bool Process::operator<(Process const& a) const {
  return a.pid < this->pid ? a.pid : this->pid;
}