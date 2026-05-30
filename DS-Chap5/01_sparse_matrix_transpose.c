/*
 * 文件: DS-Chap5/01_sparse_matrix_transpose.c
 * 主题: 稀疏矩阵三元组表的普通转置和快速转置。
 * 说明: 本文件为自包含示例，包含当前主题相关的数据结构定义、操作函数、辅助函数和演示 main。
 * 运行: 直接编译本文件即可运行，不依赖 src 目录或其他章节文件。
 * 示例: gcc -std=c11 -Wall -Wextra -pedantic DS-Chap5/01_sparse_matrix_transpose.c -o build/DS-Chap5/01_sparse_matrix_transpose
 */

#include <stdio.h>
#include <stdlib.h>

#define MAXSIZE 200
#define MAXRC 20

typedef int Status;
#define OK 1
#define ERROR 0

/*
 * 结构体: Triple
 * 作用: 稀疏矩阵三元组结构：i、j 表示非零元位置，e 表示非零元值。
 */
typedef struct {
    int i;
    int j;
    double e;
} Triple;

/*
 * 结构体: TSMatrix
 * 作用: 稀疏矩阵三元组表结构：data 保存非零元，mu/nu/tu 保存行数、列数和非零元个数。
 */
typedef struct {
    Triple data[MAXSIZE];
    int mu;
    int nu;
    int tu;
} TSMatrix;

/*
 * 结构体: OLNode
 * 作用: 十字链表非零元结点结构：row/col/value 保存位置和值，right/down 连接同行和同列结点。
 */
typedef struct OLNode {
    int i;
    int j;
    double e;
    struct OLNode *right;
    struct OLNode *down;
} OLNode, *OLink;

/*
 * 结构体: CrossList
 * 作用: 十字链表矩阵结构：rhead 和 chead 分别保存每行、每列链表头指针。
 */
typedef struct {
    int mu;
    int nu;
    int tu;
    OLink rhead[MAXRC];
    OLink chead[MAXRC];
} CrossList;

typedef enum { ATOM, LIST } ElemTag;

/*
 * 结构体: GLNode
 * 作用: 广义表结点结构：tag 区分原子和子表，first 指向子表首元，next 指向同层后继。
 */
typedef struct GLNode {
    ElemTag tag;
    int atom;
    struct GLNode *first;
    struct GLNode *next;
} GLNode, *GList;

/* ===== 当前主题相关的数据结构操作函数 ===== */

/*
 * 功能：普通三元组表转置。
 * 参数：
 *   M - 输入稀疏矩阵三元组表。
 *   T - 输出矩阵，保存 M 的转置。
 * 返回：OK 表示转置完成。
 * PPT 注释：按 M 的列序扫描非零元，把行列下标交换后写入 T。
 */
Status TransposeSMatrix(TSMatrix M, TSMatrix *T) {
    T->mu = M.nu;
    T->nu = M.mu;
    T->tu = M.tu;
    int q = 0;
    for (int col = 0; col < M.nu; ++col) {
        for (int p = 0; p < M.tu; ++p) {
            if (M.data[p].j == col) {
                T->data[q].i = M.data[p].j;
                T->data[q].j = M.data[p].i;
                T->data[q].e = M.data[p].e;
                ++q;
            }
        }
    }
    return OK;
}

/*
 * 功能：快速转置稀疏矩阵三元组表。
 * 参数：
 *   M - 输入稀疏矩阵三元组表。
 *   T - 输出转置矩阵。
 * 返回：OK 表示转置完成。
 * PPT 注释：先统计每列非零元个数 num[]，再计算每列第一个非零元在 T 中的位置 cpot[]。
 */
Status FastTransposeSMatrix(TSMatrix M, TSMatrix *T) {
    int num[MAXRC] = {0};
    int cpot[MAXRC] = {0};
    T->mu = M.nu;
    T->nu = M.mu;
    T->tu = M.tu;
    if (M.tu == 0) {
        return OK;
    }
    for (int t = 0; t < M.tu; ++t) {
        ++num[M.data[t].j];
    }
    cpot[0] = 0;
    for (int col = 1; col < M.nu; ++col) {
        cpot[col] = cpot[col - 1] + num[col - 1];
    }
    for (int p = 0; p < M.tu; ++p) {
        int col = M.data[p].j;
        int q = cpot[col]++;
        T->data[q].i = M.data[p].j;
        T->data[q].j = M.data[p].i;
        T->data[q].e = M.data[p].e;
    }
    return OK;
}

/*
 * 功能：打印稀疏矩阵三元组表。
 * 参数：
 *   name - 输出标签。
 *   M    - 待打印矩阵。
 * 返回：无。
 */
static void PrintTriples(const char *name, TSMatrix M) {
    printf("%s triples: ", name);
    for (int i = 0; i < M.tu; ++i) {
        printf("(%d,%d,%.0f) ", M.data[i].i, M.data[i].j, M.data[i].e);
    }
    printf("\n");
}

/* ===== 主题演示入口 ===== */

/*
 * 函数: main
 * 功能: 构造稀疏矩阵三元组表，分别执行普通转置和快速转置并输出结果。
 * 参数: 无。
 * 返回: 程序正常结束返回 0。
 * PPT 对应: 稀疏矩阵三元组表；比较按列扫描转置与利用每列首位置数组的快速转置。
 */
int main(void) {
    TSMatrix m = {{{0, 1, 12}, {0, 3, 9}, {1, 0, -3}, {2, 2, 24}}, 3, 4, 4};
    TSMatrix t;
    TransposeSMatrix(m, &t);
    PrintTriples("Transpose", t);
    FastTransposeSMatrix(m, &t);
    PrintTriples("FastTranspose", t);
    return 0;
}
