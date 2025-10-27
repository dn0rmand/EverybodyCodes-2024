#include <stdio.h>
#include <string.h>
#include <map>
#include <list>
#include "tools/readData.h"
#include "tools/timings.h"

#define DAY "06"

static int appleId;

static char *parseLine(char *ptr, std::map<int, int> &tree)
{
    int parent = 0;

    while (*ptr && *ptr >= 'A' && *ptr <= 'Z')
    {
        parent = parent * 100 + (*ptr - 'A' + 1);
        ptr++;
    }

    if (parent == 0)
    {
        throw;
    }

    if (*ptr != ':')
    {
        throw;
    }

    while (*ptr == ':' || *ptr == ',')
    {
        ptr++;
        if (*ptr == '@')
        {
            int id = appleId++;
            tree[-id] = parent;
            ptr++;
        }
        else if (*ptr >= 'A' && *ptr <= 'Z')
        {
            int child = 0;
            while (*ptr >= 'A' && *ptr <= 'Z')
            {
                child = child * 100 + (*ptr - 'A' + 1);
                ptr++;
            }
            tree[child] = parent;
        }
        else
        {
            throw;
        }
    }
    if (*ptr == 0x0A)
    {
        ptr++;
    }
    else if (*ptr)
    {
        throw;
    }
    return ptr;
}

static void loadTree(std::map<int, int> &tree, int part)
{
    void *data = tools::readData(DAY, part);
    char *ptr = (char *)data;

    appleId = 1;
    while (*ptr)
    {
        ptr = parseLine(ptr, tree);
    }
    free(data);
}

static char *expand(char *path, int value)
{
    if (value == 0)
    {
        return path;
    }
    int letter = value % 100;
    path = expand(path, (value - letter) / 100);
    *path++ = letter + 'A' - 1;
    return path;
}

static const char *pathName(std::list<int> *path, bool oneChar)
{
    static char data[512];

    char *name = &data[0];

    for (int id : *path)
    {
        char *ptr = expand(name, id);
        if (oneChar)
        {
            name++;
        }
        else
        {
            name = ptr;
        }
    }
    *name++ = '@';
    *name = 0;
    return data;
}

static const char *process(int part)
{
    std::map<int, int> tree;
    std::map<int, int> sizes;

    loadTree(tree, part);

    std::list<int> paths[appleId];
    int lastId = 0;
    for (int id = 1;; id++)
    {
        lastId = id;
        int parent = tree[-id];
        if (parent == 0)
        {
            break;
        }
        std::list<int> *path = &paths[id];
        std::map<int, bool> parents;

        while (parent != 0)
        {
            path->push_front(parent);
            parent = tree[parent];
            if (parents[parent])
            {
                path = NULL;
                break;
            }
            parents[parent] = true;
        }
        if (path != NULL)
        {
            int s = path->size();
            sizes[s] = sizes[s] + 1;
        }
    }

    for (int id = 1; id <= lastId; id++)
    {
        int s = paths[id].size();
        if (sizes[s] == 1)
        {
            return pathName(&paths[id], part != 1);
        }
    }
    throw;
}

static const char *part1()
{
    return process(1);
}

static const char *part2()
{
    return process(2);
}

static const char *part3()
{
    return process(3);
}

void quest06()
{
    printf("QUEST %s\n", DAY);

    const char *d1 = part1();
    printf("\tPART 1 = %s\n", d1);

    const char *d2 = part2();
    printf("\tPART 2 = %s\n", d2);

    const char *d3 = part3();
    printf("\tPART 3 = %s\n", d3);
}

int main(int count, char **args)
{
    Timing _t;

    quest06();
}
