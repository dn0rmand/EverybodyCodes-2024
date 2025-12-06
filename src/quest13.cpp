#include "tools/readData.h"
#include "tools/timings.h"
#include <queue>
#include <stdio.h>
#include <string.h>
#include <vector>

#define DAY "13"

#define abs(x) ((x) < 0 ? -(x) : (x))
#define min(x, y) ((x) < (y) ? (x) : (y))

class Point
{
public:
    Point(int x, int y)
    {
        this->x = x;
        this->y = y;
    }

    int x;
    int y;
};

class Map
{
private:
    const char *data;

public:
    int width;
    int height;

public:
    Map(int part)
    {
        data = (const char *)tools::readData(DAY, part);
        width = (int)tools::stringLength(data);
        height = (int)(strlen(data) + 1) / (width + 1);
    }

    ~Map()
    {
        free((void *)data);
    }

    const char get(int x, int y)
    {
        if (x < 0 || y < 0 || x >= width || y >= height)
        {
            return '#';
        }
        int index = x + y * (width + 1);
        return data[index];
    }
};

class State
{
public:
    int x;
    int y;
    int level;
    int time;

public:
    State(int x, int y, int level, int time)
    {
        this->x = x;
        this->y = y;
        this->level = level;
        this->time = time;
    }
};

static unsigned int findShortestPath(int part)
{
    Map map(part);

    std::queue<State> states;

    for (int y = 0; y < map.height; y++)
    {
        for (int x = 0; x < map.width; x++)
        {
            if (map.get(x, y) == 'S')
            {
                states.push(State(x, y, 0, 0));
            }
        }
    }

    int visited[map.width * map.height];
    int veryBest = -1;

    for (int k = 0; k < map.width * map.height; k++)
    {
        visited[k] = 0;
    }

    while (!states.empty())
    {
        State state = states.front();
        states.pop();
        int x = state.x;
        int y = state.y;
        for (Point pt : {Point(x - 1, y), Point(x + 1, y), Point(x, y - 1), Point(x, y + 1)})
        {
            const char v = map.get(pt.x, pt.y);
            if (v == '#' || v == 'S')
            {
                continue;
            }
            int level = v == 'E' ? 0 : (v - '0');
            int time = 0;

            if (level != state.level)
            {
                int t1 = abs(state.level - level);
                int t2 = abs(10 + state.level - level);
                int t3 = abs(state.level - 10 - level);
                time = min(min(t1, t2), t3);
            }
            time += 1 + state.time;
            int best = visited[pt.x + map.width * pt.y];
            if (best == 0 || time < best)
            {
                visited[pt.x + map.width * pt.y] = time;
                if (v != 'E')
                {
                    State newState(pt.x, pt.y, level, time);
                    states.push(newState);
                }
                else
                {
                    veryBest = time;
                }
            }
        }
    }

    return veryBest;
}

static unsigned int part1()
{
    return findShortestPath(1);
}

static unsigned int part2()
{
    return findShortestPath(2);
}

static unsigned int part3()
{
    return findShortestPath(3);
}

void quest13()
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

    quest13();
}
