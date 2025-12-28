#include "tools/quickSearch.h"
#include "tools/quickSort.h"
#include "tools/readData.h"
#include <cstdlib>
#include <stdio.h>

#define MAX_ROW 1000

static const char *quest01_data;

static char *cursor;
static char *input1;
static char *input2;
static char *input3;

static char *readLine()
{
    if (!*cursor)
    {
        return cursor; // empty Line???
    }

    while (*cursor && (*cursor <= ' ' || *cursor == 'Z'))
    {
        cursor++;
    }

    char *line = cursor;
    while (*cursor > ' ' && *cursor != 'Z')
    {
        cursor++;
    }

    if (*cursor == 'Z')
    {
        cursor++;
    }

    return line;
}

static void loadData()
{
    quest01_data = (const char *)tools::readData("01", 1);
    cursor = (char *)quest01_data;

    input1 = readLine();
    input2 = readLine();
    input3 = readLine();
}

static unsigned int part1()
{
    unsigned int sum = 0;

    for (char *ptr = input1; *ptr != 'Z'; ptr++)
    {
        switch (*ptr)
        {
            case 'A':
                break;
            case 'B':
                sum++;
                break;
            case 'C':
                sum += 3;
                break;
        }
    }

    return sum;
}

static unsigned int part2()
{
    unsigned int sum = 0;

    for (char *ptr = input2; *ptr != 'Z'; ptr += 2)
    {
        unsigned int subTotal = 0;
        unsigned int size = 2;

        for (int i = 0; i < 2; i++)
        {
            switch (ptr[i])
            {
                case 'X':
                    size = 0;
                    break;
                case 'A':
                    break;
                case 'B':
                    subTotal += 1;
                    break;
                case 'C':
                    subTotal += 3;
                    break;
                case 'D':
                    subTotal += 5;
                    break;
            }
        }
        sum += size + subTotal;
    }

    return sum;
}

static unsigned int part3()
{
    unsigned int sum = 0;

    for (char *ptr = input3; *ptr != 'Z'; ptr += 3)
    {
        unsigned int subTotal = 0;
        unsigned int size = 3;

        for (int i = 0; i < 3; i++)
        {
            switch (ptr[i])
            {
                case 'X':
                    size -= 1;
                    break;
                case 'A':
                    break;
                case 'B':
                    subTotal += 1;
                    break;
                case 'C':
                    subTotal += 3;
                    break;
                case 'D':
                    subTotal += 5;
                    break;
            }
        }

        subTotal += size == 2 ? 2 : (size == 3 ? 6 : 0);

        sum += subTotal;
    }

    return sum;
}

void quest01()
{
    loadData();

    printf("QUEST 1\n");

    unsigned int d1 = part1();
    printf("\tPART 1 = %u\n", d1);

    unsigned int d2 = part2();
    printf("\tPART 2 = %u\n", d2);

    unsigned int d3 = part3();
    printf("\tPART 3 = %u\n", d3);

    free((void *)quest01_data);
}

int main(int count, char **args)
{
    quest01();
}
