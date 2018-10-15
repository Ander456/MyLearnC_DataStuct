#include <stdio.h>
#include <stdlib.h>

struct List
{
    int item;
    struct List *next;
};

struct Stack
{
    int size;
    struct List *head;
};
typedef struct Stack *slink;

slink create_stack(void)
{
    slink stack = malloc(sizeof(struct Stack));

    if (!stack)
    {
        return stack;
    }
    stack->size = 0;
    stack->head = NULL;
    return stack;
}

int empty(slink stack)
{
    return stack->size == 0;
}

void push_stack(slink stack, int item)
{
    struct List *new = malloc(sizeof(struct List));

    if (!new)
    {
        return;
    }
    // 下面巧妙的把链表倒着增加最后head指向最后一个新的然后next指向倒数第二个 和逆序的那个思想一样
    new->item = item;
    new->next = stack->head;
    stack->head = new;
    stack->size++;
}

int pop_stack(slink stack)
{

    if (empty(stack))
    {
        return -1;
    }

    struct List *torm = stack->head;
    int retval = torm->item;
    stack->head = stack->head->next;
    free(torm);
    stack->size--;
    return retval;
}

int main(int argc, char const *argv[])
{
    slink stack = create_stack();
    int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int i = 0;

    while (i < 6)
    {
        push_stack(stack, data[i++]);
    }

    while (!empty(stack))
    {
        printf("%d\n", pop_stack(stack));
    }

    while (i < 9)
    {
        push_stack(stack, data[i++]);
    }

    while (!empty(stack))
    {
        printf("%d\n", pop_stack(stack));
    }

    return 0;
}
