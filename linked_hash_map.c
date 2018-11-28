// linked_hash_map 用双向链表+hash方式实现的数据结构
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// #include "mcheck.h"

// #include "doublelist.h"
struct list_head
{
    struct list_head *next, *prev;
};

// 初始化节点 设置name节点的前继节点和后继节点都是指向name本身
#define LIST_HEAD_INIT(name) \
    {                        \
        &(name), &(name)     \
    }

// 定义表头节点：新建双向链表表头name 并设置name的前继节点和后继节点都是指向name本身
#define LIST_HEAD(name) struct list_head name = LIST_HEAD_INIT(name)

// 初始化节点：将list节点的前继节点和后继节点都是指向list本身。
static inline void INIT_LIST_HEAD(struct list_head *list)
{
    list->next = list;
    list->prev = list;
}

// 添加节点 将new插入到prev和next之间
static inline void __list_add(struct list_head *newer, struct list_head *prev, struct list_head *next)
{
    next->prev = newer;
    newer->next = next;
    newer->prev = prev;
    prev->next = newer;
}

// 添加new节点 将new节点添加到head之前 即将new添加到双向链表末尾
static inline void list_add_tail(struct list_head *newer, struct list_head *head)
{
    __list_add(newer, head->prev, head);
}

// 从双向链表中删除entry节点
static inline void __list_del(struct list_head *prev, struct list_head *next)
{
    next->prev = prev;
    prev->next = next;
}

// 从双链表中删除entry节点
static inline void __list_del_entry(struct list_head *entry)
{
    __list_del(entry->prev, entry->next);
}

// 从双链表中删除entry节点，并将entry节点的前继节点和后继节点都指向entry本身
static inline void list_del_init(struct list_head *entry)
{
    __list_del_entry(entry);
    INIT_LIST_HEAD(entry);
}

// 用new节点取代old节点
static inline void list_replace(struct list_head *old, struct list_head *newer)
{
    newer->next = old->next;
    newer->next->prev = newer;
    newer->prev = old->prev;
    newer->prev->next = newer;
}

// 双量表是否为空
static inline int list_empty(const struct list_head *head)
{
    return head->next == head;
}

// 获取"MEMBER成员"在"结构体TYPE"中的位置偏移
#define offsetof(TYPE, MEMBER) ((size_t) & ((TYPE *)0)->MEMBER)

// 根据"结构体(type)变量"中的"域成员变量(member)的指针(ptr)"来获取指向整个结构体变量的指针
#define container_of(ptr, type, member) ({          \
    const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
    (type *)( (char *)__mptr - offsetof(type,member) ); })

// 遍历双向链表
#define list_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); pos = pos->next)

#define list_for_each_safe(pos, n, head)                   \
    for (pos = (head)->next, n = pos->next; pos != (head); \
         pos = n, n = pos->next)

#define list_entry(ptr, type, member) \
    container_of(ptr, type, member)

typedef struct _linked_hash_map_node
{
    void *key;
    void *data;
    struct _linked_hash_map_node *next; // 哈希表冲突时候用来挂 后继节点
    struct list_head Dlist_node;        // 用来挂接双向链表
} LinkedHashMapNode;

typedef struct _linked_hash_map
{
    LinkedHashMapNode **hTabs;
    struct list_head header;
    int size;
    int nel_max;
    int nel;
    int (*hash_value)(struct _linked_hash_map *h, const void *key);
    int (*keycomp)(struct _linked_hash_map *h, const void *key1, const void *key2);
    void (*hash_node_free)(LinkedHashMapNode *node, int flg);
} LinkedHashMap;

typedef int (*hash_value_func)(struct _linked_hash_map *h, const void *key);                /*哈希函数*/
typedef int (*keycmp_func)(struct _linked_hash_map *h, const void *key1, const void *key2); /*哈希key比较函数，当哈希数值一致时使用*/
typedef void (*hash_node_free_func)(LinkedHashMapNode *node, int flg);

LinkedHashMapNode *LinkedHashMap_delete(LinkedHashMap *h, void *key);

LinkedHashMap *LinkedHashMap_Create(int size, int nel_max, hash_value_func hash_value, keycmp_func keycmp, hash_node_free_func hash_node_free)
{
    int i = 0;
    LinkedHashMap *h = NULL;
    if (size <= 0 || hash_value == NULL || keycmp == NULL)
        return NULL;

    h = malloc(sizeof(LinkedHashMap));
    if (h == NULL)
        return NULL;

    h->hTabs = malloc(sizeof(LinkedHashMapNode *) * size);

    if (h->hTabs == NULL)
    {
        return NULL;
    }

    h->size = size;
    h->nel = 0;
    h->nel_max = nel_max;
    h->hash_value = hash_value;
    h->keycomp = keycmp;
    h->hash_node_free = hash_node_free;

    for (i = 0; i < size; i++)
        h->hTabs[i] = NULL;

    INIT_LIST_HEAD(&(h->header));

    return h;
}

void LinkedHashMap_destroy(LinkedHashMap *h)
{
    struct list_head *pos = NULL;
    struct list_head *next = NULL;
    LinkedHashMapNode *ptmp = NULL;
    if (h == NULL)
        return;

    list_for_each_safe(pos, next, &(h->header))
    {
        ptmp = container_of(pos, LinkedHashMapNode, Dlist_node);
        list_del_init(pos);
        if (h->hash_node_free != NULL)
        {
            h->hash_node_free(ptmp, 1);
        }
        free(h->hTabs);
        free(h);
    }
}

int LinkedHashMap_insert(LinkedHashMap *h, void *key, void *data)
{
    int i = 0;
    int hPos = 0;
    struct list_head *pos = NULL;
    LinkedHashMapNode *cur = NULL;
    LinkedHashMapNode *prev = NULL;

    hPos = h->hash_value(h, key);
    cur = h->hTabs[hPos];
    while (cur != NULL && h->keycomp(h, key, cur->key) != 0)
    {
        prev = cur;
        cur = cur->next;
    }
    if (cur == NULL)
    {
        // 链表节点满时，取表头结点，从当前哈希表和双向量表中都删除
        if (h->nel_max == h->nel)
        {
            cur = LinkedHashMap_delete(h, list_entry(h->header.next, LinkedHashMapNode, Dlist_node)->key);
            assert(cur != NULL);
            h->hash_node_free(cur, 0);
        }
        else
        {
            //不满继续创建新的节点
            cur = malloc(sizeof(LinkedHashMapNode));
            if (cur == NULL)
                return 1;
        }
        //插入到hash桶中
        if (prev == NULL)
        {
            cur->next = h->hTabs[hPos];
            h->hTabs[hPos] = cur;
        }
        else
        {
            cur->next = prev->next;
            prev->next = cur;
        }
        h->nel++;
    }
    else
    {
        //从双向链表中删除
        list_del_init(&(cur->Dlist_node));
        //只删除key和data内存
        h->hash_node_free(cur, 0);
    }

    cur->key = key;
    cur->data = data;

    list_add_tail(&(cur->Dlist_node), &(h->header));

    return 0;
}

LinkedHashMapNode *LinkedHashMap_delete(LinkedHashMap *h, void *key)
{
    int hPos = 0;
    struct list_head *pos = NULL;
    LinkedHashMapNode *prev = NULL;
    LinkedHashMapNode *cur = NULL;

    //查找当前节点
    hPos = h->hash_value(h, key);
    cur = h->hTabs[hPos];

    while (cur != NULL && h->keycomp(h, key, cur->key) != 0)
    {
        prev = cur;
        cur = cur->next;
    }

    if (cur == NULL)
        return NULL;

    if (prev == NULL)
        h->hTabs[hPos] = cur->next;
    else
        prev->next = cur->next;

    list_del_init(&(cur->Dlist_node));
    h->nel--;
    return cur;
}

void *LinkedHashMap_search(LinkedHashMap *h, void *key)
{
    int hPos = 0;
    LinkedHashMapNode *cur = NULL;
    hPos = h->hash_value(h, key);
    cur = h->hTabs[hPos];
    while (cur != NULL && h->keycomp(h, key, cur->key) != 0)
        cur = cur->next;
    if (cur == NULL)
        return NULL;

    //从双向链表中删除节点 加入尾部
    if (h->header.prev != &(cur->Dlist_node))
    {
        list_del_init(&(cur->Dlist_node));
        list_add_tail(&(cur->Dlist_node), &(h->header));
    }
    return cur->data;
}

void LinkedHashMap__dump(LinkedHashMap *h)
{
    int i = 0;
    LinkedHashMapNode *cur = NULL;
    struct list_head *pos = NULL;
    if (h == NULL)
        return;

    printf("\r\n----开始--size[%d],nel[%d]------------", h->size, h->nel);

    for (i = 0; i < h->size; i++)
    {
        printf("\r\n htables[%d]:", i);
        cur = h->hTabs[i];
        while (cur != NULL)
        {
            printf("key[%s],data[%s] ", cur->key, cur->data);
            cur = cur->next;
        }
    }

    printf("\r\n--------------------------------------------------------\r\n");

    list_for_each(pos, &(h->header))
    {
        cur = list_entry(pos, LinkedHashMapNode, Dlist_node);
        printf("key[%s] ", cur->key);
    }

    printf("\r\n----结束--size[%d],nel[%d]------------", h->size, h->nel);
}

struct test_node
{
    char key[80];
    char data[80];
};

unsigned int siample_hash(const char *str)
{
    register unsigned int hash = 0;
    register unsigned int seed = 131;

    while (*str)
    {
        hash = hash * seed + *str++;
    }

    return hash & (0x7FFFFFFF);
}

int hashtab_hvalue(LinkedHashMap *h, const void *key)
{
    return (siample_hash(key) % h->size);
}

int hashtab_keycmp(LinkedHashMap *h, const void *key1, const void *key2)
{
    return strcmp(key1, key2);
}

void hashtab_node_free(LinkedHashMapNode *node, int flg)
{
    struct test_node *ptmp = NULL;

    ptmp = list_entry(node->key, struct test_node, key);

    free(ptmp);
    if (flg)
    {
        free(node);
    }
}

int main(int argc, char const *argv[])
{
    int i = 0;
    int res = 0;
    char *pres = NULL;
    LinkedHashMapNode *node = NULL;
    struct test_node *p = NULL;
    LinkedHashMap *h = NULL;
    setenv("MALLOC_TRACE", "1.txt", 1);
    // mtrace();

    h = LinkedHashMap_Create(3, 6, hashtab_hvalue, hashtab_keycmp, hashtab_node_free);
    assert(h != NULL);
    while (1)
    {
        p = (struct test_node *)malloc(sizeof(struct test_node));
        assert(p != NULL);
        printf("\r\n 请输入key 和value，当可以等于\"quit\"时退出");
        scanf("%s", p->key);
        scanf("%s", p->data);

        if (strcmp(p->key, "quit") == 0)
        {
            free(p);
            break;
        }

        res = LinkedHashMap_insert(h, p->key, p->data);
        if (res != 0)
        {
            free(p);
            printf("\r\n key[%s],data[%s] insert failed %d", p->key, p->data, res);
        }
        else
        {
            printf("\r\n key[%s],data[%s] insert success %d", p->key, p->data, res);
        }
        LinkedHashMap__dump(h);
    }

    while (1)
    {
        p = (struct test_node *)malloc(sizeof(struct test_node));
        assert(p != NULL);
        printf("\r\n 请输入key 查询value的数值，当可以等于\"quit\"时退出");
        scanf("%s", p->key);

        if (strcmp(p->key, "quit") == 0)
        {
            free(p);
            break;
        }
        pres = LinkedHashMap_search(h, p->key);
        if (pres == NULL)
        {
            printf("\r\n key[%s] search data failed", p->key);
        }
        else
        {
            printf("\r\n key[%s],search data[%s] success", p->key, pres);
        }
        free(p);
        LinkedHashMap__dump(h);
    }

    while (1)
    {
        p = (struct test_node *)malloc(sizeof(struct test_node));
        assert(p != NULL);
        printf("\r\n 请输入key 删除节点的数值，当可以等于\"quit\"时退出");
        scanf("%s", p->key);

        if (strcmp(p->key, "quit") == 0)
        {
            free(p);
            break;
        }
        node = LinkedHashMap_delete(h, p->key);
        if (node == NULL)
        {
            printf("\r\n key[%s] delete node failed ", p->key);
        }
        else
        {
            printf("\r\n key[%s],delete data[%s] success", node->key, node->data);
            h->hash_node_free(node, 1);
        }
        free(p);
        LinkedHashMap__dump(h);
    }

    LinkedHashMap_destroy(h);

    // void muntrace();

    return 0;
}
