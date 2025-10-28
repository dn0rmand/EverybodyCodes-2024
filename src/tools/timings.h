#ifndef TIMINGS_H
#define TIMINGS_H

#include <time.h>
#include <stdio.h>

class Timing
{
    private:
        clock_t start;

    public:
        ~Timing()
        {
            clock_t end = clock();

            int msec = (end-this->start) * 1000 / CLOCKS_PER_SEC;

            printf("Executed in %d seconds %d milliseconds\n", msec/1000, msec%1000);
        }

        Timing()
        {
            this->start = clock();
        }      
};

#endif
