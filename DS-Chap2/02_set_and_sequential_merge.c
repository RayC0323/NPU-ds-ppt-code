/*
 * 文件: DS-Chap2/02_set_and_sequential_merge.c
 * 主题: 顺序表集合运算与有序顺序表归并：Union、Intersection、MergeSqList。
 * 说明: 本文件为自包含示例，包含当前主题相关的数据结构定义、操作函数、辅助函数和演示 main。
 * 运行: 直接编译本文件即可运行，不依赖 src 目录或其他章节文件。
 * 示例: gcc -std=c11 -Wall -Wextra -pedantic DS-Chap2/02_set_and_sequential_merge.c -o build/DS-Chap2/02_set_and_sequential_merge
 */

#include <stdio.h>
#include <stdlib.h>

#define MAXNUM 100
#define TRUE 1
#define FALSE 0
#define SPECIAL 2147483647

typedef int DataType;

typedef struct SeqList {
    DataType element[MAXNUM];
    int length;  /* length < MAXNUM */
} SeqList, *PSeqList;

typedef struct Node {
    DataType info;
    struct Node *next;
} Node, *PNode;

typedef struct LinkList {
    PNode head;  /* 指向单链表中的头结点 */
} LinkList, *PLinkList;

#define STATIC_MAX_SIZE 20
typedef struct {
    DataType data;
    int cursor;
} Component, SLinkList[STATIC_MAX_SIZE];

typedef struct DoubleNode {
    DataType info;
    struct DoubleNode *llink;
    struct DoubleNode *rlink;
} DoubleNode, *PDoubleNode;

typedef struct {
    PDoubleNode head;
    PDoubleNode tail;
} DoubleList, *PDoubleList;

typedef struct {
    double coef;
    int expn;
} Term;

/* ===== 当前主题相关的数据结构操作函数 ===== */

/*
 * 功能：在顺序表指定下标位置插入元素。
 * 参数：
 *   palist - 指向顺序表的指针。
 *   x      - 要插入的新元素值。
 *   p      - 插入位置下标，合法范围为 0 <= p <= palist->length。
 * 返回：插入成功返回 TRUE，表满或下标非法返回 FALSE。
 * PPT 注释：将 p 及以后的元素后移一个下标位置，再把 x 放入 p 下标位置。
 */
int insert_seq(PSeqList palist, DataType x, int p) {
    if (palist->length == MAXNUM) {
        printf("Overflow!\n");
        return FALSE;
    }
    if (p < 0 || p > palist->length) {
        printf("not exist!\n");
        return FALSE;
    }
    for (int q = palist->length - 1; q >= p; --q) {
        palist->element[q + 1] = palist->element[q];
    }
    palist->element[p] = x;
    ++palist->length;
    return TRUE;
}

/*
 * 功能：删除顺序表中指定下标位置的元素。
 * 参数：
 *   palist - 指向顺序表的指针。
 *   p      - 待删除元素下标，合法范围为 0 <= p < palist->length。
 * 返回：删除成功返回 TRUE，下标非法返回 FALSE。
 * PPT 注释：将 p 之后所有元素前移一个位置，表长减 1。
 */
int delete_seq(PSeqList palist, int p) {
    if (p < 0 || p > palist->length - 1) {
        printf("not exist!\n");
        return FALSE;
    }
    for (int q = p; q < palist->length - 1; ++q) {
        palist->element[q] = palist->element[q + 1];
    }
    --palist->length;
    return TRUE;
}

/*
 * 功能：按值查找顺序表中的元素。
 * 参数：
 *   palist - 指向顺序表的指针。
 *   x      - 要查找的元素值。
 * 返回：找到时返回第一次出现的下标；未找到返回 -1。
 * PPT 注释：求 x 在 palist 所指顺序表中的下标位置。
 */
int locate_seq(PSeqList palist, DataType x) {
    for (int q = 0; q < palist->length; ++q) {
        if (palist->element[q] == x) {
            return q;
        }
    }
    return -1;
}

/*
 * 功能：按序号取得顺序表元素。
 * 参数：
 *   palist - 指向顺序表的指针。
 *   p      - 元素序号，采用 PPT 中的 1 基序号。
 * 返回：若 1 <= p <= length，返回第 p 个元素；否则返回 SPECIAL。
 */
DataType retrieve_seq(PSeqList palist, int p) {
    if (p > 0 && p <= palist->length) {
        return palist->element[p - 1];
    }
    return SPECIAL;
}

/*
 * 功能：将顺序表置为空表。
 * 参数：
 *   palist - 指向顺序表的指针。
 * 返回：无；函数只把 length 置为 0，不清空数组内容。
 */
void createNullList_seq(PSeqList palist) {
    palist->length = 0;
}

/*
 * 功能：求两个顺序表表示集合的并集，结果保存在 la 中。
 * 参数：
 *   la - 第一个集合，同时作为结果集合。
 *   lb - 第二个集合，只读。
 * 返回：无。
 * PPT 注释：逐个取出 lb 中元素，若在 la 中未找到，则插入 la 末尾。
 */
void Union(SeqList *la, SeqList *lb) {
    int n = la->length;
    int m = lb->length;
    for (int i = 1; i <= m; ++i) {
        DataType x = retrieve_seq(lb, i);
        if (locate_seq(la, x) == -1) {
            insert_seq(la, x, n++);
        }
    }
}

/*
 * 功能：求两个顺序表表示集合的交集，结果保存在 la 中。
 * 参数：
 *   la - 第一个集合，同时作为结果集合。
 *   lb - 第二个集合，只读。
 * 返回：无。
 * PPT 注释：扫描 la，若当前元素不在 lb 中则删除，否则继续扫描下一个。
 */
void Intersection(SeqList *la, SeqList *lb) {
    int i = 0;
    while (i < la->length) {
        if (locate_seq(lb, la->element[i]) == -1) {
            delete_seq(la, i);
        } else {
            ++i;
        }
    }
}

/*
 * 功能：合并两个递增有序顺序表。
 * 参数：
 *   la - 第一个有序顺序表。
 *   lb - 第二个有序顺序表。
 *   lc - 输出顺序表，用于保存合并结果。
 * 返回：无。
 * PPT 注释：Merge two sorted lists into a new list.
 */
void MergeSqList(SeqList *la, SeqList *lb, SeqList *lc) {
    int i = 0;
    int j = 0;
    createNullList_seq(lc);
    while (i < la->length && j < lb->length) {
        if (la->element[i] <= lb->element[j]) {
            insert_seq(lc, la->element[i++], lc->length);
        } else {
            insert_seq(lc, lb->element[j++], lc->length);
        }
    }
    while (i < la->length) insert_seq(lc, la->element[i++], lc->length);
    while (j < lb->length) insert_seq(lc, lb->element[j++], lc->length);
}

/*
 * 功能：打印顺序表内容。
 * 参数：
 *   name - 输出标签。
 *   list - 待打印顺序表。
 * 返回：无。
 */
static void PrintSeqList(const char *name, SeqList list) {
    printf("%s: ", name);
    for (int i = 0; i < list.length; ++i) printf("%d ", list.element[i]);
    printf("\n");
}

/* ===== 主题演示入口 ===== */

/*
 * 函数: main
 * 功能: 构造两个顺序表，分别演示集合并、集合交，以及两个有序顺序表归并为第三个表。
 * 参数: 无。
 * 返回: 程序正常结束返回 0。
 * PPT 对应: 线性表应用题；对比集合运算的“去重”思路和有序归并的“双指针”思路。
 */
int main(void) {
    SeqList a = {{0}, 0};
    SeqList b = {{0}, 0};
    SeqList c = {{0}, 0};
    int av[] = {1, 3, 5};
    int bv[] = {3, 4, 5, 7};
    for (int i = 0; i < 3; ++i) insert_seq(&a, av[i], a.length);
    for (int i = 0; i < 4; ++i) insert_seq(&b, bv[i], b.length);
    Union(&a, &b);
    PrintSeqList("Union", a);
    Intersection(&a, &b);
    PrintSeqList("Intersection", a);

    SeqList left = {{0}, 0};
    SeqList right = {{0}, 0};
    int lv[] = {3, 5, 8, 11};
    int rv[] = {2, 6, 8, 9, 11, 15, 20};
    for (int i = 0; i < 4; ++i) insert_seq(&left, lv[i], left.length);
    for (int i = 0; i < 7; ++i) insert_seq(&right, rv[i], right.length);
    MergeSqList(&left, &right, &c);
    PrintSeqList("MergeSqList", c);
    return 0;
}
