#include <stdio.h>
#include <stdlib.h>

struct List
{
    int item;
    struct List *next;
};

struct Queue
{
    int size;
    struct List *head;
    struct List *tail;
};
typedef struct Queue *qlink;

// 现在看看 云风的那个 MQ 嗯~ 有点意思

qlink create_queue(void)
{
    qlink queue = malloc(sizeof(struct Queue));

    if (!queue)
    {
        return queue;
    }
    queue->size = 0;
    queue->head = queue->tail = NULL;
    return queue;
}

int empty(qlink queue)
{
    return queue->size == 0;
}

void enqueue(qlink queue, int item)
{
    struct List *new = malloc(sizeof(struct List));

    if (!new)
    {
        return;
    }
    new->item = item;
    new->next = NULL;

    if (queue->head)
    {
        queue->tail->next = new;
    }
    else
    {
        queue->head = new;
    }
    queue->tail = new;
    queue->size++;
}

int dequeue(qlink queue)
{

    if (empty(queue))
    {
        return -1;
    }
    int retval = queue->head->item;
    struct List *torm = queue->head;

    if (queue->head == queue->tail)
    {
        queue->tail = NULL;
    }
    queue->head = queue->head->next;
    free(torm);
    queue->size--;
    return retval;
}

int main(int argc, char const *argv[])
{
    int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int i = 0;
    qlink q = create_queue();

    while (i < 5)
    {
        enqueue(q, data[i++]);
    }

    while (!empty(q))
    {
        printf("%d\n", dequeue(q));
    }

    while (i < 10)
    {
        enqueue(q, data[i++]);
    }

    while (!empty(q))
    {
        printf("%d\n", dequeue(q));
    }

    return 0;
}
