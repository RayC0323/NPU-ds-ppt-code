/*
 * 文件: DS-Chap3/01_stacks.c
 * 主题: 顺序栈和链栈：初始化、判空、入栈、出栈和取栈顶。
 * 说明:
 * 本文件为自包含示例，包含当前主题相关的数据结构定义、操作函数、辅助函数和演示
 * main。 运行: 直接编译本文件即可运行，不依赖 src 目录或其他章节文件。 示例:
 * gcc -std=c11 -Wall -Wextra -pedantic DS-Chap3/01_stacks.c -o
 * build/DS-Chap3/01_stacks
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
 * 功能：创建并初始化一个空顺序栈。
 * 参数：无。
 * 返回：新分配的顺序栈指针，top 初始化为 -1。
 * PPT 对应：Algorithm 3.1 Initialization。
 */
PSeqStack createEmptyStack_seq(void) {
  PSeqStack stack = (PSeqStack)malloc(sizeof(SeqStack));
  if (stack == NULL) {
    exit(EXIT_FAILURE);
  }
  stack->top = -1;
  return stack;
}

/*
 * 功能：判断顺序栈是否为空。
 * 参数：
 *   pastack - 指向顺序栈的指针。
 * 返回：空栈返回 TRUE，否则返回 FALSE。
 */
int isEmptyStack_seq(PSeqStack pastack) { return pastack->top == -1; }

/*
 * 功能：向顺序栈栈顶压入一个元素。
 * 参数：
 *   pastack - 指向顺序栈的指针。
 *   x       - 待入栈元素。
 * 返回：无；若栈满则打印 overflow 并放弃插入。
 */
void push_seq(PSeqStack pastack, ElemType x) {
  if (pastack->top >= MAXNUM - 1) {
    printf("overflow!\n");
    return;
  }
  pastack->element[++pastack->top] = x;
}

/*
 * 功能：弹出顺序栈栈顶元素。
 * 参数：
 *   pastack - 指向顺序栈的指针。
 * 返回：非空时返回栈顶元素；空栈时打印提示并返回 0。
 */
ElemType pop_seq(PSeqStack pastack) {
  if (isEmptyStack_seq(pastack)) {
    printf("Empty Stack!\n");
    return 0;
  }
  return pastack->element[pastack->top--];
}

/*
 * 功能：读取顺序栈栈顶元素但不出栈。
 * 参数：
 *   pastack - 指向顺序栈的指针。
 * 返回：非空时返回栈顶元素；空栈时打印提示并返回 0。
 */
ElemType top_seq(PSeqStack pastack) {
  if (isEmptyStack_seq(pastack)) {
    printf("Empty Stack!\n");
    return 0;
  }
  return pastack->element[pastack->top];
}

/*
 * 功能：创建并初始化一个空链栈。
 * 参数：无。
 * 返回：新分配的链栈指针，top 初始化为 NULL。
 * PPT 对应：Algorithm 3.6 Initialization。
 */
PLinkStack createEmptyStack_link(void) {
  PLinkStack stack = (PLinkStack)malloc(sizeof(LinkStack));
  if (stack == NULL) {
    exit(EXIT_FAILURE);
  }
  stack->top = NULL;
  return stack;
}

/*
 * 功能：判断链栈是否为空。
 * 参数：
 *   plstack - 指向链栈的指针。
 * 返回：空栈返回 TRUE，否则返回 FALSE。
 */
int isEmptyStack_link(PLinkStack plstack) { return plstack->top == NULL; }

/*
 * 功能：向链栈栈顶压入一个新结点。
 * 参数：
 *   plstack - 指向链栈的指针。
 *   x       - 待入栈数据。
 * 返回：无；函数会动态申请一个 StackNode。
 */
void push_link(PLinkStack plstack, DataType x) {
  PStackNode node = (PStackNode)malloc(sizeof(StackNode));
  if (node == NULL) {
    exit(EXIT_FAILURE);
  }
  node->data = x;
  node->next = plstack->top;
  plstack->top = node;
}

/*
 * 功能：弹出链栈栈顶结点并释放其空间。
 * 参数：
 *   plstack - 指向链栈的指针。
 * 返回：非空时返回栈顶数据；空栈时打印提示并返回 0。
 */
DataType pop_link(PLinkStack plstack) {
  if (isEmptyStack_link(plstack)) {
    printf("Empty Stack!\n");
    return 0;
  }
  PStackNode node = plstack->top;
  DataType value = node->data;
  plstack->top = node->next;
  free(node);
  return value;
}

/*
 * 功能：读取链栈栈顶元素但不删除结点。
 * 参数：
 *   plstack - 指向链栈的指针。
 * 返回：非空时返回栈顶数据；空栈返回 0。
 */
DataType top_link(PLinkStack plstack) {
  return isEmptyStack_link(plstack) ? 0 : plstack->top->data;
}

/*
 * 功能：销毁链栈并释放所有结点。
 * 参数：
 *   stack - 待销毁链栈。
 * 返回：无；调用后 stack 指针不应继续使用。
 */
void destroy_link_stack(PLinkStack stack) {
  while (!isEmptyStack_link(stack)) {
    pop_link(stack);
  }
  free(stack);
}

/* ===== 主题演示入口 ===== */

/*
 * 函数: main
 * 功能: 分别创建顺序栈和链栈，演示入栈、取栈顶、出栈和栈空判断。
 * 参数: 无。
 * 返回: 程序正常结束返回 0。
 * PPT 对应: 栈的两种存储结构；对比数组下标 top 与链式头插/头删的实现差异。
 */
int main(void) {
  PSeqStack seq = createEmptyStack_seq();
  PLinkStack link = createEmptyStack_link();
  push_seq(seq, 10);
  push_seq(seq, 20);
  int seq_top = top_seq(seq);
  int seq_pop = pop_seq(seq);
  printf("SeqStack top=%d pop=%d\n", seq_top, seq_pop);
  push_link(link, 1);
  push_link(link, 2);
  int link_top = top_link(link);
  int link_pop = pop_link(link);
  printf("LinkStack top=%d pop=%d\n", link_top, link_pop);
  free(seq);
  destroy_link_stack(link);
  return 0;
}
