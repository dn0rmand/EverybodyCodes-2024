#include "tools/macros.h"
#include "tools/map.h"
#include "tools/readData.h"
#include "tools/timings.h"
#include <set>
#include <stdio.h>
#include <string.h>
#include <vector>

#define DAY "15"
#define QUEST_NAME quest##DAY

#define MAX_WIDTH 300L

typedef struct
{
    int x;
    int y;
    unsigned int herbs;
} TState;

static unsigned int getHerbValue(const char c)
{
    if (c >= 'A' && c <= 'Z')
    {
        int i = (int)(c - 'A');
        return 1 << i;
    }
    else
    {
        return 0;
    }
}

static unsigned int getTargetValue(tools::Map *map)
{
    unsigned int target = 0;

    for (int x = 0; x < map->width; x++)
        for (int y = 0; y < map->height; y++)
        {
            auto c = map->get(x, y);
            target |= getHerbValue(c);
        }

    return target;
}

static u_long getKey(TState pt)
{
    u_long key = pt.herbs;
    key = key * MAX_WIDTH + pt.y;
    key = key * MAX_WIDTH + pt.x;
    return key;
}

static std::vector<TState> moves({{.x = 0, .y = -1},
                                  {.x = 0, .y = 1},
                                  {.x = -1, .y = 0},
                                  {.x = 1, .y = 0}});

static int getEntrance(tools::Map *map)
{
    for (int x = 0; x < map->width; x++)
    {
        if (map->get(x, 0) != '#')
        {
            return x;
        }
    }

    return -1;
}

static int shortestPath(tools::Map *map, TState start, unsigned int target)
{
    std::vector<TState> info[2];
    std::vector<TState> *states = &(info[0]);
    std::vector<TState> *newStates = &(info[1]);
    std::set<u_long> visited;

    start.herbs = 0;
    states->push_back(start);
    visited.insert(getKey(start));

    int steps = 0;
    while (states->size() > 0)
    {
        steps++;
        newStates->clear();

        for (TState pt : *states)
        {
            for (auto move : moves)
            {
                TState newPt = {.x = pt.x + move.x, .y = pt.y + move.y, .herbs = pt.herbs};
                char c = map->get(newPt.x, newPt.y);
                if (c == '#' || c == '~')
                {
                    continue;
                }
                newPt.herbs |= getHerbValue(c);
                int key = getKey(newPt);
                if (visited.find(key) != visited.end())
                {
                    continue;
                }

                visited.insert(key);
                newStates->push_back(newPt);

                if (newPt.herbs == target)
                {
                    if (newPt.y == 0)
                    {
                        return steps;
                    }
                }
            }
        }

        SWAP(states, newStates);
    }

    return -1;
}

static unsigned int part1()
{
    tools::Map map(DAY, 1);

    int entrance = getEntrance(&map);
    int target = getTargetValue(&map);
    int total = shortestPath(&map, {.x = entrance, .y = 0, .herbs = 0}, getHerbValue('H'));

    return total;
}

static unsigned int part2()
{
    tools::Map map(DAY, 2);

    int entrance = getEntrance(&map);
    int target = getTargetValue(&map);
    int total = shortestPath(&map, {.x = entrance, .y = 0, .herbs = 0}, target);

    return total;
}

static unsigned int part3()
{
    tools::Map map(DAY, 3);

    int entrance = getEntrance(&map);
    auto target = getTargetValue(&map);
    int total = shortestPath(&map, {.x = entrance, .y = 0, .herbs = 0}, target);

    return total;
}

void QUEST_NAME()
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

    QUEST_NAME();
}
