#include <stdio.h>
#include "list.h"

void reverse_non_recursive(llink *list)
{
    llink prev = NULL, cur = *list, next;

    while (cur)
    {
        next = cur->next;
        cur->next = prev;
        prev = cur;
        cur = next;
    }
    *list = prev;
}

// 惊呆了我曹。。递归的思想 把整个上下文都推进去 然后逐步退出来 牛逼的不行
llink reverse_recursive(llink *list, llink cur)
{

    if (!cur)
    {
        return NULL;
    }
    llink prev = reverse_recursive(list, cur->next);
    cur->next = NULL;

    if (prev)
    {
        prev->next = cur;
    }
    else
    {
        *list = cur;
    }
    return cur;
}

// int main(int argc, char const *argv[])
// {
//     int data[] = {1, 2, 3, 4, 5, 6, 7};
//     int size = sizeof(data) / sizeof(int);
//     llink list = NULL;
//     int i;
//     for (i = 0; i < size; i++)
//     {
//         insert_list(&list, data[i]);
//     }
//     display_list(list);
//     // reverse_non_recursive(&list);
//     reverse_recursive(&list, list);
//     display_list(list);
//     return 0;
// }
