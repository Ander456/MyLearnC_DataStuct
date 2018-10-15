#include <stdio.h>
#include "list.h"

// 我曹好牛逼 快慢俩指针搞定 真牛逼
llink get_middle_item(llink list)
{
    llink one_p = list, two_p = list;
    while (two_p)
    {
        two_p = two_p->next; // 神来之笔

        if (two_p)
        {
            two_p = two_p->next;
        }

        if (two_p)
        {
            one_p = one_p->next;
        }
    }
    return one_p;
}

// 获取倒数第k个item
llink get_klast_item(llink list, int k)
{
    int i = 1;
    llink cur = list, curk = list;

    while (i++ <= k && cur)
    {
        curk = curk->next;
    }

    if (i > k && !curk)
    {
        return cur;
    }

    if (!curk)
    {
        return NULL;
    }

    while (curk)
    {
        cur = cur->next;
        curk = curk->next;
    }
    return cur;
}

// int main(int argc, char const *argv[])
// {
//     int data[] = {1, 2, 3, 4, 5, 6, 7};
//     int size = sizeof(data) / sizeof(int);
//     llink list = NULL;

//     for (size_t i = 0; i < size; i++)
//     {
//         insert_list(&list, data[i]);
//     }
//     llink mid = get_middle_item(list);
//     printf("middle=%d\n", mid->vertex);

//     llink k;

//     for (int i = 0; i <= size; i++)
//     {
//         k = get_klast_item(list, i);

//         if (k)
//         {
//             printf("%d last = %d\n", i, k->vertex);
//         }
//     }

//     return 0;
// }
