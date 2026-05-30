/*
 * 文件: DS-Chap6/01_binary_tree_traversal.c
 * 主题: 二叉树遍历：递归遍历、非递归遍历、层次遍历、计数和深度。
 * 说明: 本文件为自包含示例，包含当前主题相关的数据结构定义、操作函数、辅助函数和演示 main。
 * 运行: 直接编译本文件即可运行，不依赖 src 目录或其他章节文件。
 * 示例: gcc -std=c11 -Wall -Wextra -pedantic DS-Chap6/01_binary_tree_traversal.c -o build/DS-Chap6/01_binary_tree_traversal
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
 * 功能：递归先序遍历二叉树。
 * 参数：
 *   T - 二叉树根结点指针。
 * 返回：无；访问结果直接输出。
 * PPT 注释：先访问根结点，再先序遍历左子树和右子树。
 */
void PreOrderTraverse(PBinTree T) {
    if (T != NULL) {
        printf("%c ", T->data);
        PreOrderTraverse(T->left);
        PreOrderTraverse(T->right);
    }
}

/*
 * 功能：递归中序遍历二叉树。
 * 参数：
 *   T - 二叉树根结点指针。
 * 返回：无；访问结果直接输出。
 * PPT 注释：中序遍历左子树，访问根结点，再中序遍历右子树。
 */
void InOrderTraverse(PBinTree T) {
    if (T != NULL) {
        InOrderTraverse(T->left);
        printf("%c ", T->data);
        InOrderTraverse(T->right);
    }
}

/*
 * 功能：递归后序遍历二叉树。
 * 参数：
 *   T - 二叉树根结点指针。
 * 返回：无；访问结果直接输出。
 * PPT 注释：后序遍历左子树和右子树，最后访问根结点。
 */
void PostOrderTraverse(PBinTree T) {
    if (T != NULL) {
        PostOrderTraverse(T->left);
        PostOrderTraverse(T->right);
        printf("%c ", T->data);
    }
}

/*
 * 功能：统计二叉树结点个数。
 * 参数：
 *   T - 二叉树根结点指针。
 * 返回：T 中结点总数；空树返回 0。
 */
int count(PBinTree T) {
    if (T == NULL) {
        return 0;
    }
    return 1 + count(T->left) + count(T->right);
}

/*
 * 功能：求二叉树深度。
 * 参数：
 *   T - 二叉树根结点指针。
 * 返回：二叉树最大层数；空树深度为 0。
 */
int depth(PBinTree T) {
    int ldepth;
    int rdepth;
    if (T == NULL) {
        return 0;
    }
    ldepth = depth(T->left);
    rdepth = depth(T->right);
    return (ldepth > rdepth ? ldepth : rdepth) + 1;
}

/*
 * 功能：非递归先序遍历二叉树。
 * 参数：
 *   T - 二叉树根结点指针。
 * 返回：无；访问结果直接输出。
 * 说明：使用顺序栈保存尚未访问的右/左子树结点。
 */
void PreOrderTraverse_NonRecursive(PBinTree T) {
    PBinTree stack[MAXNODE];
    int top = -1;
    if (T != NULL) {
        stack[++top] = T;
    }
    while (top != -1) {
        PBinTree p = stack[top--];
        printf("%c ", p->data);
        if (p->right != NULL) {
            stack[++top] = p->right;
        }
        if (p->left != NULL) {
            stack[++top] = p->left;
        }
    }
}

/*
 * 功能：非递归中序遍历二叉树。
 * 参数：
 *   T - 二叉树根结点指针。
 * 返回：无；访问结果直接输出。
 * 说明：沿左链入栈，退栈访问后转向右子树。
 */
void InOrderTraverse_NonRecursive(PBinTree T) {
    PBinTree stack[MAXNODE];
    int top = -1;
    PBinTree p = T;
    while (p != NULL || top != -1) {
        while (p != NULL) {
            stack[++top] = p;
            p = p->left;
        }
        p = stack[top--];
        printf("%c ", p->data);
        p = p->right;
    }
}

/*
 * 功能：层次遍历二叉树。
 * 参数：
 *   T - 二叉树根结点指针。
 * 返回：无；访问结果直接输出。
 * PPT 注释：借助队列逐层访问二叉树结点。
 */
void LevelOrderTraverse(PBinTree T) {
    PBinTree queue[MAXNODE];
    int front = 0;
    int rear = 0;
    if (T != NULL) {
        queue[rear++] = T;
    }
    while (front < rear) {
        PBinTree p = queue[front++];
        printf("%c ", p->data);
        if (p->left != NULL) {
            queue[rear++] = p->left;
        }
        if (p->right != NULL) {
            queue[rear++] = p->right;
        }
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

/* ===== 主题演示入口 ===== */

/*
 * 函数: main
 * 功能: 构造示例二叉树，演示递归遍历、非递归中序遍历、层序遍历、结点计数和深度计算。
 * 参数: 无。
 * 返回: 程序正常结束返回 0。
 * PPT 对应: 二叉树遍历；对照先序/中序/后序访问根结点的时机。
 */
int main(void) {
    PBinTree tree = CreateSampleTree();
    printf("PreOrder: ");
    PreOrderTraverse(tree);
    printf("\nInOrder: ");
    InOrderTraverse(tree);
    printf("\nPostOrder: ");
    PostOrderTraverse(tree);
    printf("\nPreOrder iterative: ");
    PreOrderTraverse_NonRecursive(tree);
    printf("\nInOrder iterative: ");
    InOrderTraverse_NonRecursive(tree);
    printf("\nLevelOrder: ");
    LevelOrderTraverse(tree);
    printf("\ncount=%d, depth=%d\n", count(tree), depth(tree));
    DestroyBinTree(tree);
    return 0;
}
