/*
 * 文件: DS-Chap3/02_expression_evaluation.c
 * 主题: 括号匹配、中缀表达式求值、中缀转后缀和后缀表达式求值。
 * 说明:
 * 本文件为自包含示例，包含当前主题相关的数据结构定义、操作函数、辅助函数和演示
 * main。 运行: 直接编译本文件即可运行，不依赖 src 目录或其他章节文件。 示例:
 * gcc -std=c11 -Wall -Wextra -pedantic DS-Chap3/02_expression_evaluation.c -o
 * build/DS-Chap3/02_expression_evaluation
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
      if ((left == '(' && ch != ')') || (left == '[' && ch != ']') ||
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

/* ===== 主题演示入口 ===== */

/*
 * 函数: main
 * 功能: 演示括号匹配、中缀表达式求值、中缀转后缀，以及后缀表达式求值。
 * 参数: 无。
 * 返回: 程序正常结束返回 0。
 * PPT 对应: 栈的表达式应用；重点理解操作数栈、运算符栈和优先级比较规则。
 */
int main(void) {
  const char *expr = "2*3/(2-1)+5*(4-1)";
  char post[256];
  postfix(expr, post);
  printf("Bracket match: %d\n", bracketMatching("{[()]}"));
  printf("Infix value: %.0f\n", EvaluateExpression(expr));
  printf("Postfix: %s value=%.0f\n", post, EvaluateExpression_postfix(post));
  return 0;
}
