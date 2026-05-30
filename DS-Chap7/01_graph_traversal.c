/*
 * 文件: DS-Chap7/01_graph_traversal.c
 * 主题: 图遍历：邻接表/邻接矩阵建图、DFS 和 BFS。
 * 说明: 本文件为自包含示例，包含当前主题相关的数据结构定义、操作函数、辅助函数和演示 main。
 * 运行: 直接编译本文件即可运行，不依赖 src 目录或其他章节文件。
 * 示例: gcc -std=c11 -Wall -Wextra -pedantic DS-Chap7/01_graph_traversal.c -o build/DS-Chap7/01_graph_traversal
 */

#include <stdio.h>
#include <stdlib.h>

#define MAXV 20
#define MAXE 100
#define INF 1000000000

/*
 * 结构体: EdgeNode
 * 作用: 邻接表边结点结构：adjvex 保存邻接点下标，weight 保存边权，next 指向下一条边。
 */
typedef struct EdgeNode {
    int to;
    int weight;
    struct EdgeNode *next;
} EdgeNode;

/*
 * 结构体: Graph
 * 作用: 图结构：vexs 保存顶点，adj 保存邻接表，arcs 保存邻接矩阵，directed 标记是否有向。
 */
typedef struct {
    char vexs[MAXV];
    int vexNum;
    int arcNum;
    int directed;
    int arcs[MAXV][MAXV];      /* 邻接矩阵 */
    EdgeNode *adj[MAXV];       /* 邻接表 */
} Graph;

/*
 * 结构体: Edge
 * 作用: 边结构：u、v 表示边的两个端点，w 表示边权。
 */
typedef struct {
    int from;
    int to;
    int weight;
} Edge;

/* ===== 当前主题相关的数据结构操作函数 ===== */

/*
 * 功能：初始化图结构。
 * 参数：
 *   g        - 待初始化图指针。
 *   labels   - 顶点标签数组。
 *   n        - 顶点个数。
 *   directed - 非 0 表示有向图，0 表示无向图。
 * 返回：无。
 * 说明：同时初始化邻接矩阵 arcs 和邻接表 adj。
 */
void InitGraph(Graph *g, const char labels[], int n, int directed) {
    g->vexNum = n;
    g->arcNum = 0;
    g->directed = directed;
    for (int i = 0; i < n; ++i) {
        g->vexs[i] = labels[i];
        g->adj[i] = NULL;
        for (int j = 0; j < n; ++j) {
            g->arcs[i][j] = i == j ? 0 : INF;
        }
    }
}

/*
 * 功能：向图中加入一条边或弧。
 * 参数：
 *   g      - 图指针。
 *   from   - 起点下标。
 *   to     - 终点下标。
 *   weight - 边权。
 * 返回：无。
 * PPT 注释：邻接矩阵中设置 arcs[from][to]，邻接表中插入边结点。
 */
void AddEdge(Graph *g, int from, int to, int weight) {
    EdgeNode *node = (EdgeNode *)malloc(sizeof(EdgeNode));
    if (node == NULL) {
        exit(EXIT_FAILURE);
    }
    node->to = to;
    node->weight = weight;
    node->next = g->adj[from];
    g->adj[from] = node;
    g->arcs[from][to] = weight;
    ++g->arcNum;

    if (!g->directed) {
        EdgeNode *rev = (EdgeNode *)malloc(sizeof(EdgeNode));
        if (rev == NULL) {
            exit(EXIT_FAILURE);
        }
        rev->to = from;
        rev->weight = weight;
        rev->next = g->adj[to];
        g->adj[to] = rev;
        g->arcs[to][from] = weight;
    }
}

/*
 * 功能：释放图的邻接表结点。
 * 参数：
 *   g - 待销毁邻接表结点的图指针。
 * 返回：无。
 * 说明：图结构本身未动态分配，因此只释放每个顶点边链表。
 */
void DestroyGraph(Graph *g) {
    for (int i = 0; i < g->vexNum; ++i) {
        EdgeNode *p = g->adj[i];
        while (p != NULL) {
            EdgeNode *next = p->next;
            free(p);
            p = next;
        }
        g->adj[i] = NULL;
    }
}

/*
 * 功能：从指定顶点开始进行深度优先搜索。
 * 参数：
 *   g       - 图指针。
 *   visited - 访问标记数组。
 *   i       - DFS 起始顶点下标。
 * 返回：无；访问序列直接输出。
 * PPT 注释：为了避免一个顶点被访问多次，需要 visited 数组。
 */
void DFS(Graph *g, int visited[], int i) {
    visited[i] = 1;
    printf("%c ", g->vexs[i]);
    for (EdgeNode *p = g->adj[i]; p != NULL; p = p->next) {
        if (!visited[p->to]) {
            DFS(g, visited, p->to);
        }
    }
}

/*
 * 功能：对图进行完整深度优先遍历。
 * 参数：
 *   g - 图指针。
 * 返回：无。
 * 说明：若图非连通，会从每个未访问顶点重新启动 DFS。
 */
void traverseDFS(Graph *g) {
    int visited[MAXV] = {0};
    for (int i = 0; i < g->vexNum; ++i) {
        if (!visited[i]) {
            DFS(g, visited, i);
        }
    }
}

/*
 * 功能：从指定顶点开始进行广度优先搜索。
 * 参数：
 *   g       - 图指针。
 *   visited - 访问标记数组。
 *   start   - BFS 起始顶点下标。
 * 返回：无；访问序列直接输出。
 * PPT 注释：访问出发点后，依次访问其未访问邻接点，并借助队列扩展。
 */
void BFS(Graph *g, int visited[], int start) {
    int queue[MAXV];
    int front = 0;
    int rear = 0;
    visited[start] = 1;
    queue[rear++] = start;
    while (front < rear) {
        int v = queue[front++];
        printf("%c ", g->vexs[v]);
        for (EdgeNode *p = g->adj[v]; p != NULL; p = p->next) {
            if (!visited[p->to]) {
                visited[p->to] = 1;
                queue[rear++] = p->to;
            }
        }
    }
}

/*
 * 功能：对图进行完整广度优先遍历。
 * 参数：
 *   g - 图指针。
 * 返回：无。
 * 说明：若图非连通，会从每个未访问顶点重新启动 BFS。
 */
void traverseBFS(Graph *g) {
    int visited[MAXV] = {0};
    for (int i = 0; i < g->vexNum; ++i) {
        if (!visited[i]) {
            BFS(g, visited, i);
        }
    }
}

/* ===== 主题演示入口 ===== */

/*
 * 函数: main
 * 功能: 构造无向图，分别输出 DFS 深度优先遍历序列和 BFS 广度优先遍历序列。
 * 参数: 无。
 * 返回: 程序正常结束返回 0。
 * PPT 对应: 图的遍历；对比 DFS 的递归/栈思想和 BFS 的队列思想。
 */
int main(void) {
    Graph g;
    InitGraph(&g, "ABCDEF", 6, 0);
    AddEdge(&g, 0, 1, 6); AddEdge(&g, 0, 2, 1); AddEdge(&g, 0, 3, 5);
    AddEdge(&g, 1, 2, 5); AddEdge(&g, 1, 4, 3); AddEdge(&g, 2, 3, 5);
    AddEdge(&g, 2, 4, 6); AddEdge(&g, 2, 5, 4); AddEdge(&g, 3, 5, 2);
    AddEdge(&g, 4, 5, 6);
    printf("DFS: "); traverseDFS(&g);
    printf("\nBFS: "); traverseBFS(&g);
    printf("\n");
    DestroyGraph(&g);
    return 0;
}
