#include <string>

#include "format.h"

using std::string;
using std::to_string;
// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  string output = {""};
// test 30000 sec = 8.3 
  long hours = seconds / 3600;
  seconds = seconds - (hours * 3600);
// 30000-28800 = 1200 = 20min now sec is 0
  long minutes = seconds / 60;
  seconds = seconds - (minutes * 60);
// check if Hours two digits
  if (hours > 99) {
    output = output + "99";
  } else { // check if hours one ungsigned digit only to make the format 01 to 09 hours
    if ((hours > 0) && (hours < 10))
      output = output + "0";
    output = output +  to_string(hours);
  } // add the splitter 
  output = output + ":";
  // check if the minutes one digit otherwise its two!
  if ((minutes > 0) && (minutes < 10)) 
    output = output + "0";
  
  output = output + to_string(minutes);
  output = output + ":";

  if ((seconds > 0) && (seconds < 10)) 
    output = output + "0";
  output = output + to_string(seconds);
  return output;
}