/*
 * 文件: src/DS-Chap6.c
 * 说明: 第 6 章树和二叉树公共实现：二叉树遍历、线索二叉树与 Huffman 编码。
 * 来源: 根据课程 PPT 中的代码片段整理为可运行 C11 程序。
 * 注释: 每个函数前都包含用途、参数、返回值和对应 PPT 算法说明；主题入口文件会按知识点调用这里的公共函数。
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXNODE 100

typedef char ElemType;

/*
 * 结构体: BinTreeNode
 * 作用: 二叉树结点结构：data 保存结点值，left/right 分别指向左右孩子。
 */
typedef struct BinTreeNode {
    ElemType data;
    struct BinTreeNode *left;
    struct BinTreeNode *right;
} BinTreeNode, *PBinTree;

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

typedef enum { Link, Thread } PointerTag;

/*
 * 结构体: ThrNode
 * 作用: 线索二叉树结点结构：left/right 可指向孩子或中序前驱后继，ltag/rtag 标记指针含义。
 */
typedef struct ThrNode {
    ElemType data;
    struct ThrNode *left;
    struct ThrNode *right;
    PointerTag ltag;
    PointerTag rtag;
} ThrNode, *PThrTree;

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
 * 结构体: HtNode
 * 作用: HtNode 结构：用于保存当前数据结构算法需要的相关字段。
 */
typedef struct {
    int weight;
    int parent;
    int left;
    int right;
} HtNode;

/*
 * 功能：在 Huffman 树构造过程中选择两个最小权值根结点。
 * 参数：
 *   ht  - Huffman 结点数组。
 *   pos - 当前已使用的数组上界，只在 ht[0..pos-1] 中选择。
 *   x1  - 输出第一个最小结点下标。
 *   x2  - 输出第二个最小结点下标。
 * 返回：无。
 */
void Select(HtNode ht[], int pos, int *x1, int *x2) {
    *x1 = -1;
    *x2 = -1;
    for (int i = 0; i < pos; ++i) {
        if (ht[i].parent != -1) {
            continue;
        }
        if (*x1 == -1 || ht[i].weight < ht[*x1].weight) {
            *x2 = *x1;
            *x1 = i;
        } else if (*x2 == -1 || ht[i].weight < ht[*x2].weight) {
            *x2 = i;
        }
    }
}

/*
 * 功能：根据权值数组生成 Huffman 编码。
 * 参数：
 *   weights - 叶子结点权值数组。
 *   n       - 叶子结点个数。
 *   codes   - 输出编码字符串数组，每行保存一个叶子的 0/1 编码。
 * 返回：无。
 * PPT 注释：构造 Huffman 树后，由叶子向根逆向求编码，再反转得到最终编码。
 */
void HuffmanCoding(const int weights[], int n, char codes[][MAXNODE]) {
    HtNode ht[2 * MAXNODE];
    for (int i = 0; i < 2 * n - 1; ++i) {
        ht[i].weight = 0;
        ht[i].parent = -1;
        ht[i].left = -1;
        ht[i].right = -1;
    }
    for (int i = 0; i < n; ++i) {
        ht[i].weight = weights[i];
    }
    for (int i = n; i < 2 * n - 1; ++i) {
        int x1;
        int x2;
        Select(ht, i, &x1, &x2);
        ht[x1].parent = i;
        ht[x2].parent = i;
        ht[i].left = x1;
        ht[i].right = x2;
        ht[i].weight = ht[x1].weight + ht[x2].weight;
    }
    for (int i = 0; i < n; ++i) {
        char temp[MAXNODE];
        int len = 0;
        int child = i;
        int parent = ht[child].parent;
        while (parent != -1) {
            temp[len++] = (ht[parent].left == child) ? '0' : '1';
            child = parent;
            parent = ht[child].parent;
        }
        for (int j = 0; j < len; ++j) {
            codes[i][j] = temp[len - j - 1];
        }
        codes[i][len] = '\0';
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

/*
 * 功能：第 6 章树和二叉树算法演示入口。
 * 参数：无。
 * 返回：0 表示程序正常结束。
 * 说明：演示二叉树遍历、结点统计、深度、中序线索化和 Huffman 编码。
 */
int main(void) {
    PBinTree tree = CreateSampleTree();
    PThrTree thread_tree;
    PThrTree pre = NULL;
    int weights[] = {5, 29, 7, 8, 14, 23, 3, 11};
    int n = (int)(sizeof(weights) / sizeof(weights[0]));
    char codes[8][MAXNODE];

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

    thread_tree = CopyToThreadTree(tree);
    InThreading(thread_tree, &pre);
    printf("Threaded inorder: ");
    InOrderTraverse_Thr(thread_tree);
    printf("\n");

    HuffmanCoding(weights, n, codes);
    printf("Huffman codes: ");
    for (int i = 0; i < n; ++i) {
        printf("%d=%s ", weights[i], codes[i]);
    }
    printf("\n");

    DestroyThreadTree(thread_tree);
    DestroyBinTree(tree);
    return 0;
}
