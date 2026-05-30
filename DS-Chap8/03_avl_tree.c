/*
 * 文件: DS-Chap8/03_avl_tree.c
 * 主题: AVL 树：插入和 LL/RR/LR/RL 平衡调整。
 * 说明: 本文件为自包含示例，包含当前主题相关的数据结构定义、操作函数、辅助函数和演示 main。
 * 运行: 直接编译本文件即可运行，不依赖 src 目录或其他章节文件。
 * 示例: gcc -std=c11 -Wall -Wextra -pedantic DS-Chap8/03_avl_tree.c -o build/DS-Chap8/03_avl_tree
 */

#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0
#define MAXSIZE 100

typedef int KeyType;
typedef int DataType;
typedef int BOOL;

typedef struct {
    KeyType key;
    DataType info;
} Item;

typedef struct {
    Item elem[MAXSIZE];
    int n;
} SeqDictionary;

typedef struct BinNode {
    KeyType key;
    DataType data;
    struct BinNode *left;
    struct BinNode *right;
} BinNode, *PBinTree;

typedef struct AVLNode {
    KeyType key;
    DataType data;
    int height;
    struct AVLNode *left;
    struct AVLNode *right;
} AVLNode, *PAVLTree;

typedef enum { EMPTY, OCCUPIED, DELETED } HashState;

typedef struct {
    KeyType key;
    HashState state;
} HashItem;

typedef struct {
    HashItem table[MAXSIZE];
    int size;
} HashDictionary;

/* ===== 当前主题相关的数据结构操作函数 ===== */

/*
 * 功能：取得 AVL 结点高度。
 * 参数：
 *   T - AVL 结点指针。
 * 返回：空结点高度为 0，非空返回其 height 字段。
 */
static int height(PAVLTree T) {
    return T == NULL ? 0 : T->height;
}

/*
 * 功能：返回两个整数中的较大值。
 * 参数：
 *   a, b - 待比较整数。
 * 返回：较大者。
 */
static int max_int(int a, int b) {
    return a > b ? a : b;
}

/*
 * 功能：根据左右子树高度更新 AVL 结点高度。
 * 参数：
 *   T - 待更新结点。
 * 返回：无。
 */
static void updateHeight(PAVLTree T) {
    T->height = max_int(height(T->left), height(T->right)) + 1;
}

/*
 * 功能：AVL 树 LL 型调整，即右旋。
 * 参数：
 *   a - 失衡子树根结点。
 * 返回：旋转后的新根结点。
 */
PAVLTree LL(PAVLTree a) {
    PAVLTree b = a->left;
    a->left = b->right;
    b->right = a;
    updateHeight(a);
    updateHeight(b);
    return b;
}

/*
 * 功能：AVL 树 RR 型调整，即左旋。
 * 参数：
 *   a - 失衡子树根结点。
 * 返回：旋转后的新根结点。
 */
PAVLTree RR(PAVLTree a) {
    PAVLTree b = a->right;
    a->right = b->left;
    b->left = a;
    updateHeight(a);
    updateHeight(b);
    return b;
}

/*
 * 功能：AVL 树 LR 型调整。
 * 参数：
 *   a - 失衡子树根结点。
 * 返回：调整后的新根。
 * PPT 注释：先对左子树 RR，再对根 LL。
 */
PAVLTree LR(PAVLTree a) {
    a->left = RR(a->left);
    return LL(a);
}

/*
 * 功能：AVL 树 RL 型调整。
 * 参数：
 *   a - 失衡子树根结点。
 * 返回：调整后的新根。
 * PPT 注释：先对右子树 LL，再对根 RR。
 */
PAVLTree RL(PAVLTree a) {
    a->right = LL(a->right);
    return RR(a);
}

/*
 * 功能：向 AVL 树插入关键码，并在回溯时保持平衡。
 * 参数：
 *   T    - 当前子树根结点。
 *   key  - 插入关键码。
 *   data - 数据域。
 * 返回：插入并调整后的当前子树根。
 */
PAVLTree InsertAVL(PAVLTree T, KeyType key, DataType data) {
    if (T == NULL) {
        T = (PAVLTree)malloc(sizeof(AVLNode));
        if (T == NULL) {
            exit(EXIT_FAILURE);
        }
        T->key = key;
        T->data = data;
        T->height = 1;
        T->left = NULL;
        T->right = NULL;
        return T;
    }
    if (key < T->key) {
        T->left = InsertAVL(T->left, key, data);
        if (height(T->left) - height(T->right) == 2) {
            T = key < T->left->key ? LL(T) : LR(T);
        }
    } else if (key > T->key) {
        T->right = InsertAVL(T->right, key, data);
        if (height(T->right) - height(T->left) == 2) {
            T = key > T->right->key ? RR(T) : RL(T);
        }
    } else {
        T->data = data;
    }
    updateHeight(T);
    return T;
}

/*
 * 功能：中序打印 AVL 树。
 * 参数：
 *   T - AVL 树根指针。
 * 返回：无。
 */
void inorderAVL(PAVLTree T) {
    if (T == NULL) {
        return;
    }
    inorderAVL(T->left);
    printf("%d ", T->key);
    inorderAVL(T->right);
}

/*
 * 功能：销毁 AVL 树。
 * 参数：
 *   T - 待释放 AVL 树根指针。
 * 返回：无。
 */
void destroyAVL(PAVLTree T) {
    if (T == NULL) {
        return;
    }
    destroyAVL(T->left);
    destroyAVL(T->right);
    free(T);
}

/* ===== 主题演示入口 ===== */

/*
 * 函数: main
 * 功能: 按指定序列插入 AVL 树，触发平衡调整后输出中序序列、根结点和树高。
 * 参数: 无。
 * 返回: 程序正常结束返回 0。
 * PPT 对应: 平衡二叉树；通过 LL、RR、LR、RL 旋转恢复左右子树高度差不超过 1。
 */
int main(void) {
    PAVLTree avl = NULL;
    int keys[] = {30, 20, 40, 10, 25, 22, 50};
    for (int i = 0; i < 7; ++i) avl = InsertAVL(avl, keys[i], keys[i]);
    printf("AVL inorder: ");
    inorderAVL(avl);
    printf("\nAVL root=%d, height=%d\n", avl->key, avl->height);
    destroyAVL(avl);
    return 0;
}
