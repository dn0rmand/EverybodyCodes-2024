#include <stdio.h>
#include <string.h>
#include <vector>
#include <map>
#include "tools/readData.h"
#include "tools/timings.h"

#define DAY "09"

static void loadData(std::vector<int> *sparkballs, int part)
{
    void *data = tools::readData(DAY, part);
    char *ptr = (char *)data;
    int value = 0;

    while (*ptr)
    {
        if (*ptr >= '0' && *ptr <= '9')
        {
            value = value * 10 + *ptr - '0';
        }
        else
        {
            if (value > 0)
            {
                sparkballs->push_back(value);
            }
            value = 0;
        }

        ptr++;
    }
    if (value > 0)
    {
        sparkballs->push_back(value);
    }
    free(data);
}

static std::map<int, int> memoize;

static int getCount(int value, int modulos[])
{
    if (value == 0) {
        return 0;
    }
    if (memoize[value] != 0) {
        return memoize[value];
    }

    int best = value;
    for(int i = 0; modulos[i]; i++)
    {
        int modulo = modulos[i];
        if (modulo <= value) {
            int c = 1 + getCount(value - modulo, modulos);
            if (c < best) {
                best = c;
            }
        }
    }
    memoize[value] = best;
    return best;
}

static int getDoubleCount(int value, int modulos[])
{
    int best = value;

    int left = value / 2;
    int right= value-left;

    while(right-left <= 100)
    {
        int c1 = getCount(left, modulos);
        int c2 = getCount(right, modulos);
        int count = c1+c2;
        if (count < best) {
            best = count;
        }
        right++;
        left--;
    }
    return best;
}

static unsigned int part1()
{
    std::vector<int> sparkballs;

    loadData(&sparkballs, 1);

    unsigned int total = 0;
    int modulos[] = { 10, 5, 3, 1, 0 };

    memoize.clear();
    for(int value: sparkballs)
    {
        total += getCount(value, modulos);
    }
    return total;
}

static unsigned int part2()
{
    std::vector<int> sparkballs;

    loadData(&sparkballs, 2);

    unsigned int total = 0;
    int modulos[] = { 30, 25, 24, 20, 16, 15, 10, 5, 3, 1, 0 };

    memoize.clear();
    for(int value: sparkballs)
    {
        total += getCount(value, modulos);
    }
    return total;
}

static unsigned int part3()
{
    std::vector<int> sparkballs;

    loadData(&sparkballs, 3);

    unsigned int total = 0;
    int modulos[] = { 101, 100, 75, 74, 50, 49, 38, 37, 30, 25, 24, 20, 16, 15, 10, 5, 3, 1, 0 };

    memoize.clear();
    for(int value: sparkballs)
    {
        total += getDoubleCount(value, modulos);
    }
    return total;
}

void quest09()
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

    quest09();
}
