#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <sstream>
#include <string>
#include <vector>

#include "iostream"

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
  string os, kernel, version;
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

//Read and return the system memory utilization
float LinuxParser::MemoryUtilization()
{
  string line, key;
  float total_memory, free_memory;
  // directory to dins RAM is  defined in header.
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  while (std::getline(filestream, line)) {
    // iterate line per line and look for total or free mem.
    std::istringstream current_line(line);
    while (current_line >> key) {
      if (key == "MemTotal:")
      {
        current_line >> total_memory;
      } else if (key == "MemFree:")
      {
        current_line >> free_memory;
      }
    }
  }
  // return percentage of the ram that has been used.
  return (total_memory - free_memory) / total_memory;
}

//Read and return the system uptime
long LinuxParser::UpTime()
{
  string line, key;
  long uptime{};
  // cat /proc/stat to find procs running, defined in header.
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  while (std::getline(filestream, line))
  {
    // iterate line per line and look for total or free mem.
    std::istringstream current_line(line);
    current_line >> uptime;
  }
  return uptime;
}

// Not Required: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// Not Required: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }

// Not Required: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// Not Required: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization()
{
  // returns all the values from the cpu line in /proc/stat
  vector<string> cpuUtilization{};
  string line, cpu;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open())
  {
    // Read first line from file which should contain CPU stats
    getline(filestream, line);
    std::istringstream lineStream(line);
    // skip cpu string
    lineStream >> cpu;
    // add each value as a string to the verctor
    while (lineStream >> cpu)
    {
      cpuUtilization.push_back(cpu);
    }
  }
  return cpuUtilization;
}
// returns all the values from the cpu line in /proc/stat
vector<string> LinuxParser::CpuUtilization(int pid)
{
  
  vector<string> cpuUtilization{}; 
  string line, cpu;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open())
  {
    // read utime and stime fields.
    getline(filestream, line);
    std::istringstream lineStream(line);
    // skip cpu string
    lineStream >> cpu;
    // add each value as a string to the verctor
    while (lineStream >> cpu)
    {
      cpuUtilization.push_back(cpu);
    }
  }
  return cpuUtilization;
}
// Read and return the total number of processes
int LinuxParser::TotalProcesses()
{
  string line, key;
  int processes_total{};
  // cat /proc/stat to find procs running, defined in header.
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open())
  {
    while (std::getline(filestream, line))
    {
      // iterate line per line and look for total or free mem.
      std::istringstream current_line(line);
      while (current_line >> key) {
        if (key == "processes") {
          current_line >> processes_total;
        }
      }
    }
  }
  return processes_total;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses()
{
  string line, key;
  int processes_running{};  
  std::ifstream filestream(kProcDirectory + kStatFilename); // cat /proc/stat to find procs running, defined in header.
  while (std::getline(filestream, line))  // iterate line per line and look for total or free mem.
  {   
    std::istringstream current_line(line);
    while (current_line >> key) {
      if (key == "procs_running")
      {
        current_line >> processes_running;
      }
    }
  }
  return processes_running;
}

//Read and return the command associated with a process
string LinuxParser::Command(int pid)
{
  string command;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (stream.is_open())
  {
    std::getline(stream, command);
  }
  return command;
}
//Read and return the memory used by a process
string LinuxParser::Ram(int pid)
{
  string line, key;
  int memmory;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);

  while (std::getline(stream, line))
  {
    std::istringstream linestream(line);
    linestream >> key;
    if (key == "VmSize:")
    {
      linestream >> memmory;
      return std::to_string(memmory / 1024); //to MB
    }
  }
  return "N/A";
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid)
{ 
  string line, uid;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  //line looks like: Uid:	246837155	246837155	246837155	246837155
  if (stream.is_open())
  {
    while (getline(stream, line))
    {
      std::istringstream linestream(line);
      string key;
      linestream >> key;
      if (key == "Uid:") //found the Uid part, return its keypair.
      {
        linestream >> uid;
        return uid;
      }
    }
  } 
  return "";
  
}






// Read and return the user associated with a process
string LinuxParser::User(int pid)
{
  string line, user, x;
  std::ifstream stream(kPasswordPath);

  //layout of file: root:x:0:0:root:/root:/bin/bash
  if (stream.is_open()) {
    while (getline(stream, line))
    {
      std::istringstream linestream(line);
      std::getline(linestream, user, ':'); //root
      std::getline(linestream, x, ':');  //x
      std::getline(linestream, x, ':'); //0
      std::getline(linestream, x, ':'); //uid
      if (x == LinuxParser::Uid(pid))
      {
        return user;
      }
    }
  }
      
  return "me??"; //some uid's dont show up in my /etc/passwd file.
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid)
{
  std::string line;
  long uptime = 0;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);

  if (stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    std::istream_iterator<string> beg(linestream), end;
    vector<string> values(beg, end);

    long starttime = stol(values[21]);
    uptime = LinuxParser::UpTime() - starttime / sysconf(_SC_CLK_TCK); //clocks per tick, my cpu = 100.
  }
  return uptime;
}
