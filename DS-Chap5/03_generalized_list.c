/*
 * 文件: DS-Chap5/03_generalized_list.c
 * 主题: 广义表：构造、深度计算、复制和销毁。
 * 说明: 本文件为自包含示例，包含当前主题相关的数据结构定义、操作函数、辅助函数和演示 main。
 * 运行: 直接编译本文件即可运行，不依赖 src 目录或其他章节文件。
 * 示例: gcc -std=c11 -Wall -Wextra -pedantic DS-Chap5/03_generalized_list.c -o build/DS-Chap5/03_generalized_list
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

/* ===== 主题演示入口 ===== */

/*
 * 函数: main
 * 功能: 构造一个嵌套广义表，计算深度，复制广义表，并释放原表和副本。
 * 参数: 无。
 * 返回: 程序正常结束返回 0。
 * PPT 对应: 广义表的头尾链表存储；区分原子结点和子表结点。
 */
int main(void) {
    GList a = NewAtom(1);
    GList b = NewAtom(2);
    GList c = NewAtom(3);
    GList d = NewAtom(4);
    GList e = NewAtom(5);
    GList sub1 = NewList(b);
    GList sub2_inner = NewList(d);
    GList sub2 = NewList(sub2_inner);
    GList glist = NewList(a);
    GList copied = NULL;
    b->next = c;
    sub2_inner->next = e;
    a->next = sub1;
    sub1->next = sub2;
    CopyGList(&copied, glist);
    printf("GList: ");
    PrintGList(glist);
    printf(", depth=%d\nCopied: ", GListDepth(glist));
    PrintGList(copied);
    printf("\n");
    DestroyGList(glist);
    DestroyGList(copied);
    return 0;
}
