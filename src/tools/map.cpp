#include "./map.h"
#include <cstdlib>

namespace tools
{
    void Map::initialize(const void *input)
    {
        data = (char *)input;
        width = (int)tools::stringLength(data);
        height = (int)(strlen(data) + 1) / (width + 1);
    }

    int Map::getIndex(int x, int y)
    {
        return x + y * (width + 1);
    }

    Map::Map(const char *day, int part)
    {
        autoDelete = true;
        initialize(tools::readData(day, part));
    }

    Map::Map(const void *input, bool autoDelete)
    {
        this->autoDelete = autoDelete;
        initialize(input);
    }

    Map::~Map()
    {
        if (autoDelete)
        {
            free((void *)data);
        }
    }

    char Map::get(int x, int y)
    {
        if (x < 0 || y < 0 || x >= width || y >= height)
        {
            return '#';
        }
        return data[getIndex(x, y)];
    }

    char Map::set(int x, int y, const char c)
    {
        if (x < 0 || y < 0 || x >= width || y >= height)
        {
            return '#';
        }
        const char old = data[getIndex(x, y)];
        data[getIndex(x, y)] = c;
        return old;
    }
}
