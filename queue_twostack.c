#include <stdio.h>
#include <stdlib.h>

// 这里是用了两个 stack 来实现 queue

#define STACKSIZE 100

int stack_one[STACKSIZE];
int sp_one = -1;
void push_one(int item)
{
    stack_one[++sp_one] = item;
}

int pop_one(void)
{
    return stack_one[sp_one--];
}

int empty_one(void)
{
    return sp_one == -1;
}

int full_one(void)
{
    return sp_one == STACKSIZE - 1;
}

int stack_two[STACKSIZE];
int sp_two = -1;
void push_two(int item)
{
    stack_two[++sp_two] = item;
}

int pop_two(void)
{
    return stack_two[sp_two--];
}

int empty_two(void)
{
    return sp_two == -1;
}

int full_two(void)
{
    return sp_two == STACKSIZE - 1;
}

int empty_queue()
{
    return empty_one() && empty_two();
}

int full_queue()
{
    return full_one() && full_two();
}

void enqueue(int item)
{

    if (full_queue())
    {
        printf("Full\n");
        return;
    }

    if (!full_one())
    {
        push_one(item);
    }
    else
    {

        while (!full_two() && !empty_one())
        {
            push_two(pop_one());
        }
        push_one(item);
    }
}

// 其实stack本来先进后出 为了实现dequeue 就得 倒出来 放到另一个里
int dequeue(void)
{
    int retval;

    if (empty_two())
    {

        while (!empty_one())
        {
            retval = pop_one();
            push_two(retval);
        }
    }

    if (empty_two())
    {
        printf("Queue empty.\n");
        return -1;
    }
    else
    {
        retval = pop_two();
    }
    return retval;
}

int main(int argc, char const *argv[])
{
    int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int size = sizeof(data) / sizeof(int);
    int i = 0;

    while (i < 6)
    {
        enqueue(data[i++]);
    }

    while (!empty_queue())
    {
        printf("%d\n", dequeue());
    }

    while (i < size)
    {
        enqueue(data[i++]);
    }

    while (!empty_queue())
    {
        printf("%d\n", dequeue());
    }

    return 0;
}
