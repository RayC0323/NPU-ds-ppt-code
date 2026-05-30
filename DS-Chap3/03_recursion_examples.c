/*
 * 文件: DS-Chap3/03_recursion_examples.c
 * 主题: 递归示例：阶乘、Fibonacci、背包问题和 Hanoi 塔。
 * 说明: 本文件为自包含示例，包含当前主题相关的数据结构定义、操作函数、辅助函数和演示 main。
 * 运行: 直接编译本文件即可运行，不依赖 src 目录或其他章节文件。
 * 示例: gcc -std=c11 -Wall -Wextra -pedantic DS-Chap3/03_recursion_examples.c -o build/DS-Chap3/03_recursion_examples
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

/*
 * 结构体: SeqStack
 * 作用: 顺序栈结构：element 数组保存栈元素，top 指向当前栈顶位置。
 */
typedef struct {
    ElemType element[MAXNUM];
    int top;
} SeqStack, *PSeqStack;

/*
 * 结构体: StackNode
 * 作用: 链栈结点结构：data 保存栈元素，next 指向下一个栈结点。
 */
typedef struct StackNode {
    DataType data;
    struct StackNode *next;
} StackNode, *PStackNode;

/*
 * 结构体: LinkStack
 * 作用: 链栈控制结构：top 指向链式栈顶结点。
 */
typedef struct {
    PStackNode top;
} LinkStack, *PLinkStack;

/*
 * 结构体: QNode
 * 作用: 链队列结点结构：data 保存队列元素，next 指向后继结点。
 */
typedef struct QNode {
    DataType data;
    struct QNode *next;
} QNode, *PQNode;

/*
 * 结构体: LinkQueue
 * 作用: 链队列控制结构：front 指向队头，rear 指向队尾。
 */
typedef struct {
    PQNode front;
    PQNode rear;
} LinkQueue, *PLinkQueue;

/*
 * 结构体: SeqQueue
 * 作用: 循环顺序队列结构：element 保存队列元素，front/rear 采用取模方式循环移动。
 */
typedef struct {
    DataType element[MAXNUM];
    int front;
    int rear;
} SeqQueue, *PSeqQueue;

/* ===== 当前主题相关的数据结构操作函数 ===== */

/*
 * 功能：递归计算 n 的阶乘。
 * 参数：
 *   n - 非负整数。
 * 返回：n!。
 * PPT 对应：Recursion and Stack 中的阶乘示例。
 */
int fact(int n) {
    return n <= 1 ? 1 : n * fact(n - 1);
}

/*
 * 功能：迭代计算 n 的阶乘。
 * 参数：
 *   n - 非负整数。
 * 返回：n!。
 * PPT 对应：递归函数到非递归函数的转换。
 */
int fact_iter(int n) {
    int result = 1;
    for (int i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

/*
 * 功能：递归计算 Fibonacci 数列第 n 项。
 * 参数：
 *   n - 序号，fib(0)=0，fib(1)=1。
 * 返回：第 n 项 Fibonacci 值。
 */
int fib(int n) {
    return n <= 1 ? n : fib(n - 1) + fib(n - 2);
}

/*
 * 功能：背包问题递归判定。
 * 参数：
 *   t       - 当前目标重量。
 *   n       - 可选物品数量，使用 weights[0..n-1]。
 *   weights - 物品重量数组。
 * 返回：存在组合凑出 t 返回 TRUE，否则返回 FALSE。
 */
int knap(int t, int n, const int weights[]) {
    if (t == 0) return TRUE;
    if (t < 0 || n == 0) return FALSE;
    return knap(t - weights[n - 1], n - 1, weights) || knap(t, n - 1, weights);
}

/*
 * 功能：递归输出 Hanoi 塔移动步骤。
 * 参数：
 *   n - 盘子数量。
 *   x - 起始柱。
 *   y - 辅助柱。
 *   z - 目标柱。
 * 返回：无；移动步骤直接输出。
 */
void Hanoi(int n, char x, char y, char z) {
    if (n == 0) {
        return;
    }
    Hanoi(n - 1, x, z, y);
    printf("%c->%c ", x, z);
    Hanoi(n - 1, y, x, z);
}

/* ===== 主题演示入口 ===== */

/*
 * 函数: main
 * 功能: 演示阶乘、Fibonacci、背包可行性搜索和 Hanoi 塔移动序列。
 * 参数: 无。
 * 返回: 程序正常结束返回 0。
 * PPT 对应: 递归算法；观察递归出口、规模缩小和回溯过程。
 */
int main(void) {
    int weights[] = {2, 4, 5, 7};
    printf("fact(5)=%d, fact_iter(5)=%d\n", fact(5), fact_iter(5));
    printf("fib(8)=%d\n", fib(8));
    printf("knap target 9: %d\n", knap(9, 4, weights));
    printf("Hanoi moves: ");
    Hanoi(3, 'A', 'B', 'C');
    printf("\n");
    return 0;
}
