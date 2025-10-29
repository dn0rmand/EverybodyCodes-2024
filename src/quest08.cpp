#include <stdio.h>
#include <string.h>
#include <vector>
#include "tools/readData.h"
#include "tools/timings.h"

#define DAY "08"

static unsigned int part1()
{
    int blocks = 4099331;
    int total = 1;
    int base = 1;

    while(total < blocks) {
        base += 2;
        total += base;
    }

    int extra = total - blocks;

    return extra * base;
}

static unsigned int part2()
{
    int blocks = 20240000;
    int priests = 938;
    int acolytes = 1111;
    int thickness = 1;
    int total = 1;
    int base = 1;

    while(total < blocks) {
        base += 2;
        thickness = (thickness * priests) % acolytes;
        total += base*thickness;
    }

    int extra = total - blocks;

    return extra * base;
}

static unsigned int part3()
{
    long priests = 943657;
    long blocks = 202400000;
    long acolytes = 10;
    long thickness = 1;
    long total = 1;
    long base = 1;

    std::vector<long> columns;
    columns.push_back(1);

    long best = blocks;

    while(true) {
        base += 2;
        thickness = acolytes + (thickness * priests) % acolytes;
        total += base*thickness;
        columns.push_back(0); 
        // New column sizes
        for(int i = 0; i < columns.size(); i++) {
            columns[i] += thickness;
        }
        // Calculate holes
        if (total >= blocks) {
            long remove = 0;
            for(int i = 0; i < columns.size()-1; i++) {
                long empty = (priests * base * columns[i]) % acolytes;
                if (i > 0) {
                    remove += empty+empty;
                } else {
                    remove += empty;
                }
            }
            if ((total-remove) > blocks) {
                long extra = (total - remove) - blocks;
                if (extra < best) {
                    best = extra;
                } else {
                    break;
                }
            }
        }
    }

    return best;
}

void quest08()
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

    quest08();
}
