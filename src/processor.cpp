#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    long activeJeff = LinuxParser::ActiveJiffies();
    long idleJeff = LinuxParser::IdleJiffies();

    long activeDuration = activeJeff - prevActiveJeff;
    long idleDuration   = idleJeff - prevIdleJeff;
    long totalDuration = activeDuration + idleDuration;

    float processorUtiliztion = static_cast<float>(activeDuration) / totalDuration;

    prevActiveJeff = activeJeff;
    prevIdleJeff = idleJeff;

    return processorUtiliztion;
}