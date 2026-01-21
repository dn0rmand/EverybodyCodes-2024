#include <cstdio>
#include <cstdlib>
#include <stdint.h>

#ifndef READ_DATA_H
#define READ_DATA_H

namespace tools
{
    void *readData(const char day[], int part);

    short stringLength(const char *ptr);

    int readNumber(char **data);
}

#endif
