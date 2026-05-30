/*
 * 文件: DS-Chap8/02_binary_search_tree.c
 * 主题: 二叉排序树：查找、插入、删除和中序输出。
 * 说明: 本文件为自包含示例，包含当前主题相关的数据结构定义、操作函数、辅助函数和演示 main。
 * 运行: 直接编译本文件即可运行，不依赖 src 目录或其他章节文件。
 * 示例: gcc -std=c11 -Wall -Wextra -pedantic DS-Chap8/02_binary_search_tree.c -o build/DS-Chap8/02_binary_search_tree
 */

#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0
#define MAXSIZE 100

typedef int KeyType;
typedef int DataType;
typedef int BOOL;

/*
 * 结构体: Item
 * 作用: 查找表记录结构：key 保存关键码，info 保存与关键码关联的数据。
 */
typedef struct {
    KeyType key;
    DataType info;
} Item;

/*
 * 结构体: SeqDictionary
 * 作用: 顺序查找表结构：elem 保存记录数组，n 保存当前记录个数。
 */
typedef struct {
    Item elem[MAXSIZE];
    int n;
} SeqDictionary;

/*
 * 结构体: BinNode
 * 作用: 二叉排序树结点结构：key 为关键码，data 为数据域，left/right 指向左右子树。
 */
typedef struct BinNode {
    KeyType key;
    DataType data;
    struct BinNode *left;
    struct BinNode *right;
} BinNode, *PBinTree;

/*
 * 结构体: AVLNode
 * 作用: AVL 树结点结构：key/data 保存记录，height 保存树高，left/right 指向左右子树。
 */
typedef struct AVLNode {
    KeyType key;
    DataType data;
    int height;
    struct AVLNode *left;
    struct AVLNode *right;
} AVLNode, *PAVLTree;

typedef enum { EMPTY, OCCUPIED, DELETED } HashState;

/*
 * 结构体: HashItem
 * 作用: 散列表单元结构：key 保存关键码，state 标记空闲、占用或已删除状态。
 */
typedef struct {
    KeyType key;
    HashState state;
} HashItem;

/*
 * 结构体: HashDictionary
 * 作用: 散列表结构：table 保存开放定址表，size 保存表长。
 */
typedef struct {
    HashItem table[MAXSIZE];
    int size;
} HashDictionary;

/* ===== 当前主题相关的数据结构操作函数 ===== */

/*
 * 功能：二叉排序树查找。
 * 参数：
 *   ptree    - 当前子树根指针。
 *   key      - 待查找关键码。
 *   parent   - 当前子树根的父结点。
 *   position - 输出查找结果；成功为目标结点，失败为最后访问的父结点。
 * 返回：找到返回 TRUE，否则返回 FALSE。
 */
BOOL searchBST(PBinTree ptree, KeyType key, PBinTree parent, PBinTree *position) {
    if (ptree == NULL) {
        *position = parent;
        return FALSE;
    }
    if (key == ptree->key) {
        *position = ptree;
        return TRUE;
    }
    if (key < ptree->key) {
        return searchBST(ptree->left, key, ptree, position);
    }
    return searchBST(ptree->right, key, ptree, position);
}

/*
 * 功能：向二叉排序树插入或更新结点。
 * 参数：
 *   T    - 指向当前子树根指针的指针。
 *   key  - 插入关键码。
 *   data - 数据域。
 * 返回：无；若关键码已存在则更新数据域。
 */
void InsertNode(PBinTree *T, KeyType key, DataType data) {
    if (*T == NULL) {
        *T = (PBinTree)malloc(sizeof(BinNode));
        if (*T == NULL) {
            exit(EXIT_FAILURE);
        }
        (*T)->key = key;
        (*T)->data = data;
        (*T)->left = NULL;
        (*T)->right = NULL;
        return;
    }
    if (key < (*T)->key) {
        InsertNode(&(*T)->left, key, data);
    } else if (key > (*T)->key) {
        InsertNode(&(*T)->right, key, data);
    } else {
        (*T)->data = data;
    }
}

/*
 * 功能：删除当前根结点。
 * 参数：
 *   p - 指向待删除结点指针的指针。
 * 返回：无。
 * 说明：分三种情况处理：无右子树、无左子树、左右子树均存在。
 */
static void Delete(PBinTree *p) {
    PBinTree q;
    PBinTree s;
    if ((*p)->right == NULL) {
        q = *p;
        *p = (*p)->left;
        free(q);
    } else if ((*p)->left == NULL) {
        q = *p;
        *p = (*p)->right;
        free(q);
    } else {
        q = *p;
        s = (*p)->left;
        while (s->right != NULL) {
            q = s;
            s = s->right;
        }
        (*p)->key = s->key;
        (*p)->data = s->data;
        if (q != *p) {
            q->right = s->left;
        } else {
            q->left = s->left;
        }
        free(s);
    }
}

/*
 * 功能：删除二叉排序树中关键码等于 key 的结点。
 * 参数：
 *   T   - 指向当前子树根指针的指针。
 *   key - 待删除关键码。
 * 返回：删除成功返回 TRUE，未找到返回 FALSE。
 */
BOOL DeleteBST(PBinTree *T, KeyType key) {
    if (*T == NULL) {
        return FALSE;
    }
    if (key == (*T)->key) {
        Delete(T);
        return TRUE;
    }
    if (key < (*T)->key) {
        return DeleteBST(&(*T)->left, key);
    }
    return DeleteBST(&(*T)->right, key);
}

/*
 * 功能：中序打印二叉排序树。
 * 参数：
 *   T - 二叉排序树根指针。
 * 返回：无；输出结果应为递增关键码序列。
 */
void inorderBST(PBinTree T) {
    if (T == NULL) {
        return;
    }
    inorderBST(T->left);
    printf("%d ", T->key);
    inorderBST(T->right);
}

/*
 * 功能：销毁二叉排序树。
 * 参数：
 *   T - 待释放二叉排序树根指针。
 * 返回：无。
 */
void destroyBST(PBinTree T) {
    if (T == NULL) {
        return;
    }
    destroyBST(T->left);
    destroyBST(T->right);
    free(T);
}

/* ===== 主题演示入口 ===== */

/*
 * 函数: main
 * 功能: 插入关键字构造二叉排序树，演示查找、删除和中序遍历输出。
 * 参数: 无。
 * 返回: 程序正常结束返回 0。
 * PPT 对应: 二叉排序树；中序遍历结果应保持关键字递增。
 */
int main(void) {
    PBinTree bst = NULL;
    PBinTree found = NULL;
    int keys[] = {45, 24, 53, 12, 37, 93, 28};
    for (int i = 0; i < 7; ++i) InsertNode(&bst, keys[i], keys[i] * 10);
    printf("searchBST(37): %d\n", searchBST(bst, 37, NULL, &found));
    DeleteBST(&bst, 24);
    printf("BST after delete 24: ");
    inorderBST(bst);
    printf("\n");
    destroyBST(bst);
    return 0;
}
