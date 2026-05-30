/*
 * 文件: DS-Chap2/06_double_linked_list.c
 * 主题: 双向链表：在指定结点前插入和删除指定结点。
 * 说明: 本文件为自包含示例，包含当前主题相关的数据结构定义、操作函数、辅助函数和演示 main。
 * 运行: 直接编译本文件即可运行，不依赖 src 目录或其他章节文件。
 * 示例: gcc -std=c11 -Wall -Wextra -pedantic DS-Chap2/06_double_linked_list.c -o build/DS-Chap2/06_double_linked_list
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
 * 功能：创建带头尾哨兵结点的双向链表。
 * 参数：无。
 * 返回：新建双向链表指针。
 */
PDoubleList CreateDoubleList(void) {
    PDoubleList list = (PDoubleList)malloc(sizeof(DoubleList));
    if (list == NULL) exit(EXIT_FAILURE);
    list->head = (PDoubleNode)malloc(sizeof(DoubleNode));
    list->tail = (PDoubleNode)malloc(sizeof(DoubleNode));
    if (list->head == NULL || list->tail == NULL) exit(EXIT_FAILURE);
    list->head->rlink = list->tail;
    list->head->llink = NULL;
    list->tail->llink = list->head;
    list->tail->rlink = NULL;
    return list;
}

/*
 * 功能：在双向链表结点 p 之前插入新元素。
 * 参数：
 *   list - 双向链表指针，用于保持接口一致。
 *   x    - 新结点数据值。
 *   p    - 插入位置，新结点插入到 p 的左侧。
 * 返回：无。
 * PPT 注释：s->llink=p->llink; s->rlink=p; 再改两侧链接。
 */
void insert_dbllink(PDoubleList list, DataType x, PDoubleNode p) {
    (void)list;
    PDoubleNode s = (PDoubleNode)malloc(sizeof(DoubleNode));
    if (s == NULL) exit(EXIT_FAILURE);
    s->info = x;
    s->llink = p->llink;
    s->rlink = p;
    p->llink->rlink = s;
    p->llink = s;
}

/*
 * 功能：删除双向链表中的指定结点。
 * 参数：
 *   list - 双向链表指针，用于保持接口一致。
 *   p    - 要删除的结点，不能是头尾哨兵。
 * 返回：无。
 * PPT 注释：p->llink->rlink=p->rlink; p->rlink->llink=p->llink。
 */
void delete_dbllink(PDoubleList list, PDoubleNode p) {
    (void)list;
    if (p == NULL || p->llink == NULL || p->rlink == NULL) return;
    p->llink->rlink = p->rlink;
    p->rlink->llink = p->llink;
    free(p);
}

/*
 * 功能：释放双向链表的所有结点和表头结构。
 * 参数：
 *   list - 待销毁双向链表。
 * 返回：无。
 */
void DestroyDoubleList(PDoubleList list) {
    PDoubleNode p = list->head;
    while (p != NULL) {
        PDoubleNode next = p->rlink;
        free(p);
        p = next;
    }
    free(list);
}

/*
 * 功能：打印双向链表内容。
 * 参数：
 *   name - 输出标签。
 *   list - 待打印双向链表。
 * 返回：无。
 */
static void PrintDoubleList(const char *name, PDoubleList list) {
    printf("%s: ", name);
    for (PDoubleNode p = list->head->rlink; p != list->tail; p = p->rlink) printf("%d ", p->info);
    printf("\n");
}

/* ===== 主题演示入口 ===== */

/*
 * 函数: main
 * 功能: 创建双向链表，在指定结点前插入多个元素，再删除中间结点并打印前后结果。
 * 参数: 无。
 * 返回: 程序正常结束返回 0。
 * PPT 对应: 双向链表插入和删除；重点检查 llink 与 rlink 是否成对更新。
 */
int main(void) {
    PDoubleList list = CreateDoubleList();
    insert_dbllink(list, 10, list->tail);
    insert_dbllink(list, 30, list->tail);
    insert_dbllink(list, 20, list->tail->llink);
    PrintDoubleList("Double linked list", list);
    delete_dbllink(list, list->head->rlink->rlink);
    PrintDoubleList("After deleting middle node", list);
    DestroyDoubleList(list);
    return 0;
}
