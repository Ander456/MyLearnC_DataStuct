#include <stdio.h>
#include <stdlib.h>

#define LIMIT 20
#define BUCKETSIZE 10
#define MAP(x) (x / 2)

struct List
{
    int item;
    struct List *next;
};

struct ListHead
{
    struct List *head;
};

void bucket_init(struct ListHead **buckets, int buckstsize)
{
    int i;
    for (i = 0; i < buckstsize; i++)
    {
        buckets[i] = malloc(sizeof(struct ListHead));
        buckets[i]->head = NULL;
    }
}

void list_add(struct ListHead **buckets, int item, int positioin)
{
    printf("%d save in %d\n", item, positioin);
    struct ListHead *list = buckets[positioin];
    struct List *new = malloc(sizeof(struct List));
    new->item = item;
    new->next = NULL;

    if (list->head)
    {
        struct List *current = list->head, *prev = NULL;
        while (current)
        {
            if (current->item >= item)
            {
                new->next = current;
                if (prev)
                {
                    prev->next = new;
                }
                else
                {
                    list->head = new;
                }
                break;
            }
            prev = current;
            current = current->next;
        }
        if (prev)
            prev->next = new;
    }
    else
    {
        list->head = new;
    }
}

void bucket_add(struct ListHead **buckets, int item)
{
    list_add(buckets, item, MAP(item));
}

void bucket_scan(struct ListHead **buckets, int *data, int bucketsize)
{
    int sp = -1, i, nowvalue;
    struct List *list;

    for (i = 0; i < bucketsize; i++)
    {
        list = buckets[i]->head;

        while (list)
        {
            nowvalue = list->item;
            printf("nowvalue=%d\n", nowvalue);
            data[++sp] = nowvalue;
            list = list->next;
        }
    }
}

void bucketsort(int *data, int size)
{
    struct ListHead *buckets[BUCKETSIZE];
    // printf("%lu\n", sizeof(buckets)); //64为平台 指针占用内存为8 32位为4
    bucket_init(buckets, BUCKETSIZE);
    int i;

    for (i = 0; i < size; i++)
    {
        printf("add %d\n", data[i]);
        bucket_add(buckets, data[i]);
    }
    bucket_scan(buckets, data, BUCKETSIZE);
}

void displayoutcome(int *data, int size)
{
    int i;
    for (i = 0; i < size; i++)
        printf("%d ", *(data + i));
    printf("\n");
}

int main(int argc, char const *argv[])
{
    /*
	时间复杂度:O(n)
	空间复杂度:O()
	适用于属于一定范围内的数的排序。也就是首先得有范围 根据一定范围来 划分 桶的数量 通常用来做外部排序即 把磁盘大文件 分成小份 读取到内存中排序这种 分成小桶 知道满足内存限制
	思路:。
	*/
    int data[] = {
        5,
        4,
        3,
        19,
        3,
        9,
        16,
        15,
        0,
        0,
        9,
        12,
        9,
        10,
        10,
        4,
        19,
        15,
        12,
        16,
        0,
        0,
        1,
        1,
        3,
        4,
        14,
        3,
        6,
        12,
        1,
        3,
        14,
        4,
        15,
        4,
        4,
        7,
        11,
        13,
        2,
        1,
        15,
        13,
        1,
        12,
        9,
        17,
        13,
        16,
    };
    int size = sizeof(data) / sizeof(int);
    bucketsort(data, size);
    displayoutcome(data, size);
    return 0;
}
