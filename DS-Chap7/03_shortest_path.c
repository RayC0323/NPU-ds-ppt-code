/*
 * 文件: DS-Chap7/03_shortest_path.c
 * 主题: 最短路径：Dijkstra 和 Floyd。
 * 说明: 本文件为自包含示例，包含当前主题相关的数据结构定义、操作函数、辅助函数和演示 main。
 * 运行: 直接编译本文件即可运行，不依赖 src 目录或其他章节文件。
 * 示例: gcc -std=c11 -Wall -Wextra -pedantic DS-Chap7/03_shortest_path.c -o build/DS-Chap7/03_shortest_path
 */

#include <stdio.h>
#include <stdlib.h>

#define MAXV 20
#define MAXE 100
#define INF 1000000000

typedef struct EdgeNode {
    int to;
    int weight;
    struct EdgeNode *next;
} EdgeNode;

typedef struct {
    char vexs[MAXV];
    int vexNum;
    int arcNum;
    int directed;
    int arcs[MAXV][MAXV];      /* 邻接矩阵 */
    EdgeNode *adj[MAXV];       /* 邻接表 */
} Graph;

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
 * 功能：Dijkstra 单源最短路径算法。
 * 参数：
 *   graph - 有向带权图，权值非负。
 *   start - 源点下标。
 *   dist  - 输出从 start 到各顶点的最短距离。
 *   path  - 输出最短路径中每个顶点的前驱顶点下标。
 * 返回：无。
 * PPT 注释：求从 v0 出发到图上其余各顶点可能达到的最短路径长度。
 */
void dijkstra(Graph *graph, int start, int dist[], int path[]) {
    int s[MAXV] = {0};
    for (int i = 0; i < graph->vexNum; ++i) {
        dist[i] = graph->arcs[start][i];
        path[i] = (dist[i] < INF && i != start) ? start : -1;
    }
    s[start] = 1;
    dist[start] = 0;
    for (int i = 1; i < graph->vexNum; ++i) {
        int min = INF;
        int minvex = -1;
        for (int j = 0; j < graph->vexNum; ++j) {
            if (!s[j] && dist[j] < min) {
                min = dist[j];
                minvex = j;
            }
        }
        if (minvex == -1) {
            break;
        }
        s[minvex] = 1;
        for (int j = 0; j < graph->vexNum; ++j) {
            if (!s[j] && graph->arcs[minvex][j] < INF &&
                dist[minvex] + graph->arcs[minvex][j] < dist[j]) {
                dist[j] = dist[minvex] + graph->arcs[minvex][j];
                path[j] = minvex;
            }
        }
    }
}

/*
 * 功能：Floyd 任意两点间最短路径算法。
 * 参数：
 *   g    - 有向带权图。
 *   dist - 输出任意两点间最短距离矩阵。
 *   path - 输出路径前驱矩阵。
 * 返回：无。
 */
void floyd(Graph *g, int dist[MAXV][MAXV], int path[MAXV][MAXV]) {
    for (int i = 0; i < g->vexNum; ++i) {
        for (int j = 0; j < g->vexNum; ++j) {
            dist[i][j] = g->arcs[i][j];
            path[i][j] = (i != j && dist[i][j] < INF) ? i : -1;
        }
    }
    for (int k = 0; k < g->vexNum; ++k) {
        for (int i = 0; i < g->vexNum; ++i) {
            for (int j = 0; j < g->vexNum; ++j) {
                if (dist[i][k] < INF && dist[k][j] < INF &&
                    dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    path[i][j] = path[k][j];
                }
            }
        }
    }
}

/* ===== 主题演示入口 ===== */

/*
 * 函数: main
 * 功能: 构造带权有向图，演示 Dijkstra 单源最短路径和 Floyd 任意两点最短路径。
 * 参数: 无。
 * 返回: 程序正常结束返回 0。
 * PPT 对应: 最短路径；理解 dist/path 数组在松弛过程中的含义。
 */
int main(void) {
    Graph g;
    int dist[MAXV], path[MAXV], all_dist[MAXV][MAXV], all_path[MAXV][MAXV];
    InitGraph(&g, "ABCDE", 5, 1);
    AddEdge(&g, 0, 1, 10); AddEdge(&g, 0, 3, 30); AddEdge(&g, 0, 4, 100);
    AddEdge(&g, 1, 2, 50); AddEdge(&g, 2, 4, 10); AddEdge(&g, 3, 2, 20);
    AddEdge(&g, 3, 4, 60);
    dijkstra(&g, 0, dist, path);
    printf("Dijkstra from A: ");
    for (int i = 0; i < g.vexNum; ++i) printf("%d ", dist[i] >= INF ? -1 : dist[i]);
    printf("\n");
    floyd(&g, all_dist, all_path);
    printf("Floyd A->E: %d\n", all_dist[0][4]);
    DestroyGraph(&g);
    return 0;
}
