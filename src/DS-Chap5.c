/*
 * 文件: src/DS-Chap5.c
 * 说明: 第 5 章数组和广义表公共实现：稀疏矩阵、十字链表与广义表。
 * 来源: 根据课程 PPT 中的代码片段整理为可运行 C11 程序。
 * 注释: 每个函数前都包含用途、参数、返回值和对应 PPT 算法说明；主题入口文件会按知识点调用这里的公共函数。
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

/*
 * 功能：创建广义表原子结点。
 * 参数：
 *   value - 原子值。
 * 返回：新建原子结点指针。
 */
GList NewAtom(int value) {
    GList node = (GList)malloc(sizeof(GLNode));
    if (node == NULL) {
        exit(EXIT_FAILURE);
    }
    node->tag = ATOM;
    node->atom = value;
    node->first = NULL;
    node->next = NULL;
    return node;
}

/*
 * 功能：创建广义表表结点。
 * 参数：
 *   first - 表头指针，指向该子表第一个元素。
 * 返回：新建表结点指针。
 */
GList NewList(GList first) {
    GList node = (GList)malloc(sizeof(GLNode));
    if (node == NULL) {
        exit(EXIT_FAILURE);
    }
    node->tag = LIST;
    node->atom = 0;
    node->first = first;
    node->next = NULL;
    return node;
}

/*
 * 功能：计算广义表深度。
 * 参数：
 *   L - 广义表或原子结点指针。
 * 返回：原子深度为 0，空表深度为 1，非空表深度为子表最大深度加 1。
 * PPT 对应：GListDepth(GList L)。
 */
int GListDepth(GList L) {
    if (L == NULL) {
        return 1;
    }
    if (L->tag == ATOM) {
        return 0;
    }
    int max_depth = 0;
    for (GList p = L->first; p != NULL; p = p->next) {
        int d = GListDepth(p);
        if (d > max_depth) {
            max_depth = d;
        }
    }
    return max_depth + 1;
}

/*
 * 功能：复制广义表。
 * 参数：
 *   T - 输出参数，保存新复制出的广义表指针。
 *   L - 输入广义表。
 * 返回：OK 表示复制完成。
 * PPT 对应：CopyGList(T, L)，递归复制 first 和 next 链。
 */
Status CopyGList(GList *T, GList L) {
    if (L == NULL) {
        *T = NULL;
        return OK;
    }
    *T = (GList)malloc(sizeof(GLNode));
    if (*T == NULL) {
        exit(EXIT_FAILURE);
    }
    (*T)->tag = L->tag;
    (*T)->atom = L->atom;
    (*T)->first = NULL;
    (*T)->next = NULL;
    if (L->tag == LIST) {
        CopyGList(&(*T)->first, L->first);
    }
    CopyGList(&(*T)->next, L->next);
    return OK;
}

/*
 * 功能：按广义表括号表示法打印广义表。
 * 参数：
 *   L - 待打印广义表。
 * 返回：无；直接输出到标准输出。
 */
void PrintGList(GList L) {
    if (L == NULL) {
        printf("()");
        return;
    }
    if (L->tag == ATOM) {
        printf("%d", L->atom);
        return;
    }
    printf("(");
    for (GList p = L->first; p != NULL; p = p->next) {
        if (p != L->first) {
            printf(",");
        }
        PrintGList(p);
    }
    printf(")");
}

/*
 * 功能：销毁广义表并释放所有结点。
 * 参数：
 *   L - 待销毁广义表。
 * 返回：无。
 */
void DestroyGList(GList L) {
    if (L == NULL) {
        return;
    }
    if (L->tag == LIST) {
        DestroyGList(L->first);
    }
    DestroyGList(L->next);
    free(L);
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

/*
 * 功能：第 5 章稀疏矩阵和广义表演示入口。
 * 参数：无。
 * 返回：0 表示程序正常结束。
 * 说明：演示三元组转置、快速转置、矩阵乘法、十字链表和广义表复制。
 */
int main(void) {
    TSMatrix M = {{{0, 1, 12}, {0, 3, 9}, {1, 0, -3}, {2, 2, 24}}, 3, 4, 4};
    TSMatrix N = {{{0, 0, 2}, {1, 1, 3}, {2, 0, 4}, {3, 1, 5}}, 4, 2, 4};
    TSMatrix T;
    TSMatrix Q;
    CrossList cross;

    TransposeSMatrix(M, &T);
    PrintTriples("Transpose", T);
    FastTransposeSMatrix(M, &T);
    PrintTriples("FastTranspose", T);
    MultiSMatrix(M, N, &Q);
    PrintDense("M * N", Q);

    CreateOLSMatrix(&cross, M);
    printf("CrossList row 0: ");
    for (OLink p = cross.rhead[0]; p != NULL; p = p->right) {
        printf("(%d,%d,%.0f) ", p->i, p->j, p->e);
    }
    printf("\n");

    GList a = NewAtom(1);
    GList b = NewAtom(2);
    GList c = NewAtom(3);
    GList d = NewAtom(4);
    GList e = NewAtom(5);
    GList sub1 = NewList(b);
    b->next = c;
    GList sub2_inner = NewList(d);
    GList sub2 = NewList(sub2_inner);
    sub2_inner->next = e;
    GList glist = NewList(a);
    a->next = sub1;
    sub1->next = sub2;
    GList copied = NULL;
    CopyGList(&copied, glist);
    printf("GList: ");
    PrintGList(glist);
    printf(", depth=%d\nCopied GList: ", GListDepth(glist));
    PrintGList(copied);
    printf("\n");

    DestroyCrossList(&cross);
    DestroyGList(glist);
    DestroyGList(copied);
    return 0;
}
