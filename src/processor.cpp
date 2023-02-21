#include "processor.h"

#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "linux_parser.h"

// Return the aggregate CPU utilization
float Processor::Utilization() {
  long cpu_info_0 = 0;
 
  std::vector<std::string> cpuutilization0 = LinuxParser::CpuUtilization();  //  read the first line of   /proc/stat
  for (auto cpu : cpuutilization0)
  {
    cpu_info_0 += std::stol(cpu);   // sum all of the times found on that first line to get the total time
  }  

  float percent = 1.0 - float((std::stol(cpuutilization0[LinuxParser::kIdle_])) / float(cpu_info_0)); // divide the fourth column ("idle")
  return percent;

  // return LinuxParser::CpuUtilization();
}