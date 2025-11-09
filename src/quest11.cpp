#include <stdio.h>
#include <string.h>
#include <map>
#include "tools/readData.h"
#include "tools/timings.h"

#define DAY "11"

#define MAX_TOKEN 100

class Mapping
{
public:
    int A;
    int Z;

private:
    std::map<int, int> converter;

    int convert(int token)
    {
        if (converter.find(token) == converter.end())
        {
            int v = converter.size();
            converter[token] = v;
            if (token == 0)
            {
                A = v;
            }
            else if (token == 25)
            {
                Z = v;
            }
            return v;
        }
        else
        {
            return converter[token];
        }
    }

public:
    int mapping[MAX_TOKEN][MAX_TOKEN];

    Mapping()
    {
        memset(mapping, 0, MAX_TOKEN * MAX_TOKEN * sizeof(int));
    }

    void add(int from, int to)
    {
        from = convert(from);
        to = convert(to);

        if (to >= MAX_TOKEN || from >= MAX_TOKEN)
        {
            throw;
        }

        mapping[from][to]++;
    }

    int tokenCount()
    {
        return converter.size();
    }
};

static int getToken(char **ptr)
{
    int token = 0;
    char *p = *ptr;
    while (*p >= 'A' && *p <= 'Z')
    {
        const char c = *p++;
        token = token * 26 + (c - 'A');
    }
    *ptr = p;
    return token;
}

static Mapping parseInput(int part)
{
    Mapping mapping;

    void *data = tools::readData(DAY, part);
    char *ptr = (char *)data;

    while (*ptr)
    {
        int from = getToken(&ptr);
        if (*ptr++ != ':')
        {
            throw;
        }

        while (*ptr && *ptr != 0x0A)
        {
            int to = getToken(&ptr);
            if (*ptr == ',')
            {
                ptr++;
            }
            mapping.add(from, to);
        }

        if (*ptr)
        {
            ptr++;
        }
    }

    free(data);

    return mapping;
}

static unsigned long process(Mapping mapping, int start, int days)
{
    std::map<int, unsigned long> state1;
    std::map<int, unsigned long> state2;

    auto state = &state1;
    auto newState = &state2;

    (*state)[start] = 1;

    unsigned long total = 0;

    for (int day = 0; day < days; day++)
    {
        newState->clear();
        total = 0;
        for (auto e : *state)
        {
            int from = e.first;
            unsigned long fromCount = e.second;

            auto x = mapping.mapping[from];
            for (int to = 0; to < mapping.tokenCount(); to++)
            {
                unsigned long toCount = x[to] * fromCount;
                if (toCount > 0)
                {
                    (*newState)[to] += toCount;
                    total += toCount;
                }
            }
        }

        auto old = state;
        state = newState;
        newState = old;

        if (total == 0)
        {
            break;
        }
    }

    return total;
}

static unsigned long part1()
{
    Mapping mapping = parseInput(1);

    return process(mapping, mapping.A, 4);
}

static unsigned long part2()
{
    Mapping mapping = parseInput(2);

    return process(mapping, mapping.Z, 10);
}

static unsigned long part3()
{
    Mapping mapping = parseInput(3);

    unsigned long max = 0;
    unsigned long min = 0;

    for (int start = 0; start < mapping.tokenCount(); start++)
    {
        unsigned long count = process(mapping, start, 20);
        if (count == 0)
        {
            continue;
        }
        if (min == 0)
        {
            min = count;
            max = count;
        }
        else if (count < min)
        {
            min = count;
        }
        else if (count > max)
        {
            max = count;
        }
    }

    return max - min;
}

void quest11()
{
    printf("QUEST %s\n", DAY);

    unsigned long d1 = part1();
    printf("\tPART 1 = %lu\n", d1);

    unsigned long d2 = part2();
    printf("\tPART 2 = %lu\n", d2);

    unsigned long d3 = part3();
    printf("\tPART 3 = %lu\n", d3);
}

int main(int count, char **args)
{
    Timing _t;

    quest11();
}
