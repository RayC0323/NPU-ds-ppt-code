/*
 * 文件: DS-Chap3/04_queues.c
 * 主题: 循环顺序队列和链队列：入队、出队和取队头。
 * 说明: 本文件为自包含示例，包含当前主题相关的数据结构定义、操作函数、辅助函数和演示 main。
 * 运行: 直接编译本文件即可运行，不依赖 src 目录或其他章节文件。
 * 示例: gcc -std=c11 -Wall -Wextra -pedantic DS-Chap3/04_queues.c -o build/DS-Chap3/04_queues
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXNUM 100
#define TRUE 1
#define FALSE 0

typedef int ElemType;
typedef int DataType;

typedef struct {
    ElemType element[MAXNUM];
    int top;
} SeqStack, *PSeqStack;

typedef struct StackNode {
    DataType data;
    struct StackNode *next;
} StackNode, *PStackNode;

typedef struct {
    PStackNode top;
} LinkStack, *PLinkStack;

typedef struct QNode {
    DataType data;
    struct QNode *next;
} QNode, *PQNode;

typedef struct {
    PQNode front;
    PQNode rear;
} LinkQueue, *PLinkQueue;

typedef struct {
    DataType element[MAXNUM];
    int front;
    int rear;
} SeqQueue, *PSeqQueue;

/* ===== 当前主题相关的数据结构操作函数 ===== */

/*
 * 功能：创建并初始化一个空链队列。
 * 参数：无。
 * 返回：新分配的链队列指针，front/rear 均为 NULL。
 */
PLinkQueue createEmptyQueue_link(void) {
    PLinkQueue q = (PLinkQueue)malloc(sizeof(LinkQueue));
    if (q == NULL) exit(EXIT_FAILURE);
    q->front = q->rear = NULL;
    return q;
}

/*
 * 功能：判断链队列是否为空。
 * 参数：
 *   q - 指向链队列的指针。
 * 返回：空队列返回 TRUE，否则返回 FALSE。
 */
int isEmptyQueue_link(PLinkQueue q) {
    return q->front == NULL;
}

/*
 * 功能：从队尾插入一个链队列结点。
 * 参数：
 *   q - 指向链队列的指针。
 *   x - 入队元素。
 * 返回：无。
 */
void enQueue_link(PLinkQueue q, DataType x) {
    PQNode node = (PQNode)malloc(sizeof(QNode));
    if (node == NULL) exit(EXIT_FAILURE);
    node->data = x;
    node->next = NULL;
    if (q->rear == NULL) {
        q->front = q->rear = node;
    } else {
        q->rear->next = node;
        q->rear = node;
    }
}

/*
 * 功能：删除并返回链队列队头元素。
 * 参数：
 *   q - 指向链队列的指针。
 * 返回：非空时返回队头数据；空队列返回 0。
 */
DataType deQueue_link(PLinkQueue q) {
    if (isEmptyQueue_link(q)) return 0;
    PQNode node = q->front;
    DataType value = node->data;
    q->front = node->next;
    if (q->front == NULL) q->rear = NULL;
    free(node);
    return value;
}

/*
 * 功能：读取链队列队头元素但不出队。
 * 参数：
 *   q - 指向链队列的指针。
 * 返回：非空时返回队头数据；空队列返回 0。
 */
DataType frontQueue_link(PLinkQueue q) {
    return isEmptyQueue_link(q) ? 0 : q->front->data;
}

/*
 * 功能：销毁链队列并释放所有结点。
 * 参数：
 *   q - 待销毁链队列。
 * 返回：无。
 */
void destroy_link_queue(PLinkQueue q) {
    while (!isEmptyQueue_link(q)) {
        deQueue_link(q);
    }
    free(q);
}

/*
 * 功能：创建并初始化一个空循环顺序队列。
 * 参数：无。
 * 返回：新分配的顺序队列指针，front 和 rear 均为 0。
 */
PSeqQueue createEmptyQueue_seq(void) {
    PSeqQueue q = (PSeqQueue)malloc(sizeof(SeqQueue));
    if (q == NULL) exit(EXIT_FAILURE);
    q->front = q->rear = 0;
    return q;
}

/*
 * 功能：判断循环顺序队列是否为空。
 * 参数：
 *   q - 指向顺序队列的指针。
 * 返回：队空返回 TRUE，否则返回 FALSE。
 */
int isEmptyQueue_seq(PSeqQueue q) {
    return q->front == q->rear;
}

/*
 * 功能：向循环顺序队列队尾插入元素。
 * 参数：
 *   q - 指向顺序队列的指针。
 *   x - 入队元素。
 * 返回：无；队满时输出提示并放弃入队。
 */
void enQueue_seq(PSeqQueue q, DataType x) {
    if ((q->rear + 1) % MAXNUM == q->front) {
        printf("Queue overflow!\n");
        return;
    }
    q->element[q->rear] = x;
    q->rear = (q->rear + 1) % MAXNUM;
}

/*
 * 功能：删除并返回循环顺序队列队头元素。
 * 参数：
 *   q - 指向顺序队列的指针。
 * 返回：非空时返回队头数据；空队列返回 0。
 */
DataType deQueue_seq(PSeqQueue q) {
    if (isEmptyQueue_seq(q)) return 0;
    DataType value = q->element[q->front];
    q->front = (q->front + 1) % MAXNUM;
    return value;
}

/*
 * 功能：读取循环顺序队列队头元素但不出队。
 * 参数：
 *   q - 指向顺序队列的指针。
 * 返回：非空时返回队头数据；空队列返回 0。
 */
DataType frontQueue_seq(PSeqQueue q) {
    return isEmptyQueue_seq(q) ? 0 : q->element[q->front];
}

/* ===== 主题演示入口 ===== */

/*
 * 函数: main
 * 功能: 分别创建循环顺序队列和链队列，演示入队、取队头、出队和队空判断。
 * 参数: 无。
 * 返回: 程序正常结束返回 0。
 * PPT 对应: 队列的两种存储结构；重点观察 front/rear 指针移动和循环取模。
 */
int main(void) {
    PSeqQueue seq = createEmptyQueue_seq();
    PLinkQueue link = createEmptyQueue_link();
    enQueue_seq(seq, 11);
    enQueue_seq(seq, 22);
    int seq_front = frontQueue_seq(seq);
    int seq_dequeued = deQueue_seq(seq);
    printf("SeqQueue front=%d deQueue=%d\n", seq_front, seq_dequeued);
    enQueue_link(link, 33);
    enQueue_link(link, 44);
    int link_front = frontQueue_link(link);
    int link_dequeued = deQueue_link(link);
    printf("LinkQueue front=%d deQueue=%d\n", link_front, link_dequeued);
    free(seq);
    destroy_link_queue(link);
    return 0;
}
