/*
 * 文件: DS-Chap6/03_huffman.c
 * 主题: Huffman 树构造和 Huffman 编码。
 * 说明: 本文件为自包含示例，包含当前主题相关的数据结构定义、操作函数、辅助函数和演示 main。
 * 运行: 直接编译本文件即可运行，不依赖 src 目录或其他章节文件。
 * 示例: gcc -std=c11 -Wall -Wextra -pedantic DS-Chap6/03_huffman.c -o build/DS-Chap6/03_huffman
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
 * 结构体: HtNode
 * 作用: HtNode 结构：用于保存当前数据结构算法需要的相关字段。
 */
typedef struct {
    int weight;
    int parent;
    int left;
    int right;
} HtNode;

/* ===== 当前主题相关的数据结构操作函数 ===== */

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

/* ===== 主题演示入口 ===== */

/*
 * 函数: main
 * 功能: 根据一组权值构造 Huffman 树，生成并打印每个权值对应的 Huffman 编码。
 * 参数: 无。
 * 返回: 程序正常结束返回 0。
 * PPT 对应: Huffman 树和编码；观察每轮选取两个最小权值结点合并的过程。
 */
int main(void) {
    int weights[] = {5, 29, 7, 8, 14, 23, 3, 11};
    int n = (int)(sizeof(weights) / sizeof(weights[0]));
    char codes[8][MAXNODE];
    HuffmanCoding(weights, n, codes);
    printf("Huffman codes: ");
    for (int i = 0; i < n; ++i) printf("%d=%s ", weights[i], codes[i]);
    printf("\n");
    return 0;
}
