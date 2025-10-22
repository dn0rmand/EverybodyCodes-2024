#include <stdint.h>

#ifndef QUICK_SORT_H
#define QUICK_SORT_H

namespace tools
{
    template <typename T>
    void swap(T *data, int i, int j)
    {
        T temp = data[i];

        data[i] = data[j];
        data[j] = temp;
    }

    template <typename T>
    int partition(T *data, int low, int high)
    {
        T pivot = data[high];
        T i = (low - 1);

        for (int j = low; j <= high - 1; j++)
        {
            if (data[j] <= pivot)
            {
                i++;
                swap<T>(data, i, j);
            }
        }

        swap<T>(data, i + 1, high);
        return (i + 1);
    }

    template <typename T>
    void quickSort(T *data, int low, int high)
    {
        if (low < high)
        {
            int pi = partition<T>(data, low, high);

            quickSort<T>(data, low, pi - 1);
            quickSort<T>(data, pi + 1, high);
        }
    }
}

#endif
