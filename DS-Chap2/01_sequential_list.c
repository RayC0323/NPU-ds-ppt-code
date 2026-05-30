/*
 * 文件: DS-Chap2/01_sequential_list.c
 * 主题: 顺序表基本操作：插入、删除、定位、按序号取值、前驱/后继和判空。
 * 说明: 本文件为自包含示例，包含当前主题相关的数据结构定义、操作函数、辅助函数和演示 main。
 * 运行: 直接编译本文件即可运行，不依赖 src 目录或其他章节文件。
 * 示例: gcc -std=c11 -Wall -Wextra -pedantic DS-Chap2/01_sequential_list.c -o build/DS-Chap2/01_sequential_list
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
 * 功能：求顺序表中第一个元素的下标。
 * 参数：
 *   palist - 指向顺序表的指针。
 * 返回：非空表返回 0；空表返回 -1。
 */
int first_seq(PSeqList palist) {
    return palist->length == 0 ? -1 : 0;
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
 * 功能：求顺序表中某下标元素的后继位置。
 * 参数：
 *   palist - 指向顺序表的指针。
 *   p      - 当前元素下标。
 * 返回：存在后继则返回 p+1；否则返回 -1。
 */
int next_seq(PSeqList palist, int p) {
    return (p >= 0 && p < palist->length - 1) ? p + 1 : -1;
}

/*
 * 功能：求顺序表中某下标元素的前驱位置。
 * 参数：
 *   palist - 指向顺序表的指针。
 *   p      - 当前元素下标。
 * 返回：存在前驱则返回 p-1；否则返回 -1。
 */
int previous_seq(PSeqList palist, int p) {
    return (p > 0 && p < palist->length) ? p - 1 : -1;
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
 * 功能：判断顺序表是否为空。
 * 参数：
 *   palist - 指向顺序表的指针。
 * 返回：空表返回 TRUE，非空返回 FALSE。
 */
int isNullList_seq(PSeqList palist) {
    return palist->length == 0;
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
 * 功能: 构造一个顺序表，依次演示插入、删除、定位、取第 i 个元素、求前驱/后继和判空。
 * 参数: 无。
 * 返回: 程序正常结束返回 0。
 * PPT 对应: 线性表顺序存储的基本操作；重点观察元素后移/前移以及 length 字段的变化。
 */
int main(void) {
  SeqList list = {{0}, 0};
  insert_seq(&list, 10, 0);
  insert_seq(&list, 30, 1);
  insert_seq(&list, 20, 1);
  PrintSeqList("SeqList after insert", list);
  printf("first=%d, locate(20)=%d, retrieve(2)=%d\n", first_seq(&list),
         locate_seq(&list, 20), retrieve_seq(&list, 2));
  printf("previous index of 1=%d, next index of 1=%d\n", previous_seq(&list, 1),
         next_seq(&list, 1));
  delete_seq(&list, 1);
  PrintSeqList("SeqList after delete index 1", list);
  createNullList_seq(&list);
  printf("isNullList_seq=%d\n", isNullList_seq(&list));
  return 0;
}
