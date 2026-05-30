/*
 * 文件: src/DS-Chap3.c
 * 说明: 第 3 章栈、队列与递归公共实现：顺序栈、链栈、表达式求值、递归示例和队列。
 * 来源: 根据课程 PPT 中的代码片段整理为可运行 C11 程序。
 * 注释: 每个函数前都包含用途、参数、返回值和对应 PPT 算法说明；主题入口文件会按知识点调用这里的公共函数。
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
int isEmptyStack_seq(PSeqStack pastack) {
    return pastack->top == -1;
}

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

typedef struct StackNode {
    DataType data;
    struct StackNode *next;
} StackNode, *PStackNode;

typedef struct {
    PStackNode top;
} LinkStack, *PLinkStack;

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
int isEmptyStack_link(PLinkStack plstack) {
    return plstack->top == NULL;
}

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

/*
 * 功能：括号匹配检查。
 * 参数：
 *   text - 待检查的字符串。
 * 返回：括号正确嵌套并全部匹配返回 TRUE，否则返回 FALSE。
 * 说明：使用字符栈保存尚未匹配的左括号。
 */
int bracketMatching(const char *text) {
    char stack[MAXNUM];
    int top = -1;
    for (int i = 0; text[i] != '\0'; ++i) {
        char ch = text[i];
        if (ch == '(' || ch == '[' || ch == '{') {
            stack[++top] = ch;
        } else if (ch == ')' || ch == ']' || ch == '}') {
            if (top == -1) {
                return FALSE;
            }
            char left = stack[top--];
            if ((left == '(' && ch != ')') ||
                (left == '[' && ch != ']') ||
                (left == '{' && ch != '}')) {
                return FALSE;
            }
        }
    }
    return top == -1;
}

/*
 * 功能：返回运算符优先级。
 * 参数：
 *   op - 运算符字符，支持 +、-、*、/。
 * 返回：加减运算符返回 1，乘除运算符返回 2，其他字符返回 0。
 */
static int precedence(char op) {
    return (op == '+' || op == '-') ? 1 : (op == '*' || op == '/') ? 2 : 0;
}

/*
 * 功能：执行一次二元算术运算。
 * 参数：
 *   left  - 左操作数。
 *   right - 右操作数。
 *   op    - 运算符，支持 +、-、*、/。
 * 返回：运算结果。
 */
static double apply(double left, double right, char op) {
    if (op == '+') return left + right;
    if (op == '-') return left - right;
    if (op == '*') return left * right;
    return left / right;
}

/*
 * 功能：计算中缀表达式的值。
 * 参数：
 *   expr - 中缀表达式字符串，支持数字、括号和 + - * /。
 * 返回：表达式计算结果。
 * PPT 对应：算法“求中缀表达式值”，使用运算符栈和操作数栈。
 */
double EvaluateExpression(const char *expr) {
    double values[MAXNUM];
    char ops[MAXNUM];
    int vtop = -1;
    int otop = -1;
    for (int i = 0; expr[i] != '\0';) {
        if (isspace((unsigned char)expr[i])) {
            ++i;
        } else if (isdigit((unsigned char)expr[i])) {
            char *end = NULL;
            values[++vtop] = strtod(&expr[i], &end);
            i = (int)(end - expr);
        } else if (expr[i] == '(') {
            ops[++otop] = expr[i++];
        } else if (expr[i] == ')') {
            while (otop >= 0 && ops[otop] != '(') {
                double r = values[vtop--];
                double l = values[vtop--];
                values[++vtop] = apply(l, r, ops[otop--]);
            }
            --otop;
            ++i;
        } else {
            char op = expr[i++];
            while (otop >= 0 && precedence(ops[otop]) >= precedence(op)) {
                double r = values[vtop--];
                double l = values[vtop--];
                values[++vtop] = apply(l, r, ops[otop--]);
            }
            ops[++otop] = op;
        }
    }
    while (otop >= 0) {
        double r = values[vtop--];
        double l = values[vtop--];
        values[++vtop] = apply(l, r, ops[otop--]);
    }
    return values[vtop];
}

/*
 * 功能：将中缀表达式转换为后缀表达式。
 * 参数：
 *   expr - 输入中缀表达式。
 *   out  - 输出缓冲区，保存以空格分隔的后缀表达式。
 * 返回：无。
 * PPT 对应：Application 4: Infix Expression to Postfix。
 */
void postfix(const char *expr, char *out) {
    char ops[MAXNUM];
    int otop = -1;
    int pos = 0;
    for (int i = 0; expr[i] != '\0';) {
        if (isspace((unsigned char)expr[i])) {
            ++i;
        } else if (isdigit((unsigned char)expr[i])) {
            while (isdigit((unsigned char)expr[i])) {
                out[pos++] = expr[i++];
            }
            out[pos++] = ' ';
        } else if (expr[i] == '(') {
            ops[++otop] = expr[i++];
        } else if (expr[i] == ')') {
            while (otop >= 0 && ops[otop] != '(') {
                out[pos++] = ops[otop--];
                out[pos++] = ' ';
            }
            --otop;
            ++i;
        } else {
            char op = expr[i++];
            while (otop >= 0 && precedence(ops[otop]) >= precedence(op)) {
                out[pos++] = ops[otop--];
                out[pos++] = ' ';
            }
            ops[++otop] = op;
        }
    }
    while (otop >= 0) {
        out[pos++] = ops[otop--];
        out[pos++] = ' ';
    }
    out[pos] = '\0';
}

/*
 * 功能：计算后缀表达式的值。
 * 参数：
 *   expr - 以空格分隔的后缀表达式字符串。
 * 返回：表达式计算结果。
 * 说明：遇到数字入栈，遇到运算符弹出两个操作数并将结果入栈。
 */
double EvaluateExpression_postfix(const char *expr) {
    double stack[MAXNUM];
    int top = -1;
    char copy[256];
    strcpy(copy, expr);
    char *token = strtok(copy, " ");
    while (token != NULL) {
        if (strlen(token) == 1 && strchr("+-*/", token[0]) != NULL) {
            double r = stack[top--];
            double l = stack[top--];
            stack[++top] = apply(l, r, token[0]);
        } else {
            stack[++top] = atof(token);
        }
        token = strtok(NULL, " ");
    }
    return stack[top];
}

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

typedef struct QNode {
    DataType data;
    struct QNode *next;
} QNode, *PQNode;

typedef struct {
    PQNode front;
    PQNode rear;
} LinkQueue, *PLinkQueue;

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

typedef struct {
    DataType element[MAXNUM];
    int front;
    int rear;
} SeqQueue, *PSeqQueue;

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

/*
 * 功能：第 3 章栈、队列和递归算法演示入口。
 * 参数：无。
 * 返回：0 表示程序正常结束。
 * 说明：演示顺序栈、链栈、表达式求值、递归、链队列和循环顺序队列。
 */
int main(void) {
    PSeqStack seq_stack = createEmptyStack_seq();
    PLinkStack link_stack = createEmptyStack_link();
    PSeqQueue seq_queue = createEmptyQueue_seq();
    PLinkQueue link_queue = createEmptyQueue_link();
    char post[256];
    const char *expr = "2*3/(2-1)+5*(4-1)";
    int weights[] = {2, 4, 5, 7};

    push_seq(seq_stack, 10);
    push_seq(seq_stack, 20);
    printf("SeqStack pop: %d\n", pop_seq(seq_stack));
    push_link(link_stack, 1);
    push_link(link_stack, 2);
    printf("LinkStack top: %d\n", top_link(link_stack));
    printf("Bracket match: %d\n", bracketMatching("{[()]}"));
    postfix(expr, post);
    printf("Infix value: %.0f\n", EvaluateExpression(expr));
    printf("Postfix: %s value=%.0f\n", post, EvaluateExpression_postfix(post));
    printf("fact(5)=%d, fact_iter(5)=%d\n", fact(5), fact_iter(5));
    printf("fib(8)=%d\n", fib(8));
    printf("knap target 9 in {2,4,5,7}: %d\n", knap(9, 4, weights));
    printf("Hanoi moves: ");
    Hanoi(3, 'A', 'B', 'C');
    printf("\n");

    enQueue_seq(seq_queue, 11);
    enQueue_seq(seq_queue, 22);
    printf("SeqQueue deQueue: %d\n", deQueue_seq(seq_queue));
    enQueue_link(link_queue, 33);
    enQueue_link(link_queue, 44);
    printf("LinkQueue front: %d\n", frontQueue_link(link_queue));

    free(seq_stack);
    destroy_link_stack(link_stack);
    free(seq_queue);
    destroy_link_queue(link_queue);
    return 0;
}
