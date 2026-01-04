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
        bool autoDelete;

        void initialize(const void *input)
        {
            data = (char *)input;
            width = (int)tools::stringLength(data);
            height = (int)(strlen(data) + 1) / (width + 1);
        }

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
            autoDelete = true;

            initialize(tools::readData(day, part));
        }

        Map(const void *input, bool autoDelete = true)
        {
            autoDelete = autoDelete;
            initialize(input);
        }

        ~Map()
        {
            if (autoDelete)
            {
                free((void *)data);
            }
        }

        virtual const char get(int x, int y)
        {
            if (x < 0 || y < 0 || x >= width || y >= height)
            {
                return '#';
            }
            return data[getIndex(x, y)];
        }

        virtual const char set(int x, int y, const char c)
        {
            if (x < 0 || y < 0 || x >= width || y >= height)
            {
                return '#';
            }
            const char old = data[getIndex(x, y)];
            data[getIndex(x, y)] = c;
            return old;
        }
    };
}
#endif
