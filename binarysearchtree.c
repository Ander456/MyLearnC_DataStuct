#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef int mytype;

typedef struct _bstree_node
{
    mytype data;
    struct _bstree_node *lchild;
    struct _bstree_node *rchild;
} bstree_node;

typedef struct _bstree
{
    int size;
    int (*compare)(mytype key1, mytype key2);
    int (*destroy)(mytype data);
    bstree_node *root;
} bstree;

typedef int (*compare_fuc)(mytype key1, mytype key2);
typedef int (*destroy_fuc)(mytype data);

#define bstree_is_empty(tree) (tree->size == 0)

bstree *bstree_create(compare_fuc compare, destroy_fuc destroy);

bstree *bstree_create(compare_fuc compare, destroy_fuc destroy)
{
    bstree *tree = NULL;
    tree = malloc(sizeof(bstree));
    if (tree == NULL)
        return NULL;

    tree->size = 0;
    tree->compare = compare;
    tree->destroy = destroy;
    tree->root = NULL;
    return tree;
}

bstree_node *bstree_search(bstree *tree, mytype data)
{
    bstree_node *node = NULL;
    int res = 0;
    if (tree == NULL || bstree_is_empty(tree))
        return NULL;
    node = tree->root;

    while (node != NULL)
    {
        res = tree->compare(data, node->data);
        if (res == 0)
            return node;
        else if (res > 0)
            node = node->rchild;
        else
            node = node->lchild;
    }

    return NULL;
}

int bstree_insert(bstree *tree, mytype data)
{
    bstree_node *node = NULL;
    bstree_node *tmp = NULL;
    int res = 0;
    if (tree == NULL)
        return -1;
    node = malloc(sizeof(bstree_node));
    if (node == NULL)
        return -2;
    node->data = data;
    node->lchild = NULL;
    node->rchild = NULL;

    //如果二叉树为空 直接挂到根节点
    if (bstree_is_empty(tree))
    {
        tree->root = node;
        tree->size++;
        return 0;
    }

    tmp = tree->root;

    while (tmp != NULL)
    {
        res = tree->compare(data, tmp->data);
        if (res > 0)
        {
            if (tmp->rchild == NULL)
            {
                tmp->rchild = node;
                tree->size++;
                return 0;
            }
            tmp = tmp->rchild;
        }
        else
        {
            if (tmp->lchild == NULL)
            {
                tmp->lchild = node;
                return 0;
            }
            tmp = tmp->lchild;
        }
    }
    return -3;
}

int bstree_delete(bstree *tree, mytype data)
{
    bstree_node *node = NULL;  //要删除的节点
    bstree_node *pnode = NULL; //要删除的节点的父节点
    bstree_node *minnode = NULL;
    bstree_node *pminnode = NULL;

    mytype tmp = 0;
    int res = 0;

    if (tree == NULL || bstree_is_empty(tree))
        return -1;

    node = tree->root;

    while (node != NULL && (res = tree->compare(data, node->data) != 0))
    {
        pnode = node;
        if (res > 0)
            node = node->rchild;
        else
            node = node->lchild;
    }

    //说明要删除的节点不存在
    if (node == NULL)
    {
        return -2;
    }

    /*1. 如果要删除的node有2个节点需要找到右子树最小节点minnode
        再更新minnode和node的节点数据 这样minnode节点就是要删除的节点
        再更新node和pnode节点指向要删除的节点
    */
    if (node->lchild != NULL && node->rchild != NULL)
    {
        minnode = node->rchild;
        pminnode = node;

        while (minnode->lchild != NULL)
        {
            pminnode = minnode;
            minnode = minnode->lchild;
        }

        //node节点和minode节点数据互换
        tmp = node->data;
        node->data = minnode->data;
        minnode->data = tmp;
        node = minnode;
        pnode = pminnode;
    }

    //2. 当前要删除的节点只有左孩子或者右孩子时，直接父节点指向删除的节点
    if (node->lchild != NULL)
        minnode = node->lchild;
    else if (node->rchild != NULL)
        minnode = node->rchild;
    else
        minnode = NULL;

    if (pnode == NULL) //当前要删除的是根节点
        tree->root = minnode;
    else if (pnode->lchild == node)
        pnode->lchild = minnode;
    else
        pnode->rchild = minnode;

    tree->size--;
    free(node);
    return 0;
}

void bstree_destroy_node(bstree *tree, bstree_node *root)
{
    if (root == NULL)
        return;
    bstree_destroy_node(tree, root->lchild);
    bstree_destroy_node(tree, root->rchild);
    free(root);
}

void bstree_destroy(bstree *tree)
{
    bstree_destroy_node(tree, tree->root);
    free(tree);
    return;
}

void bstree_inorder_node(bstree_node *root)
{
    if (root == NULL)
        return;

    bstree_inorder_node(root->lchild);
    printf(" %d ", root->data);
    bstree_inorder_node(root->rchild);
    return;
}

void bstree_dump(bstree *tree)
{
    if (tree == NULL || bstree_is_empty(tree))
        printf("\r\n 当前的树是空树");
    printf("\r\nSTART-----------------%d------------\r\n", tree->size);
    bstree_inorder_node(tree->root);
    printf("\r\nEND---------------------------------");
}

int bstree_compare(mytype key1, mytype key2)
{
    if (key1 == key2)
        return 0;
    else if (key1 > key2)
        return 1;
    else
        return -1;
}

int main(int argc, char const *argv[])
{
    bstree *tree = NULL;
    bstree_node *node = NULL;
    mytype data = 0;
    int res = 0;

    tree = bstree_create(bstree_compare, NULL);
    assert(tree != NULL);

    while (1)
    {
        printf("\r\n 插入一个数字， 输入100时退出：");
        scanf("%d", &data);
        if (data == 100)
            break;
        res = bstree_insert(tree, data);
        printf("\r\n %d 插入 %s 成功", data, res != 0 ? "不" : "");
    }

    bstree_dump(tree);

    while (1)
    {
        printf("\r\n删除一个数字，输入100时退出：");
        scanf("%d", &data);
        if (data == 100)
            break;
        res = bstree_delete(tree, data);
        printf("\r\n %d 删除%s成功", data, (res != 0) ? ("不") : (" "));
        bstree_dump(tree);
    }

    bstree_destroy(tree);

    return 0;
}
