#ifndef TOOLS_MAP_H
#define TOOLS_MAP_H

#include "./readData.h"
#include <string.h>

namespace tools
{
    class Map
    {
    private:
        char *data;

    protected:
        const int getIndex(int x, int y)
        {
            return x + y * (width + 1);
        }

    public:
        int width;
        int height;

    public:
        Map(const char *day, int part)
        {
            data = (char *)tools::readData(day, part);
            width = (int)tools::stringLength(data);
            height = (int)(strlen(data) + 1) / (width + 1);
        }

        Map(const void *input)
        {
            data = (char *)input;
            width = (int)tools::stringLength(data);
            height = (int)(strlen(data) + 1) / (width + 1);
        }

        ~Map()
        {
            free((void *)data);
        }

        virtual const char get(int x, int y)
        {
            if (x < 0 || y < 0 || x >= width || y >= height)
            {
                return '#';
            }
            return data[getIndex(x, y)];
        }
    };
}
#endif
