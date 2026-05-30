/*
 * 文件: src/DS-Chap2.c
 * 说明: 第 2 章线性表公共实现：顺序表、单链表、静态链表、双向链表、Josephus 和多项式。
 * 来源: 根据课程 PPT 中的代码片段整理为可运行 C11 程序。
 * 注释: 每个函数前都包含用途、参数、返回值和对应 PPT 算法说明；主题入口文件会按知识点调用这里的公共函数。
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

typedef struct Node {
    DataType info;
    struct Node *next;
} Node, *PNode;

typedef struct LinkList {
    PNode head;  /* 指向单链表中的头结点 */
} LinkList, *PLinkList;

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
 * 功能：返回单链表中给定结点的后继结点。
 * 参数：
 *   p - 当前结点指针。
 * 返回：p 非空时返回 p->next；p 为空时返回 NULL。
 */
PNode next_link(PNode p) {
    return p == NULL ? NULL : p->next;
}

/*
 * 功能：判断带头结点单链表是否为空。
 * 参数：
 *   list - 指向带头结点单链表的指针。
 * 返回：空表返回 TRUE，非空返回 FALSE。
 */
int isNullLink_link(PLinkList list) {
    return list->head->next == NULL;
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

#define STATIC_MAX_SIZE 20
typedef struct {
    DataType data;
    int cursor;
} Component, SLinkList[STATIC_MAX_SIZE];

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

typedef struct DoubleNode {
    DataType info;
    struct DoubleNode *llink;
    struct DoubleNode *rlink;
} DoubleNode, *PDoubleNode;

typedef struct {
    PDoubleNode head;
    PDoubleNode tail;
} DoubleList, *PDoubleList;

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

typedef struct {
    double coef;
    int expn;
} Term;

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

/*
 * 功能：第 2 章线性表相关算法演示入口。
 * 参数：无。
 * 返回：0 表示程序正常结束。
 * 说明：依次演示顺序表、链表、静态链表、双向链表、Josephus 和多项式加法。
 */
int main(void) {
    SeqList la = {{0}, 0};
    SeqList lb = {{0}, 0};
    SeqList lc = {{0}, 0};
    int a_values[] = {3, 5, 8, 11};
    int b_values[] = {2, 6, 8, 9, 11, 15, 20};
    for (int i = 0; i < 4; ++i) insert_seq(&la, a_values[i], la.length);
    for (int i = 0; i < 7; ++i) insert_seq(&lb, b_values[i], lb.length);
    MergeSqList(&la, &lb, &lc);
    PrintSeqList("Merged SqList", lc);
    printf("locate_seq(8)=%d, retrieve_seq(3)=%d\n", locate_seq(&lc, 8), retrieve_seq(&lc, 3));

    SeqList setA = {{0}, 0};
    SeqList setB = {{0}, 0};
    int set_a[] = {1, 3, 5};
    int set_b[] = {3, 4, 5, 7};
    for (int i = 0; i < 3; ++i) insert_seq(&setA, set_a[i], setA.length);
    for (int i = 0; i < 4; ++i) insert_seq(&setB, set_b[i], setB.length);
    Union(&setA, &setB);
    PrintSeqList("Union", setA);
    Intersection(&setA, &setB);
    PrintSeqList("Intersection", setA);

    int link_a_values[] = {1, 3, 5, 7};
    int link_b_values[] = {2, 4, 6};
    PLinkList linkA = BuildLinkList(link_a_values, 4);
    PLinkList linkB = BuildLinkList(link_b_values, 3);
    PLinkList merged = createNullList_link();
    insert_link(linkA, 4, find_link(linkA, 2));
    delete_link(linkA, 7);
    PrintLinkList("Linked list", linkA);
    printf("first_link=%d, previous of 4=%d\n",
           retrieve_link(first_link(linkA)), retrieve_link(previous_link(linkA, 4)));
    MergeList_L(*linkA, *linkB, merged);
    PrintLinkList("Merged linked list", merged);

    SLinkList static_list;
    InitList(static_list);
    int node1 = Malloc(static_list);
    int node2 = Malloc(static_list);
    static_list[node1].data = 42;
    static_list[node2].data = 84;
    Free(static_list, node1);
    printf("Static list allocated nodes: %d, %d\n", node1, node2);

    PDoubleList dlist = CreateDoubleList();
    insert_dbllink(dlist, 10, dlist->tail);
    insert_dbllink(dlist, 20, dlist->tail);
    insert_dbllink(dlist, 15, dlist->tail->llink);
    PrintDoubleList("Double linked list", dlist);

    int out[MAXNUM];
    Josephus(7, 3, out);
    printf("Josephus n=7,m=3: ");
    for (int i = 0; i < 7; ++i) printf("%d ", out[i]);
    printf("\n");

    Term pa[] = {{3, 0}, {2, 2}, {5, 4}};
    Term pb[] = {{-3, 0}, {7, 1}, {1, 4}};
    Term pc[MAXNUM];
    int pn = AddPolyn(pa, 3, pb, 3, pc);
    printf("Polynomial sum: ");
    for (int i = 0; i < pn; ++i) printf("%.0fx^%d ", pc[i].coef, pc[i].expn);
    printf("\n");

    DestroyLinkList(linkA);
    DestroyLinkList(linkB);
    DestroyLinkList(merged);
    DestroyDoubleList(dlist);
    return 0;
}
