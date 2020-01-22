#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // Done: See src/processor.cpp
  float AggrateUtilization();   // Overall utilization of CPU
  // Done: Declare any necessary private members
 private:
    long prevActiveJeff{0};
    long prevIdleJeff{0};
};

#endif