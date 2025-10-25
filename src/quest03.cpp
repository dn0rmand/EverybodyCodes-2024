#include <stdio.h>
#include <string.h>
#include "tools/readData.h"

class Data
{
private:
    char *data;
    int totalSize;
    bool diagonals;

public:
    int width;
    int height;

public:
    Data(void *data, bool diagonals = false)
    {
        this->data = (char *)data;
        this->totalSize = strlen((char *)data);

        this->width = tools::stringLength((char *)data);
        this->height = (this->totalSize + 1) / (width + 1);
        this->diagonals = diagonals;
    }

    ~Data()
    {
        free(this->data);
    }

private:
    void set(int x, int y, char c)
    {
        if (y < 0 || y >= this->height || x < 0 || x >= this->width)
        {
            throw;
        }

        int index = x + y * (this->width + 1);
        if (index >= this->totalSize)
        {
            throw;
        }
        this->data[index] = c;
    }

    char get(int x, int y)
    {
        if (y < 0 || y >= this->height || x < 0 || x >= this->width)
        {
            return '0';
        }

        int index = x + y * (this->width + 1);
        if (index >= this->totalSize)
        {
            throw;
        }
        const char c = this->data[index];
        if (c < 32)
        {
            throw;
        }
        return c == '.' ? '0' : (c == '#' ? '1' : c);
    }

    bool bad(char c1, char c2)
    {
        if (c1 == '0' || c2 == '0')
        {
            return true;
        }
        int d = c1 - c2;
        return d < -1 || d > 1;
    }

    bool canBeRemoved(int x, int y, char deep)
    {
        if (bad(this->get(x, y - 1), deep) ||
            bad(this->get(x - 1, y), deep) ||
            bad(this->get(x + 1, y), deep) ||
            bad(this->get(x, y + 1), deep))
        {
            return false;
        }

        if (this->diagonals)
        {
            if (bad(this->get(x - 1, y - 1), deep) ||
                bad(this->get(x + 1, y - 1), deep) ||
                bad(this->get(x - 1, y + 1), deep) ||
                bad(this->get(x + 1, y + 1), deep))
            {
                return false;
            }
        }
        return true;
    }

public:
    int step(char step)
    {
        int total = 0;

        for (int y = 0; y < this->height; y++)
        {
            for (int x = 0; x < this->width; x++)
            {
                char deep = this->get(x, y);
                if (deep == '0')
                {
                    continue;
                }

                if (step == '0' && deep == '1')
                {
                    total++;
                    this->set(x, y, '1');
                }
                else if (this->canBeRemoved(x, y, deep + 1))
                {
                    this->set(x, y, deep + 1);
                    total++;
                }
            }
        }

        return total;
    }
};

static unsigned int part1()
{
    Data map(tools::readData("03", 1));
    int total = 0;

    for (char step = '0';; step++)
    {
        int extra = map.step(step);
        if (extra == 0)
        {
            break;
        }
        total += extra;
    }
    return total;
}

static unsigned int part2()
{
    Data map(tools::readData("03", 2));
    int total = 0;

    for (char step = '0';; step++)
    {
        int extra = map.step(step);
        if (extra == 0)
        {
            break;
        }
        total += extra;
    }
    return total;
}

static unsigned int part3()
{
    Data map(tools::readData("03", 3), true);
    int total = 0;

    for (char step = '0';; step++)
    {
        int extra = map.step(step);
        if (extra == 0)
        {
            break;
        }
        total += extra;
    }
    return total;
}

void quest03()
{
    printf("QUEST 3\n");

    unsigned int d1 = part1();
    printf("\tPART 1 = %u\n", d1);

    unsigned int d2 = part2();
    printf("\tPART 2 = %u\n", d2);

    unsigned int d3 = part3();
    printf("\tPART 3 = %u\n", d3);
}

int main(int count, char **args)
{
    quest03();
}
