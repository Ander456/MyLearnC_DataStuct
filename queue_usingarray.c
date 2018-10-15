#include <stdio.h>
#include <stdlib.h>

// 这里实现的是环形队列 也就是循环的 。为了dequeue的时候提高效率 否则就得搬移

struct Queue
{
    int capibility;
    int *data; //一般这种都是用来存数组的 看下面head tail都是int类型的标识索引
    int head;
    int tail;
};
typedef struct Queue *qarray;

qarray create_queue(int capibility)
{
    qarray q = malloc(sizeof(struct Queue));

    if (!q)
    {
        return q;
    }
    q->capibility = capibility;
    q->data = malloc(sizeof(int) * capibility);
    q->head = q->tail = 0;
    return q;
}

int full(qarray q)
{
    return q->head == (q->tail + 1) % q->capibility; //通过这行就可以看出来是循环顺序队列
}

int empty(qarray q)
{
    return q->head == q->tail;
}

void enqueue(qarray q, int item)
{

    if (full(q))
    {
        return;
    }
    q->data[q->tail] = item;
    q->tail = (q->tail + 1) % q->capibility;
}

int dequeue(qarray q)
{

    if (empty(q))
    {
        return -1;
    }
    int retval = q->data[q->head];
    q->head = (q->head + 1) % q->capibility;
    return retval;
}

int main(int argc, char const *argv[])
{
    int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    int i = 0;
    qarray q = create_queue(8);

    while (i < 5)
    {
        enqueue(q, data[i++]);
    }

    while (!empty(q))
    {
        // dequeue(q);
        printf("%d\n", dequeue(q));
    }

    while (i < 15 && !full(q))
    {
        enqueue(q, data[i++]);
    }

    while (!empty(q))
    {
        printf("%d\n", dequeue(q));
    }

    return 0;
}
