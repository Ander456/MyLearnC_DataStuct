#include <stdio.h>
#include <stdlib.h>

struct Stack
{
    int capability;
    int sp;
    int *data;
};
typedef struct Stack *sarray;

sarray create_stack(int capability)
{
    sarray stack = malloc(sizeof(struct Stack));

    if (!stack)
    {
        return stack;
    }
    stack->capability = capability;
    stack->data = malloc(sizeof(int) * capability);
    stack->sp = -1;
    return stack;
}

int empty(sarray stack)
{
    return stack->sp == -1;
}

int full(sarray stack)
{
    return stack->sp == stack->capability - 1;
}

void push_stack(sarray stack, int item)
{

    if (full(stack))
    {
        printf("Full Stack");
        return;
    }

    stack->data[++stack->sp] = item;
}

int pop_stack(sarray stack)
{

    if (empty(stack))
    {
        return -1;
    }
    return stack->data[stack->sp--];
}

int main(int argc, char const *argv[])
{
    sarray stack = create_stack(5);
    int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int i = 0;

    while (!full(stack))
    {
        push_stack(stack, data[i++]);
    }

    while (!empty(stack))
    {
        printf("%d\n", pop_stack(stack));
    }

    while (i < 10 && !full(stack))
    {
        push_stack(stack, data[i++]);
    }

    while (!empty(stack))
    {
        printf("%d\n", pop_stack(stack));
    }

    return 0;
}
