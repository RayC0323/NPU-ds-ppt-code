/*
 * 文件: DS-Chap6/02_threaded_binary_tree.c
 * 主题: 中序线索二叉树：线索化和线索遍历。
 * 说明: 本文件为自包含示例，包含当前主题相关的数据结构定义、操作函数、辅助函数和演示 main。
 * 运行: 直接编译本文件即可运行，不依赖 src 目录或其他章节文件。
 * 示例: gcc -std=c11 -Wall -Wextra -pedantic DS-Chap6/02_threaded_binary_tree.c -o build/DS-Chap6/02_threaded_binary_tree
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXNODE 100

typedef char ElemType;

typedef struct BinTreeNode {
    ElemType data;
    struct BinTreeNode *left;
    struct BinTreeNode *right;
} BinTreeNode, *PBinTree;

typedef enum { Link, Thread } PointerTag;

typedef struct ThrNode {
    ElemType data;
    struct ThrNode *left;
    struct ThrNode *right;
    PointerTag ltag;
    PointerTag rtag;
} ThrNode, *PThrTree;

typedef struct {
    int weight;
    int parent;
    int left;
    int right;
} HtNode;

/* ===== 当前主题相关的数据结构操作函数 ===== */

/*
 * 功能：创建一个二叉树结点。
 * 参数：
 *   value - 结点数据域。
 *   left  - 左孩子指针。
 *   right - 右孩子指针。
 * 返回：新结点指针；分配失败时程序退出。
 */
PBinTree NewNode(ElemType value, PBinTree left, PBinTree right) {
    PBinTree node = (PBinTree)malloc(sizeof(BinTreeNode));
    if (node == NULL) {
        exit(EXIT_FAILURE);
    }
    node->data = value;
    node->left = left;
    node->right = right;
    return node;
}

/*
 * 功能：构造用于演示遍历算法的样例二叉树。
 * 参数：无。
 * 返回：样例树根指针，结构为 A(B(D,E),C(,F))。
 */
PBinTree CreateSampleTree(void) {
    PBinTree d = NewNode('D', NULL, NULL);
    PBinTree e = NewNode('E', NULL, NULL);
    PBinTree f = NewNode('F', NULL, NULL);
    PBinTree b = NewNode('B', d, e);
    PBinTree c = NewNode('C', NULL, f);
    return NewNode('A', b, c);
}

/*
 * 功能：把普通二叉树复制成线索二叉树结点结构。
 * 参数：
 *   T - 普通二叉树根指针。
 * 返回：复制后的线索二叉树根指针；空树返回 NULL。
 */
PThrTree CopyToThreadTree(PBinTree T) {
    if (T == NULL) {
        return NULL;
    }
    PThrTree node = (PThrTree)malloc(sizeof(ThrNode));
    if (node == NULL) {
        exit(EXIT_FAILURE);
    }
    node->data = T->data;
    node->ltag = Link;
    node->rtag = Link;
    node->left = CopyToThreadTree(T->left);
    node->right = CopyToThreadTree(T->right);
    return node;
}

/*
 * 功能：按中序遍历对二叉树进行线索化。
 * 参数：
 *   p   - 当前访问的线索树结点。
 *   pre - 指向“刚刚访问过的前驱结点指针”的指针。
 * 返回：无。
 * PPT 注释：若左子树为空建立前驱线索；若前驱右子树为空建立后继线索。
 */
void InThreading(PThrTree p, PThrTree *pre) {
    if (p == NULL) {
        return;
    }
    InThreading(p->left, pre);
    if (p->left == NULL) {
        p->ltag = Thread;
        p->left = *pre;
    }
    if (*pre != NULL && (*pre)->right == NULL) {
        (*pre)->rtag = Thread;
        (*pre)->right = p;
    }
    *pre = p;
    InThreading(p->right, pre);
}

/*
 * 功能：遍历中序线索二叉树。
 * 参数：
 *   T - 已经完成中序线索化的二叉树根指针。
 * 返回：无；访问结果直接输出。
 * 说明：遇到 Thread 标记时沿线索寻找后继，避免递归或栈。
 */
void InOrderTraverse_Thr(PThrTree T) {
    PThrTree p = T;
    while (p != NULL) {
        while (p->ltag == Link && p->left != NULL) {
            p = p->left;
        }
        printf("%c ", p->data);
        while (p->rtag == Thread) {
            p = p->right;
            if (p == NULL) {
                return;
            }
            printf("%c ", p->data);
        }
        p = p->right;
    }
}

/*
 * 功能：销毁普通二叉树。
 * 参数：
 *   T - 待销毁二叉树根指针。
 * 返回：无。
 */
void DestroyBinTree(PBinTree T) {
    if (T == NULL) {
        return;
    }
    DestroyBinTree(T->left);
    DestroyBinTree(T->right);
    free(T);
}

/*
 * 功能：销毁线索二叉树。
 * 参数：
 *   T - 待销毁线索二叉树根指针。
 * 返回：无。
 * 说明：只沿 Link 指针递归释放，避免沿线索重复释放结点。
 */
void DestroyThreadTree(PThrTree T) {
    if (T == NULL) {
        return;
    }
    if (T->ltag == Link) {
        DestroyThreadTree(T->left);
    }
    if (T->rtag == Link) {
        DestroyThreadTree(T->right);
    }
    free(T);
}

/* ===== 主题演示入口 ===== */

/*
 * 函数: main
 * 功能: 将普通二叉树复制为线索二叉树，按中序规则线索化并用线索进行中序遍历。
 * 参数: 无。
 * 返回: 程序正常结束返回 0。
 * PPT 对应: 中序线索二叉树；理解空左/右指针如何改造成前驱/后继线索。
 */
int main(void) {
    PBinTree tree = CreateSampleTree();
    PThrTree thread_tree = CopyToThreadTree(tree);
    PThrTree pre = NULL;
    InThreading(thread_tree, &pre);
    printf("Threaded inorder: ");
    InOrderTraverse_Thr(thread_tree);
    printf("\n");
    DestroyThreadTree(thread_tree);
    DestroyBinTree(tree);
    return 0;
}
