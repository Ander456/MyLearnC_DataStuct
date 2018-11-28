#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define MAX_TREE_NODES (1 << 8)

struct node
{
    int data;
};

struct binary_tree
{
    union {
        unsigned long nodes;
        struct node *n[MAX_TREE_NODES];
    };
};

void init_binary_tree(struct binary_tree *tree)
{
    int i;
    for (i = 0; i < MAX_TREE_NODES; i++)
        tree->n[i] = NULL;
}

struct node *create_node(int data)
{
    struct node *n;
    n = malloc(sizeof(struct node));
    if (n)
        n->data = data;
    return n;
}

void fake_a_tree(struct binary_tree *tree)
{
    int i, data[10] = {7, 4, 9, 2, 6, 8, 10, 1, 3, 5}; //这棵模拟的树是 完全二叉树 只浪费了0控件（方便计算） 这是用数组实现的不是链表
    init_binary_tree(tree);
    for (i = 0; i < 10; i++)
        tree->n[i + 1] = create_node(data[i]);
    tree->nodes = 10;
}

//这里也是利用了 二叉搜索树 也是 儿茶顺序树的原理 按照顺序打印出来了 中序遍历的方法 （当然 二叉树还可以 前序从根遍历 或者逆序从左叶右叶然后依次递归往上的方式遍历）
void _in_order(struct binary_tree *tree, int index)
{
    if (!tree->n[index])
        return;

    _in_order(tree, index << 1);

    printf("[%2d]: %4d\n", index, tree->n[index]->data);

    _in_order(tree, (index << 1) + 1);
}

void in_order(struct binary_tree *tree)
{
    _in_order(tree, 1);
}

int main(int argc, char const *argv[])
{
    struct binary_tree tree;

    fake_a_tree(&tree);

    in_order(&tree);

    return 0;
}
