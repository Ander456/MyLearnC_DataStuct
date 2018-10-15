#include <stdio.h>
#include <stdlib.h>
#include "list.h"

void delete_item_advanced(llink node)
{
    llink next = node->next;

    if (next)
    {
        node->vertex = next->vertex;
        node->next = next->next;
        free(next);
    }
    else
    {
        printf("cannot delete %d for its the last item\n", node->vertex);
    }
}

void delete_item(llink list, llink node)
{

    if (!list)
    {
        return;
    }

    llink after = node->next;
    llink before = NULL;
    while (list)
    {

        if (list->next && list->next->vertex == node->vertex)
        {
            before = list;
            break;
        }
        list = list->next;
    }

    if (!before)
    {
        // 表示node是头节点
        free(node);
    }
    else
    {
        if (!after)
        {
            // 末尾节点
            before->next = NULL;
        }
        else
        {
            before->next = after;
        }
        free(node);
    }
}

// int main(int argc, char const *argv[])
// {
//     int data[] = {0, 1, 2, 3, 4};
//     int size = sizeof(data) / sizeof(int);
//     llink list = NULL;

//     for (size_t i = 0; i < size; i++)
//     {
//         insert_list(&list, data[i]);
//     }
//     display_list(list);

//     int del[] = {4, 3, 1, 2};
//     llink node = NULL;

//     for (size_t i = 0; i < size; i++)
//     {
//         int item = del[i];

//         if (!item)
//         {
//             continue;
//         }

//         node = find_item(list, del[i]);

//         if (node)
//         {
//             // delete_item_advanced(node);
//             delete_item(list, node);
//             display_list(list);
//         }
//         else
//         {
//             printf("can node delete the node because list does not have the node\n");
//         }
//     }
//     return 0;
// }