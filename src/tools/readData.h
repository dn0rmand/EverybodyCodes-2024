#include <cstdio>
#include <cstdlib>
#include <stdint.h>

#ifndef READ_DATA_H
#define READ_DATA_H

namespace tools
{
    void *readData(const char day[], int part)
    {
        char name[100];

        snprintf(name, 100, "../data/quest%s/%i.raw", day, part);
        FILE *file = fopen(name, "r");

        fseek(file, 0L, SEEK_END);
        long size = ftell(file);

        char *data = (char *)malloc(size + 1);
        fseek(file, 0L, SEEK_SET);
        fread(data, 1, size, file);
        fclose(file);

        data[size] = 0;

        return (void *)data;
    }

    short stringLength(const char *ptr)
    {
        short length = 0;
        while (*ptr >= 32)
        {
            length++;
            ptr++;
        }
        return length;
    }

    int readNumber(char **data)
    {
        char *ptr = *data;
        // Ignore white spaces
        while (*ptr == ' ')
        {
            ptr++;
        }
        // Expect digit
        if (*ptr < '0' || *ptr > '9')
        {
            throw;
        }
        // Read value
        int value = 0;
        while (*ptr >= '0' && *ptr <= '9')
        {
            value = value * 10 + (*ptr - '0');
            ptr++;
        }
        // Ignore white spaces
        while (*ptr == ' ')
        {
            ptr++;
        }
        *data = ptr;
        return value;
    }
}

#endif
