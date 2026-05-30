/*
 * 文件: DS-Chap2/07_josephus_and_polynomial.c
 * 主题: Josephus 环和一元多项式加法。
 * 说明: 本文件为自包含示例，包含当前主题相关的数据结构定义、操作函数、辅助函数和演示 main。
 * 运行: 直接编译本文件即可运行，不依赖 src 目录或其他章节文件。
 * 示例: gcc -std=c11 -Wall -Wextra -pedantic DS-Chap2/07_josephus_and_polynomial.c -o build/DS-Chap2/07_josephus_and_polynomial
 */

#include <stdio.h>
#include <stdlib.h>

#define MAXNUM 100
#define TRUE 1
#define FALSE 0
#define SPECIAL 2147483647

typedef int DataType;

/*
 * 结构体: SeqList
 * 作用: 顺序表结构：用定长数组保存线性表元素，length 记录当前有效元素个数。
 */
typedef struct SeqList {
    DataType element[MAXNUM];
    int length;  /* length < MAXNUM */
} SeqList, *PSeqList;

/*
 * 结构体: Node
 * 作用: 单链表结点结构：info 保存数据域，next 指向后继结点。
 */
typedef struct Node {
    DataType info;
    struct Node *next;
} Node, *PNode;

/*
 * 结构体: LinkList
 * 作用: 带头结点单链表结构：head 指向头结点，便于统一处理插入和删除。
 */
typedef struct LinkList {
    PNode head;  /* 指向单链表中的头结点 */
} LinkList, *PLinkList;

#define STATIC_MAX_SIZE 20
/*
 * 结构体: Component
 * 作用: 静态链表结点结构：data 保存数据，cursor 保存下一个结点的数组下标。
 */
typedef struct {
    DataType data;
    int cursor;
} Component, SLinkList[STATIC_MAX_SIZE];

/*
 * 结构体: DoubleNode
 * 作用: 双向链表结点结构：info 保存数据，llink 指向前驱，rlink 指向后继。
 */
typedef struct DoubleNode {
    DataType info;
    struct DoubleNode *llink;
    struct DoubleNode *rlink;
} DoubleNode, *PDoubleNode;

/*
 * 结构体: DoubleList
 * 作用: 双向链表控制结构：head 和 tail 分别指向头尾哨兵结点。
 */
typedef struct {
    PDoubleNode head;
    PDoubleNode tail;
} DoubleList, *PDoubleList;

/*
 * 结构体: Term
 * 作用: 一元多项式项结构：coef 保存系数，expn 保存指数。
 */
typedef struct {
    double coef;
    int expn;
} Term;

/* ===== 当前主题相关的数据结构操作函数 ===== */

/*
 * 功能：求 Josephus 问题的出列序列。
 * 参数：
 *   n   - 初始人数，编号为 1..n。
 *   m   - 每次数到 m 的人出列。
 *   out - 输出数组，长度至少为 n。
 * 返回：无；出列顺序写入 out。
 */
void Josephus(int n, int m, int out[]) {
    int people[MAXNUM];
    int count = n;
    int index = 0;
    for (int i = 0; i < n; ++i) people[i] = i + 1;
    for (int k = 0; k < n; ++k) {
        index = (index + m - 1) % count;
        out[k] = people[index];
        for (int j = index; j < count - 1; ++j) {
            people[j] = people[j + 1];
        }
        --count;
    }
}

/*
 * 功能：一元多项式加法。
 * 参数：
 *   a, an  - 第一个多项式项数组及项数，按指数递增。
 *   b, bn  - 第二个多项式项数组及项数，按指数递增。
 *   result - 输出结果项数组。
 * 返回：结果多项式项数。
 * PPT 注释：指数相等则系数相加，系数为 0 的项删除。
 */
int AddPolyn(Term a[], int an, Term b[], int bn, Term result[]) {
    int i = 0;
    int j = 0;
    int k = 0;
    while (i < an && j < bn) {
        if (a[i].expn < b[j].expn) {
            result[k++] = a[i++];
        } else if (a[i].expn > b[j].expn) {
            result[k++] = b[j++];
        } else {
            double coef = a[i].coef + b[j].coef;
            if (coef != 0.0) {
                result[k++] = (Term){coef, a[i].expn};
            }
            ++i;
            ++j;
        }
    }
    while (i < an) result[k++] = a[i++];
    while (j < bn) result[k++] = b[j++];
    return k;
}

/* ===== 主题演示入口 ===== */

/*
 * 函数: main
 * 功能: 演示 Josephus 出列顺序，并构造两个一元多项式链表后完成相加。
 * 参数: 无。
 * 返回: 程序正常结束返回 0。
 * PPT 对应: 循环链表应用和多项式链表应用；展示链表在动态删除和按指数合并中的用法。
 */
int main(void) {
    int out[MAXNUM];
    Josephus(7, 3, out);
    printf("Josephus n=7,m=3: ");
    for (int i = 0; i < 7; ++i) printf("%d ", out[i]);
    printf("\n");

    Term pa[] = {{3, 0}, {2, 2}, {5, 4}};
    Term pb[] = {{-3, 0}, {7, 1}, {1, 4}};
    Term pc[MAXNUM];
    int n = AddPolyn(pa, 3, pb, 3, pc);
    printf("Polynomial sum: ");
    for (int i = 0; i < n; ++i) printf("%.0fx^%d ", pc[i].coef, pc[i].expn);
    printf("\n");
    return 0;
}
