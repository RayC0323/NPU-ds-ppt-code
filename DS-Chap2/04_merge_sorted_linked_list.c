/*
 * 文件: DS-Chap2/04_merge_sorted_linked_list.c
 * 主题: 合并两个有序单链表：对应 PPT 中 MergeList_L 问题。
 * 说明: 本文件为自包含示例，包含当前主题相关的数据结构定义、操作函数、辅助函数和演示 main。
 * 运行: 直接编译本文件即可运行，不依赖 src 目录或其他章节文件。
 * 示例: gcc -std=c11 -Wall -Wextra -pedantic DS-Chap2/04_merge_sorted_linked_list.c -o build/DS-Chap2/04_merge_sorted_linked_list
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
 * 功能：创建一个带头结点的空单链表。
 * 参数：无。
 * 返回：指向新建链表结构的指针；内存不足时程序退出。
 */
PLinkList createNullList_link(void) {
    PLinkList list = (PLinkList)malloc(sizeof(LinkList));
    if (list == NULL) exit(EXIT_FAILURE);
    list->head = (PNode)malloc(sizeof(Node));
    if (list->head == NULL) exit(EXIT_FAILURE);
    list->head->next = NULL;
    return list;
}

/*
 * 功能：在单链表指定结点之后插入新结点。
 * 参数：
 *   list - 链表指针，此处用于保持 PPT 函数接口，实际插入只依赖 p。
 *   x    - 新结点数据域。
 *   p    - 插入位置的前驱结点，新结点插在 p 后面。
 * 返回：无。
 * PPT 注释：q->next = p->next; p->next = q。
 */
void insert_link(PLinkList list, DataType x, PNode p) {
    (void)list;
    PNode q = (PNode)malloc(sizeof(Node));
    if (q == NULL) exit(EXIT_FAILURE);
    q->info = x;
    q->next = p->next;
    p->next = q;
}

/*
 * 功能：根据数组批量构造带头结点单链表。
 * 参数：
 *   values - 输入数组。
 *   n      - 数组元素个数。
 * 返回：新建链表指针，结点次序与数组一致。
 */
PLinkList BuildLinkList(const int values[], int n) {
    PLinkList list = createNullList_link();
    PNode tail = list->head;
    for (int i = 0; i < n; ++i) {
        insert_link(list, values[i], tail);
        tail = tail->next;
    }
    return list;
}

/*
 * 功能：合并两个递增有序单链表。
 * 参数：
 *   la - 第一个带头结点有序链表，按值传入只读取其结点。
 *   lb - 第二个带头结点有序链表。
 *   lc - 输出链表，需预先创建头结点。
 * 返回：无。
 * PPT 注释：Merging two sorted linked lists (with head node).
 */
void MergeList_L(LinkList la, LinkList lb, PLinkList lc) {
    PNode pa = la.head->next;
    PNode pb = lb.head->next;
    PNode pc = lc->head;
    while (pa != NULL && pb != NULL) {
        if (pa->info <= pb->info) {
            pc->next = (PNode)malloc(sizeof(Node));
            pc = pc->next;
            pc->info = pa->info;
            pa = pa->next;
        } else {
            pc->next = (PNode)malloc(sizeof(Node));
            pc = pc->next;
            pc->info = pb->info;
            pb = pb->next;
        }
        pc->next = NULL;
    }
    PNode rest = pa != NULL ? pa : pb;
    while (rest != NULL) {
        pc->next = (PNode)malloc(sizeof(Node));
        pc = pc->next;
        pc->info = rest->info;
        pc->next = NULL;
        rest = rest->next;
    }
}

/*
 * 功能：释放带头结点单链表占用的全部内存。
 * 参数：
 *   list - 待销毁链表。
 * 返回：无；调用后 list 不应继续使用。
 */
void DestroyLinkList(PLinkList list) {
    PNode p = list->head;
    while (p != NULL) {
        PNode next = p->next;
        free(p);
        p = next;
    }
    free(list);
}

/*
 * 功能：打印带头结点单链表内容。
 * 参数：
 *   name - 输出标签。
 *   list - 待打印链表。
 * 返回：无。
 */
static void PrintLinkList(const char *name, PLinkList list) {
    printf("%s: ", name);
    for (PNode p = list->head->next; p != NULL; p = p->next) printf("%d ", p->info);
    printf("\n");
}

/* ===== 主题演示入口 ===== */

/*
 * 函数: main
 * 功能: 构造两条递增有序单链表，调用 MergeList_L 合并到第三条链表并打印结果。
 * 参数: 无。
 * 返回: 程序正常结束返回 0。
 * PPT 对应: 合并两个有序链表问题；重点观察 pa、pb、pc 三个工作指针如何推进。
 */
int main(void) {
    int a[] = {1, 3, 5, 7};
    int b[] = {2, 4, 6, 8};
    PLinkList la = BuildLinkList(a, 4);
    PLinkList lb = BuildLinkList(b, 4);
    PLinkList lc = createNullList_link();
    MergeList_L(*la, *lb, lc);
    PrintLinkList("Merged sorted linked list", lc);
    DestroyLinkList(la);
    DestroyLinkList(lb);
    DestroyLinkList(lc);
    return 0;
}
