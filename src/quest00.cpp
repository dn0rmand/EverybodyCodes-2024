#include <stdio.h>
#include <string.h>
#include "tools/readData.h"
#include "tools/timings.h"

#define DAY "00"

static unsigned int part1()
{
    void* data = tools::readData(DAY, 1);

    unsigned int total = 0;

    free(data);

    return total;
}

static unsigned int part2()
{
    void* data = tools::readData(DAY, 2);

    unsigned int total = 0;

    free(data);

    return total;
}

static unsigned int part3()
{
    void* data = tools::readData(DAY, 3);

    unsigned int total = 0;

    free(data);

    return total;
}

void quest00()
{
    printf("QUEST %s\n", DAY);

    unsigned int d1 = part1();
    printf("\tPART 1 = %u\n", d1);

    unsigned int d2 = part2();
    printf("\tPART 2 = %u\n", d2);

    unsigned int d3 = part3();
    printf("\tPART 3 = %u\n", d3);
}

int main(int count, char **args)
{
    Timing _t;

    quest00();
}
