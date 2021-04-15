#include "format.h"

#include <string>

using std::string;

#define ONE_HOUR 3600
#define ONE_MINUTE 60


string Format::ElapsedTime(long seconds_) {
  long seconds = seconds_;
  long hour, minute;

  hour = seconds/ONE_HOUR;
  seconds -= hour * ONE_HOUR;
  minute = seconds/ONE_MINUTE;
  seconds -= minute * ONE_MINUTE;

  return (std::to_string(hour)+ ":"+ std::to_string(minute) + ":" +std::to_string(seconds));
}