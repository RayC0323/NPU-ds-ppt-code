/*
 * 文件: DS-Chap2/05_static_linked_list.c
 * 主题: 静态链表备用空间链：InitList、Malloc、Free。
 * 说明: 本文件为自包含示例，包含当前主题相关的数据结构定义、操作函数、辅助函数和演示 main。
 * 运行: 直接编译本文件即可运行，不依赖 src 目录或其他章节文件。
 * 示例: gcc -std=c11 -Wall -Wextra -pedantic DS-Chap2/05_static_linked_list.c -o build/DS-Chap2/05_static_linked_list
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
 * 功能：初始化静态链表的备用空间链。
 * 参数：
 *   list - 静态链表数组，0 号单元作为备用链表头。
 * 返回：无。
 * PPT 注释：将整个数组空间初始化成一个链表。
 */
void InitList(SLinkList list) {
    for (int i = 0; i < STATIC_MAX_SIZE - 1; ++i) {
        list[i].cursor = i + 1;
    }
    list[STATIC_MAX_SIZE - 1].cursor = 0;
}

/*
 * 功能：从静态链表备用空间链申请一个空闲结点。
 * 参数：
 *   list - 静态链表数组。
 * 返回：可用结点下标；若无空间则返回 0。
 */
int Malloc(SLinkList list) {
    int i = list[0].cursor;
    if (i != 0) {
        list[0].cursor = list[i].cursor;
    }
    return i;
}

/*
 * 功能：回收静态链表中的一个结点。
 * 参数：
 *   list - 静态链表数组。
 *   k    - 要回收的结点下标。
 * 返回：无。
 * PPT 注释：将释放出的节点连接到可用空间链上。
 */
void Free(SLinkList list, int k) {
    list[k].cursor = list[0].cursor;
    list[0].cursor = k;
}

/* ===== 主题演示入口 ===== */

/*
 * 函数: main
 * 功能: 初始化静态链表备用空间，申请两个数组结点，回收其中一个结点并查看备用链表头变化。
 * 参数: 无。
 * 返回: 程序正常结束返回 0。
 * PPT 对应: 静态链表；把数组下标当作“游标指针”，理解 Malloc 和 Free 对备用空间链的维护。
 */
int main(void) {
    SLinkList list;
    InitList(list);
    int a = Malloc(list);
    int b = Malloc(list);
    list[a].data = 42;
    list[b].data = 84;
    printf("allocated: %d(%d), %d(%d)\n", a, list[a].data, b, list[b].data);
    Free(list, a);
    printf("after Free(%d), first free cursor=%d\n", a, list[0].cursor);
    return 0;
}
