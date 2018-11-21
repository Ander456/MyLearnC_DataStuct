#include <stdlib.h>
#include <stdio.h>

int binarysearch(int *data, int size, int item)
{
    int low = 0;
    int hight = size - 1;
    while (low <= hight)
    {
        int mid = (low + hight) / 2;
        if (data[mid] == item)
            return mid;
        else if (data[mid] < item)
            low = mid + 1;
        else
            hight = mid - 1;
    }
    return -1;
}

int binarysearh_first_equal(int *data, int size, int item)
{
    int low = 0;
    int high = size - 1;
    while (low <= high)
    {
        int mid = low + ((high - low) >> 1);
        if (data[mid] >= item)
            high = mid - 1;
        else
            low = mid + 1;
    }
    if (low < size && data[low] == item)
        return low;
    else
        return -1;
}

int main(int argc, char const *argv[])
{
    int array[] = {1, 4, 7, 7, 11, 45, 88, 88, 99, 100};
    int search = 88;
    int size = sizeof(array) / sizeof(int);
    int ret = binarysearch(array, size, search);
    printf("find %d\n", ret);
    ret = binarysearh_first_equal(array, size, search);
    printf("find %d\n", ret);
    return 0;
}
