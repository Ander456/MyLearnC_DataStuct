

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_LEVEL 5 //有多少层 总共
typedef char bool;
#define true 1
#define false 2

// 核心理解 一定要记住 列是单元 然后每一列的那个forward数组里存着的都是指针指向了一个Node的位置

//跳跃表节点结构体
typedef struct SkipListNode
{
    int key;
    int data;
    struct SkipListNode *forward[0]; //这个数组里的每个元素都是指针指向一个跳表节点 同理下一个同行的节点又有这个forward[同行] 指向下一个同行的节点
} SkipListNode;

//跳跃表链表结构
typedef struct SkipList
{
    int level;
    struct SkipListNode *head;
} SkipList;

//节点初始化  每创建一个 节点 其实都是 创建一个 N层的节点结构 想下 就是一个竖着的N层结构
SkipListNode *CreatSkipNode(int level, int key, int data)
{
    SkipListNode *newNode = (SkipListNode *)malloc(sizeof(SkipListNode) + level * sizeof(SkipListNode *));
    assert(newNode != NULL);

    memset(newNode, 0, sizeof(SkipListNode) + level * sizeof(SkipListNode *));

    newNode->key = key;
    newNode->data = data;

    return newNode;
}

//初始化跳表
SkipList *CreatSkipList()
{
    SkipList *newlist = (SkipList *)malloc(sizeof(SkipList));
    assert(newlist != NULL);

    newlist->head = CreatSkipNode(MAX_LEVEL - 1, 0, 0);

    for (int i = 0; i < MAX_LEVEL; i++)
    {
        newlist->head->forward[i] = NULL;
    }

    return newlist;
}

//随机产生层数
int RandLevel()
{
    int k = 1;
    while (rand() % 2)
        k++;
    return (k < MAX_LEVEL) ? k : MAX_LEVEL;
}

//插入节点
bool InsertNode(SkipList *sl, int key, int data)
{
    SkipListNode *update[MAX_LEVEL];
    SkipListNode *p, *q = NULL;
    p = sl->head;
    int k = sl->level;
    //从高到低找节点插入的位置，update存储每一层应该插入的位置
    for (int i = k - 1; i >= 0; i--)
    {
        while ((q = p->forward[i]) && (q->key < key))
        {
            p = q;
        }
        update[i] = p;
    }
    //不能插入相同的key
    if (q && q->key == key)
    {
        return false;
    }

    //产生一个随机层数
    //新建一个待插入节点q，层层插入
    k = RandLevel();
    //更新跳跃表的level
    if (k > (sl->level))
    {
        for (int i = (sl->level); i < k; ++i)
        {
            update[i] = sl->head;
        }
        sl->level = k;
    }

    q = CreatSkipNode(k, key, data);
    //逐层更新节点的指针，跟普通链表的插入一样
    for (int i = 0; i < k; ++i)
    {
        q->forward[i] = update[i]->forward[i]; //往里存指针
        update[i]->forward[i] = q;
    }
    return true;
}

//搜索指定的key
int SearchByKey(SkipList *sl, int key)
{
    SkipListNode *p, *q = NULL;
    int k = sl->level;
    p = sl->head;
    for (int i = k - 1; i >= 0; i--)
    {
        while ((q = p->forward[i]) && (q->key <= key))
        {
            if (q->key == key)
                return (q->key);
            p = q;
        }
    }
    return 0;
}

//删除指定的key
bool deleteNode(SkipList *sl, int key)
{
    SkipListNode *p, *q = NULL;
    SkipListNode *update[MAX_LEVEL];
    p = sl->head;
    int k = sl->level;
    for (int i = k - 1; i >= 0; i--)
    {
        while ((q = p->forward[i]) && (q->key < key))
        {
            p = q;
        }
        update[i] = p;
    }
    if (q && q->key == key)
    {
        //逐层删除，和删除普通链表一样
        for (int i = 0; i < sl->level; ++i)
        {
            if (update[i]->forward[i] == q)
            {
                update[i]->forward[i] = q->forward[i]; //就跟单链表一样 要删除一个把当前的这个指向要删除的下一个就好
            }
        }
        free(q);
        //如果删除的是最大层的节点，那末需要重新维护跳表
        for (int i = sl->level - 1; i >= 0; i--)
        {
            if (sl->head->forward[i] == NULL)
            {
                sl->level--;
            }
        }
        return true;
    }
    else
    {
        return false;
    }
}

//打印跳跃表
void PrintSkipList(SkipList *sl)
{
    SkipListNode *p, *q = NULL;
    //从最高层开始打印
    int k = sl->level;
    for (int i = k - 1; i >= 0; i--)
    {
        p = sl->head;
        while ((q = p->forward[i]))
        {
            printf("%d->", p->data);
            p = q;
        }
        printf("\n");
    }
    printf("\n");
}

int main(void)
{
    SkipList *sl = CreatSkipList();
    PrintSkipList(sl);
    for (int i = 0; i <= 40; i++)
    {
        InsertNode(sl, i, i * 2);
        PrintSkipList(sl);
    }
    PrintSkipList(sl);
    int m = SearchByKey(sl, 4);
    printf("serch value = %d\n", m);

    bool b = deleteNode(sl, 4);
    if (b)
    {
        printf("删除成功\n");
    }
    PrintSkipList(sl);
    system("pause");
    return 0;
}
