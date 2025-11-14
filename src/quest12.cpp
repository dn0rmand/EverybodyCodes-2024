#include "tools/readData.h"
#include "tools/timings.h"
#include <map>
#include <stdio.h>
#include <string.h>
#include <vector>

#define DAY "12"

struct Point
{
    int x;
    int y;
    int hits;
};

class Map
{
public:
    std::map<char, struct Point> segments;
    std::vector<struct Point> targets;

    Map(int part)
    {
        char *data = (char *)tools::readData(DAY, part);
        int w = tools::stringLength(data) + 1;
        int h = (strlen(data) + 1) / w;

        for (int i = 0; data[i]; i++)
        {
            char c = data[i];
            switch (c)
            {
                case 'T':
                case 'H':
                {
                    struct Point pt;
                    pt.x = i % w;
                    pt.y = h - 1 - (i - pt.x) / w;
                    pt.hits = c == 'T' ? 1 : 2;
                    targets.push_back(pt);
                    break;
                }
                case '.':
                case 0x0A:
                case '=':
                    break;
                default:
                {
                    struct Point pt;
                    pt.x = i % w;
                    pt.y = h - 1 - (i - pt.x) / w;

                    segments[c] = pt;
                    break;
                }
            }
        }
        free(data);
    }
};

static unsigned int shoot(struct Point target, struct Point from, int coef)
{
    int distance = target.x - from.x;
    if (distance < 1)
    {
        throw;
    }

    distance -= from.y - target.y;

    if (distance % 3 == 0)
    {
        return coef * (distance / 3) * target.hits;
    }
    return 0;
}

static unsigned int shootThemAll(int part)
{
    Map map(part);

    unsigned int total = 0;

    for (auto target : map.targets)
    {
        unsigned int v = shoot(target, map.segments['C'], 3);
        if (v == 0)
        {
            v = shoot(target, map.segments['B'], 2);
        }
        if (v == 0)
        {
            v = shoot(target, map.segments['A'], 1);
        }
        if (v == 0)
        {
            throw;
        }

        total += v;
    }

    return total;
}

static unsigned int part1()
{
    return shootThemAll(1);
}

static unsigned int part2()
{
    return shootThemAll(2);
}

static unsigned int part3()
{
    Map map(3);

    unsigned int total = 0;

    return total;
}

void quest12()
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

    quest12();
}
