/*
 * 文件: DS-Chap2/03_singly_linked_list.c
 * 主题: 带头结点单链表：插入、删除、定位、前驱和第 i 个结点。
 * 说明: 本文件为自包含示例，包含当前主题相关的数据结构定义、操作函数、辅助函数和演示 main。
 * 运行: 直接编译本文件即可运行，不依赖 src 目录或其他章节文件。
 * 示例: gcc -std=c11 -Wall -Wextra -pedantic DS-Chap2/03_singly_linked_list.c -o build/DS-Chap2/03_singly_linked_list
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
 * 功能：删除带头结点单链表中第一个值为 x 的结点。
 * 参数：
 *   list - 指向带头结点单链表的指针。
 *   x    - 要删除的元素值。
 * 返回：无；若未找到 x，链表保持不变。
 * PPT 注释：先找 x 的前驱结点，再改变链接并释放被删结点。
 */
void delete_link(PLinkList list, DataType x) {
    PNode p = list->head;
    while (p->next != NULL && p->next->info != x) {
        p = p->next;
    }
    if (p->next != NULL) {
        PNode q = p->next;
        p->next = q->next;
        free(q);
    }
}

/*
 * 功能：返回带头结点单链表中第一个数据结点的位置。
 * 参数：
 *   list - 指向带头结点单链表的指针。
 * 返回：第一个数据结点指针；空表返回 NULL。
 */
PNode first_link(PLinkList list) {
    return list->head->next;
}

/*
 * 功能：按值定位单链表结点。
 * 参数：
 *   list - 指向带头结点单链表的指针。
 *   x    - 要查找的元素值。
 * 返回：找到时返回结点指针；未找到返回 NULL。
 */
PNode locate_link(PLinkList list, DataType x) {
    PNode p = list->head->next;
    while (p != NULL && p->info != x) {
        p = p->next;
    }
    return p;
}

/*
 * 功能：取得单链表结点的数据域。
 * 参数：
 *   p - 目标结点指针。
 * 返回：p 非空时返回 p->info；p 为空时返回 SPECIAL。
 */
DataType retrieve_link(PNode p) {
    return p == NULL ? SPECIAL : p->info;
}

/*
 * 功能：查找值为 x 的结点的前驱结点。
 * 参数：
 *   list - 指向带头结点单链表的指针。
 *   x    - 目标结点的数据值。
 * 返回：找到时返回前驱结点指针；未找到返回 NULL。
 */
PNode previous_link(PLinkList list, DataType x) {
    PNode p = list->head;
    while (p->next != NULL && p->next->info != x) {
        p = p->next;
    }
    return p->next == NULL ? NULL : p;
}

/*
 * 功能：在带头结点单链表中查找第 i 个数据结点。
 * 参数：
 *   list - 指向带头结点单链表的指针。
 *   i    - 1 基序号，i < 1 为非法。
 * 返回：第 i 个数据结点指针；不存在时返回 NULL。
 */
PNode find_link(PLinkList list, int i) {
    if (i < 1) return NULL;
    PNode p = list->head;
    for (int j = 0; j < i && p != NULL; ++j) {
        p = p->next;
    }
    return p;
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
 * 功能: 构造带头结点单链表，演示按位置插入、按值删除、定位、求前驱和按序号取结点。
 * 参数: 无。
 * 返回: 程序正常结束返回 0。
 * PPT 对应: 单链表基本操作；重点观察插入/删除时前驱结点的 next 指针如何改接。
 */
int main(void) {
    int values[] = {1, 3, 5, 7};
    PLinkList list = BuildLinkList(values, 4);
    insert_link(list, 4, find_link(list, 2));
    delete_link(list, 7);
    PrintLinkList("Linked list", list);
    printf("first=%d, locate(5)=%d, previous(4)=%d\n",
           retrieve_link(first_link(list)),
           retrieve_link(locate_link(list, 5)),
           retrieve_link(previous_link(list, 4)));
    DestroyLinkList(list);
    return 0;
}
