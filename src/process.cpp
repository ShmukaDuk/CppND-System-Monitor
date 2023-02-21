#include "process.h"

#include <unistd.h>

#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
using std::string;
using std::to_string;
using std::vector;

// init this process by parsing in it's const pid.
Process::Process(int pid) : pid_(pid) {}

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
// float Process::CpuUtilization() const { return LinuxParser::CpuUtilization(pid_); }

float Process::CpuUtilization() const
{
  int pidticks{};
  std::vector<std::string> pidutilization = LinuxParser::CpuUtilization(pid_);
  int it{};
  for (auto proc : pidutilization)
  {
    if ((it == 14) | (it == 13) | (it == 15) | (it == 12)) {
      pidticks += stoi(proc);
    }
    it++;
  }
  float pidtime = pidticks / (float)sysconf(_SC_CLK_TCK);
  long pidlifetime = LinuxParser::UpTime(pid_);
  float pidpercent = pidtime / static_cast<float>(pidlifetime);
  pidpercent = pidlifetime != 0 ? (pidpercent/(float)pidlifetime) : 0.0;
//   std::cout << "Pid Time:" << pidtime << " PID lifetime " << pidlifetime << " pidppercent " << pidpercent << std::endl;
  return pidpercent;
}

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

//Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

//Overload the "less than" comparison operator for Process objects

bool Process::operator<(Process const& a) const {
  return a.CpuUtilization() < CpuUtilization();
}