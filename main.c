#include <stdio.h>
#include <stdlib.h>
#include "list.h"

// int main(int argc, char const *argv[])
// {
//     llink list = NULL;

//     for (size_t i = 0; i < 100; i++)
//     {
//         insert_list(&list, i);
//     }
//     insert_list_recursive(list, 1000);
//     display_list(list);
//     printf("input the search value you want");
//     int val;
//     scanf("%d", &val);
//     llink search = search_list(list, val);

//     if (search)
//     {
//         printf("%d\n", search->vertex);
//     }
//     else
//     {
//         printf("the list does not have the value");
//     }

//     int len = list_length(list);
//     printf("the list length is %d", len);

//     return 0;
// }