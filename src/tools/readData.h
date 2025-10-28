#include <stdint.h>
#include <cstdio>
#include <cstdlib>

#ifndef READ_DATA_H
#define READ_DATA_H

namespace tools
{
    void *readData(const char day[], int part) {
        char name[100];

        snprintf(name, 100, "../data/quest%s/%i.raw", day, part);
        FILE *file = fopen(name, "r");
        
        fseek(file, 0L, SEEK_END);
        long size = ftell(file);

        char *data = (char*)malloc(size+1);
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
}

#endif
