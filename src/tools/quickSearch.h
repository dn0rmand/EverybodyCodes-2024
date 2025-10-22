#include <stdint.h>

#ifndef QUICK_SEARCH_H
#define QUICK_SEARCH_H

namespace tools
{
    template <typename T>
    int quickSearch(T *data, T value, int length)
    {
        int min = 0;
        int max = length - 1;

        while (min < max)
        {
            int middle = (min + max) / 2;

            if (middle == min || middle == max)
            {
                if (data[min] == value)
                {
                    return min;
                }
                if (data[max] == value)
                {
                    return max;
                }
                return -1;
            }

            if (data[middle] < value)
            {
                min = middle + 1;
            }
            else if (data[middle] > value)
            {
                max = middle - 1;
            }
            else
            {
                return middle;
            }
        }

        if (min >= 0 && min < length && data[min] == value)
        {
            return min;
        }

        if (max >= 0 && max < length && data[max] == value)
        {
            return max;
        }

        return -1;
    }
}

#endif
