/*
 * 文件: DS-Chap5/02_matrix_multiply_crosslist.c
 * 主题: 稀疏矩阵乘法和十字链表表示。
 * 说明: 本文件为自包含示例，包含当前主题相关的数据结构定义、操作函数、辅助函数和演示 main。
 * 运行: 直接编译本文件即可运行，不依赖 src 目录或其他章节文件。
 * 示例: gcc -std=c11 -Wall -Wextra -pedantic DS-Chap5/02_matrix_multiply_crosslist.c -o build/DS-Chap5/02_matrix_multiply_crosslist
 */

#include <stdio.h>
#include <stdlib.h>

#define MAXSIZE 200
#define MAXRC 20

typedef int Status;
#define OK 1
#define ERROR 0

typedef struct {
    int i;
    int j;
    double e;
} Triple;

typedef struct {
    Triple data[MAXSIZE];
    int mu;
    int nu;
    int tu;
} TSMatrix;

typedef struct OLNode {
    int i;
    int j;
    double e;
    struct OLNode *right;
    struct OLNode *down;
} OLNode, *OLink;

typedef struct {
    int mu;
    int nu;
    int tu;
    OLink rhead[MAXRC];
    OLink chead[MAXRC];
} CrossList;

typedef enum { ATOM, LIST } ElemTag;

typedef struct GLNode {
    ElemTag tag;
    int atom;
    struct GLNode *first;
    struct GLNode *next;
} GLNode, *GList;

/* ===== 当前主题相关的数据结构操作函数 ===== */

/*
 * 功能：把三元组表转换为二维数组表示。
 * 参数：
 *   M     - 输入稀疏矩阵。
 *   dense - 输出二维数组，函数会先把矩阵范围内元素清零。
 * 返回：无。
 */
static void toDense(TSMatrix M, double dense[MAXRC][MAXRC]) {
    for (int i = 0; i < M.mu; ++i) {
        for (int j = 0; j < M.nu; ++j) {
            dense[i][j] = 0.0;
        }
    }
    for (int k = 0; k < M.tu; ++k) {
        dense[M.data[k].i][M.data[k].j] = M.data[k].e;
    }
}

/*
 * 功能：把二维数组中的非零元压缩为三元组表。
 * 参数：
 *   dense - 输入二维数组。
 *   rows  - 行数。
 *   cols  - 列数。
 *   M     - 输出三元组表。
 * 返回：无。
 */
static void fromDense(double dense[MAXRC][MAXRC], int rows, int cols, TSMatrix *M) {
    M->mu = rows;
    M->nu = cols;
    M->tu = 0;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (dense[i][j] != 0.0) {
                M->data[M->tu++] = (Triple){i, j, dense[i][j]};
            }
        }
    }
}

/*
 * 功能：稀疏矩阵乘法 Q = M * N。
 * 参数：
 *   M - 左矩阵。
 *   N - 右矩阵。
 *   Q - 输出乘积矩阵。
 * 返回：维度匹配返回 OK，否则返回 ERROR。
 * PPT 注释：若 M.nu != N.mu，则矩阵不能相乘；本实现用稠密数组完成演示计算。
 */
Status MultiSMatrix(TSMatrix M, TSMatrix N, TSMatrix *Q) {
    double a[MAXRC][MAXRC];
    double b[MAXRC][MAXRC];
    double c[MAXRC][MAXRC] = {{0.0}};
    if (M.nu != N.mu) {
        return ERROR;
    }
    toDense(M, a);
    toDense(N, b);
    for (int i = 0; i < M.mu; ++i) {
        for (int k = 0; k < M.nu; ++k) {
            if (a[i][k] == 0.0) {
                continue;
            }
            for (int j = 0; j < N.nu; ++j) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    fromDense(c, M.mu, N.nu, Q);
    return OK;
}

/*
 * 功能：由三元组表创建稀疏矩阵十字链表。
 * 参数：
 *   M      - 输出十字链表结构。
 *   source - 输入三元组表。
 * 返回：OK 表示创建完成。
 * PPT 注释：输入各非零元并构造十字链表，同时维护行头指针和列头指针。
 */
Status CreateOLSMatrix(CrossList *M, TSMatrix source) {
    M->mu = source.mu;
    M->nu = source.nu;
    M->tu = source.tu;
    for (int i = 0; i < MAXRC; ++i) {
        M->rhead[i] = NULL;
        M->chead[i] = NULL;
    }
    for (int k = 0; k < source.tu; ++k) {
        OLNode *node = (OLNode *)malloc(sizeof(OLNode));
        if (node == NULL) {
            exit(EXIT_FAILURE);
        }
        node->i = source.data[k].i;
        node->j = source.data[k].j;
        node->e = source.data[k].e;

        OLink *row_link = &M->rhead[node->i];
        while (*row_link != NULL && (*row_link)->j < node->j) {
            row_link = &(*row_link)->right;
        }
        node->right = *row_link;
        *row_link = node;

        OLink *col_link = &M->chead[node->j];
        while (*col_link != NULL && (*col_link)->i < node->i) {
            col_link = &(*col_link)->down;
        }
        node->down = *col_link;
        *col_link = node;
    }
    return OK;
}

/*
 * 功能：销毁十字链表并释放所有非零元结点。
 * 参数：
 *   M - 待销毁的十字链表。
 * 返回：无。
 * 说明：沿行链释放即可，每个结点只释放一次。
 */
void DestroyCrossList(CrossList *M) {
    for (int i = 0; i < M->mu; ++i) {
        OLink p = M->rhead[i];
        while (p != NULL) {
            OLink next = p->right;
            free(p);
            p = next;
        }
        M->rhead[i] = NULL;
    }
}

/*
 * 功能：以二维矩阵形式打印稀疏矩阵。
 * 参数：
 *   name - 输出标签。
 *   M    - 待打印矩阵。
 * 返回：无。
 */
static void PrintDense(const char *name, TSMatrix M) {
    double dense[MAXRC][MAXRC];
    toDense(M, dense);
    printf("%s dense:\n", name);
    for (int i = 0; i < M.mu; ++i) {
        for (int j = 0; j < M.nu; ++j) {
            printf("%5.0f", dense[i][j]);
        }
        printf("\n");
    }
}

/* ===== 主题演示入口 ===== */

/*
 * 函数: main
 * 功能: 演示稀疏矩阵乘法，并把矩阵转换为十字链表后输出非零元位置和值。
 * 参数: 无。
 * 返回: 程序正常结束返回 0。
 * PPT 对应: 稀疏矩阵乘法与十字链表；理解行链、列链如何共享同一个非零元结点。
 */
int main(void) {
    TSMatrix m = {{{0, 1, 12}, {0, 3, 9}, {1, 0, -3}, {2, 2, 24}}, 3, 4, 4};
    TSMatrix n = {{{0, 0, 2}, {1, 1, 3}, {2, 0, 4}, {3, 1, 5}}, 4, 2, 4};
    TSMatrix q;
    CrossList cross;
    MultiSMatrix(m, n, &q);
    PrintDense("M * N", q);
    CreateOLSMatrix(&cross, m);
    printf("CrossList row 0: ");
    for (OLink p = cross.rhead[0]; p != NULL; p = p->right) {
        printf("(%d,%d,%.0f) ", p->i, p->j, p->e);
    }
    printf("\n");
    DestroyCrossList(&cross);
    return 0;
}
