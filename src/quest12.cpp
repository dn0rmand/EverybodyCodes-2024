#include "tools/macros.h"
#include "tools/readData.h"
#include "tools/timings.h"
#include <map>
#include <stdio.h>
#include <string.h>
#include <vector>

#define DAY "12"

typedef struct
{
    int score;
    int rank;
    int altitude;
} TRank;

static TRank missed = {.score = INT32_MAX, .altitude = 0};

typedef struct
{
    int x;
    int y;
    int hits;
} TPoint;

class Map
{
public:
    std::map<char, TPoint> segments;
    std::vector<TPoint> targets;

    static int getValue(char **ptr)
    {
        int value = 0;
        char *p = *ptr;
        while (*p && *p <= ' ')
        {
            p++;
        }

        if (!*p)
        {
            *ptr = p;
            return -1;
        }

        while (*p >= '0' && *p <= '9')
        {
            const char c = *p++;
            value = value * 10 + (c - '0');
        }
        *ptr = p;
        return value;
    }

    Map(int part)
    {
        char *data = (char *)tools::readData(DAY, part);
        if (part == 3)
        {
            TPoint ptA = {.x = 0, .y = 0};
            TPoint ptB = {.x = 0, .y = 1};
            TPoint ptC = {.x = 0, .y = 2};

            segments['C'] = ptC;
            segments['B'] = ptB;
            segments['A'] = ptA;

            char *ptr = data;
            while (*ptr)
            {
                int x = getValue(&ptr);
                if (x <= 0)
                {
                    break;
                }
                int y = getValue(&ptr);
                if (y <= 0)
                {
                    throw;
                }

                TPoint pt = {.x = x, .y = y, .hits = 1};
                targets.push_back(pt);
            }
        }
        else
        {
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
                        TPoint pt;
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
                        TPoint pt;
                        pt.x = i % w;
                        pt.y = h - 1 - (i - pt.x) / w;

                        segments[c] = pt;
                        break;
                    }
                }
            }
        }
        free(data);
    }
};

static unsigned int shoot(TPoint target, TPoint from, int coef)
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

static TRank shootStar(TPoint star, TPoint rocket, int coef, TRank best)
{
    int maxTime = (star.x + 1) / 2;
    int C = star.y - star.x;

    if ((rocket.x + C) == rocket.y)
    {
        int score = maxTime * coef;
        int altitude = maxTime + C;
        if (altitude > best.altitude || (altitude == best.altitude && score < best.score))
        {
            best = {.altitude = altitude, .rank = coef, .score = score};
        }
        return best;
    }

    int found = false;
    TRank localBest = missed;

    for (int f = maxTime; f > 0; f--)
    {
        TPoint from = rocket;
        TPoint target = star;

        target.x -= f;
        target.y -= f;
        from.x += f;
        from.y += f;

        int start = f;

        if ((target.x - f) > (from.x + f))
        {
            target.x -= f;
            target.y -= f;
            from.x += f;
            start += f;
        }

        for (int step = start;; step++)
        {
            target.x--;
            target.y--;
            from.x++;

            if (step >= (f + f))
            {
                from.y--;
            }
            if (target.x < from.x)
            {
                break;
            }

            if (target.y <= 0 || from.y < 0)
            {
                break;
            }

            if (from.y == from.x + C && step >= (f - 1)) // on the line
            {
                // just a matter of time offset
                if (target.x - from.x == target.y - from.y)
                {
                    target = from;
                }
            }

            if (target.y < best.altitude)
            {
                break;
            }

            if (from.x == target.x && from.y == target.y)
            {
                int score = f * coef;
                if (from.y > best.altitude || (from.y == best.altitude && score < best.score))
                {
                    best = {.altitude = from.y, .rank = coef, .score = score};
                }
                return best;
            }
        }
    }

    return best;
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

    for (TPoint &target : map.targets)
    {
        TRank best = missed;

        best = shootStar(target, map.segments['C'], 3, best);
        best = shootStar(target, map.segments['B'], 2, best);
        best = shootStar(target, map.segments['A'], 1, best);

        total += best.score;
    }

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
