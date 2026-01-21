#include "./timings.h"

Timing::~Timing() {
  clock_t end = clock();

  int msec = (end - this->mainStart) * 1000 / CLOCKS_PER_SEC;

  printf("Executed in %d seconds %d milliseconds\n", msec / 1000, msec % 1000);
}

Timing::Timing() {
  this->start = clock();
  this->mainStart = this->start;
}

void Timing::log(const char *message) {
  clock_t end = clock();

  int msec = (end - this->start) * 1000 / CLOCKS_PER_SEC;

  printf("%s executed in %d seconds %d milliseconds\n", message, msec / 1000,
         msec % 1000);

  this->start = clock(); // reset the start time
}