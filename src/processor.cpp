#include "processor.h"

#include <chrono>
#include <iostream>
#include <thread>

using std::string;
using namespace LinuxParser;
using std::stod;

// Done: Return the aggregate CPU utilization
double Processor::Utilization() {
  double utilization;
  string line, name_field, value;
  std::vector<double>
      CpuData;  //[0]user, [1]nice, [2]system, [3]idle, [4]iowait, [5]irq,
                // [6]softirq, [7]steal, [8]guest, [9]guest_nice;
  std::vector<double> PreviousCpuData;

 

  std::ifstream Previousfilestream(kProcDirectory + kStatFilename);
  if (Previousfilestream.is_open()) {
    std::getline(Previousfilestream, line);
    std::istringstream Previousfilestream(line);
    Previousfilestream >> name_field;
    if (name_field == "cpu") {
      while (Previousfilestream >> value) {
        PreviousCpuData.push_back(stod(value));
      }
    }
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> name_field;
    if (name_field == "cpu") {
      while (linestream >> value) {
        CpuData.push_back(stod(value));
      }
    }
  }

  double PrevIdle =
      PreviousCpuData[3] + PreviousCpuData[4];  // previdle + previowait
  double Idle = CpuData[3] + CpuData[4];         // idle + iowait

  double PrevNonIdle = PreviousCpuData[0] + PreviousCpuData[1] +
                      PreviousCpuData[2] + PreviousCpuData[5] +
                      PreviousCpuData[6] +
                      PreviousCpuData[7];  // prevuser + prevnice + prevsystem +
                                           // previrq + prevsoftirq + prevsteal

  double NonIdle = CpuData[0] + CpuData[1] + CpuData[2] + CpuData[5] +
                  CpuData[6] +
                  CpuData[7];  // user + nice + system + irq + softirq + steal

  double PrevTotal = PrevIdle + PrevNonIdle;
  double Total = Idle + NonIdle;

  // differentiate: actual value minus the previous one
  double totald = Total - PrevTotal;
  double idled = Idle - PrevIdle;

  utilization = (totald - idled) / totald;

  return utilization;
}