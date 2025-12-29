#ifndef TOOLS_MAP_H
#define TOOLS_MAP_H

#include "./readData.h"
#include <string.h>

namespace tools
{
    class Map
    {
    private:
        const char *data;

    public:
        int width;
        int height;

    public:
        Map(const char *day, int part)
        {
            data = (const char *)tools::readData(day, part);
            width = (int)tools::stringLength(data);
            height = (int)(strlen(data) + 1) / (width + 1);
        }

        Map(const void *input)
        {
            data = (const char *)input;
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
}
#endif
