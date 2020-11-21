#include "processor.h"
#include "linux_parser.h"
// TODO: Return the aggregate CPU utilization
// ref: anshulpatel.in/post/linux_cpu_percentage "DONE"
float Processor::Utilization() { return (LinuxParser::ActiveJiffies()/LinuxParser::Jiffies());}