#include "format.h"

#include <string>

using std::string;

string Format::ElapsedTime(long seconds) {
  long hours = seconds / 3600;
  seconds %= 3600;
  long minutes = seconds / 60;
  seconds %= 60;
  std::string formatted_time = std::to_string(hours) + ":" +
                               std::to_string(minutes) + ":" +
                               std::to_string(seconds);

  return formatted_time;
}
