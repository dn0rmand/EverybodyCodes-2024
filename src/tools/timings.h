#ifndef TIMINGS_H
#define TIMINGS_H

#include <stdio.h>
#include <time.h>

class Timing {
private:
  clock_t start;
  clock_t mainStart;

public:
  ~Timing();

  Timing();

  void log(const char *message);
};

#endif
