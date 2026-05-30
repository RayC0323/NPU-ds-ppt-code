/*
 * 文件: src/DS-Chap7.c
 * 说明: 第 7 章图公共实现：图遍历、最小生成树、最短路径、拓扑排序和关键路径。
 * 来源: 根据课程 PPT 中的代码片段整理为可运行 C11 程序。
 * 注释: 每个函数前都包含用途、参数、返回值和对应 PPT 算法说明；主题入口文件会按知识点调用这里的公共函数。
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

/*
 * 功能：Prim 算法求无向连通网的最小生成树。
 * 参数：
 *   g   - 无向带权图。
 *   mst - 输出最小生成树边数组。
 * 返回：写入 mst 的边数。
 * PPT 注释：每次选择连接 U 和 V-U 的最小权边。
 */
int Prim(Graph *g, Edge mst[]) {
    int selected[MAXV] = {0};
    int lowcost[MAXV];
    int parent[MAXV];
    int count = 0;
    for (int i = 0; i < g->vexNum; ++i) {
        lowcost[i] = g->arcs[0][i];
        parent[i] = lowcost[i] < INF ? 0 : -1;
    }
    selected[0] = 1;
    for (int k = 1; k < g->vexNum; ++k) {
        int min = INF;
        int v = -1;
        for (int i = 0; i < g->vexNum; ++i) {
            if (!selected[i] && lowcost[i] < min) {
                min = lowcost[i];
                v = i;
            }
        }
        if (v == -1) {
            break;
        }
        selected[v] = 1;
        mst[count++] = (Edge){parent[v], v, lowcost[v]};
        for (int i = 0; i < g->vexNum; ++i) {
            if (!selected[i] && g->arcs[v][i] < lowcost[i]) {
                lowcost[i] = g->arcs[v][i];
                parent[i] = v;
            }
        }
    }
    return count;
}

/*
 * 功能：qsort 使用的边权比较函数。
 * 参数：
 *   a, b - 指向 Edge 的泛型指针。
 * 返回：负数表示 a 权值更小，正数表示 b 权值更小，0 表示相等。
 */
static int cmp_edge(const void *a, const void *b) {
    const Edge *ea = (const Edge *)a;
    const Edge *eb = (const Edge *)b;
    return ea->weight - eb->weight;
}

/*
 * 功能：并查集查找，带路径压缩。
 * 参数：
 *   parent - 并查集父结点数组。
 *   x      - 待查找元素。
 * 返回：x 所在集合代表元。
 */
static int find_set(int parent[], int x) {
    if (parent[x] != x) {
        parent[x] = find_set(parent, parent[x]);
    }
    return parent[x];
}

/*
 * 功能：Kruskal 算法求无向连通网的最小生成树。
 * 参数：
 *   g   - 无向带权图。
 *   mst - 输出最小生成树边数组。
 * 返回：写入 mst 的边数。
 * PPT 注释：按权的非递减次序选择边，若 FIND-SET(u) != FIND-SET(v) 则加入 A。
 */
int Kruskal(Graph *g, Edge mst[]) {
    Edge edges[MAXE];
    int edge_count = 0;
    for (int i = 0; i < g->vexNum; ++i) {
        for (EdgeNode *p = g->adj[i]; p != NULL; p = p->next) {
            if (g->directed || i < p->to) {
                edges[edge_count++] = (Edge){i, p->to, p->weight};
            }
        }
    }
    qsort(edges, (size_t)edge_count, sizeof(edges[0]), cmp_edge);

    int parent[MAXV];
    for (int i = 0; i < g->vexNum; ++i) {
        parent[i] = i;
    }
    int count = 0;
    for (int i = 0; i < edge_count; ++i) {
        int ru = find_set(parent, edges[i].from);
        int rv = find_set(parent, edges[i].to);
        if (ru != rv) {
            parent[rv] = ru;
            mst[count++] = edges[i];
        }
    }
    return count;
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

/*
 * 功能：对有向无环图进行拓扑排序。
 * 参数：
 *   g     - 有向图。
 *   order - 输出拓扑序列的顶点下标数组。
 * 返回：输出顶点个数；若小于 vexNum，说明图中存在环。
 * PPT 注释：先求各顶点入度，再反复输出入度为 0 的顶点。
 */
int TopoSort(Graph *g, int order[]) {
    int indegree[MAXV] = {0};
    int stack[MAXV];
    int top = -1;
    int count = 0;
    for (int i = 0; i < g->vexNum; ++i) {
        for (EdgeNode *p = g->adj[i]; p != NULL; p = p->next) {
            ++indegree[p->to];
        }
    }
    for (int i = 0; i < g->vexNum; ++i) {
        if (indegree[i] == 0) {
            stack[++top] = i;
        }
    }
    while (top != -1) {
        int v = stack[top--];
        order[count++] = v;
        for (EdgeNode *p = g->adj[v]; p != NULL; p = p->next) {
            if (--indegree[p->to] == 0) {
                stack[++top] = p->to;
            }
        }
    }
    return count;
}

/*
 * 功能：求 AOE 网的关键路径。
 * 参数：
 *   g        - 有向无环带权图，边表示活动，权值表示持续时间。
 *   critical - 输出关键活动边数组。
 * 返回：关键活动数量。
 * PPT 注释：求各顶点最早/最迟发生时间，活动 ee == el 即为关键活动。
 */
int CriticalPath(Graph *g, Edge critical[]) {
    int order[MAXV];
    int count = TopoSort(g, order);
    int ve[MAXV] = {0};
    int vl[MAXV];
    int critical_count = 0;
    for (int i = 0; i < count; ++i) {
        int v = order[i];
        for (EdgeNode *p = g->adj[v]; p != NULL; p = p->next) {
            if (ve[v] + p->weight > ve[p->to]) {
                ve[p->to] = ve[v] + p->weight;
            }
        }
    }
    for (int i = 0; i < g->vexNum; ++i) {
        vl[i] = ve[order[count - 1]];
    }
    for (int i = count - 1; i >= 0; --i) {
        int v = order[i];
        for (EdgeNode *p = g->adj[v]; p != NULL; p = p->next) {
            if (vl[p->to] - p->weight < vl[v]) {
                vl[v] = vl[p->to] - p->weight;
            }
        }
    }
    for (int v = 0; v < g->vexNum; ++v) {
        for (EdgeNode *p = g->adj[v]; p != NULL; p = p->next) {
            int ee = ve[v];
            int el = vl[p->to] - p->weight;
            if (ee == el) {
                critical[critical_count++] = (Edge){v, p->to, p->weight};
            }
        }
    }
    return critical_count;
}

/*
 * 功能：打印边集合并统计总权值。
 * 参数：
 *   g     - 图指针，用于把下标转换为顶点标签。
 *   name  - 输出标签。
 *   edges - 待打印边数组。
 *   n     - 边数量。
 * 返回：无。
 */
static void print_edges(Graph *g, const char *name, Edge edges[], int n) {
    int total = 0;
    printf("%s: ", name);
    for (int i = 0; i < n; ++i) {
        total += edges[i].weight;
        printf("%c-%c(%d) ", g->vexs[edges[i].from], g->vexs[edges[i].to], edges[i].weight);
    }
    printf("total=%d\n", total);
}

/*
 * 功能：第 7 章图算法演示入口。
 * 参数：无。
 * 返回：0 表示程序正常结束。
 * 说明：演示图遍历、Prim、Kruskal、Dijkstra、Floyd、拓扑排序和关键路径。
 */
int main(void) {
    Graph ug;
    Graph dg;
    Graph dag;
    Edge edges[MAXE];
    int dist[MAXV];
    int path[MAXV];
    int all_dist[MAXV][MAXV];
    int all_path[MAXV][MAXV];
    int order[MAXV];

    InitGraph(&ug, "ABCDEF", 6, 0);
    AddEdge(&ug, 0, 1, 6);
    AddEdge(&ug, 0, 2, 1);
    AddEdge(&ug, 0, 3, 5);
    AddEdge(&ug, 1, 2, 5);
    AddEdge(&ug, 1, 4, 3);
    AddEdge(&ug, 2, 3, 5);
    AddEdge(&ug, 2, 4, 6);
    AddEdge(&ug, 2, 5, 4);
    AddEdge(&ug, 3, 5, 2);
    AddEdge(&ug, 4, 5, 6);

    printf("DFS: ");
    traverseDFS(&ug);
    printf("\nBFS: ");
    traverseBFS(&ug);
    printf("\n");
    print_edges(&ug, "Prim", edges, Prim(&ug, edges));
    print_edges(&ug, "Kruskal", edges, Kruskal(&ug, edges));

    InitGraph(&dg, "ABCDE", 5, 1);
    AddEdge(&dg, 0, 1, 10);
    AddEdge(&dg, 0, 3, 30);
    AddEdge(&dg, 0, 4, 100);
    AddEdge(&dg, 1, 2, 50);
    AddEdge(&dg, 2, 4, 10);
    AddEdge(&dg, 3, 2, 20);
    AddEdge(&dg, 3, 4, 60);
    dijkstra(&dg, 0, dist, path);
    printf("Dijkstra from A: ");
    for (int i = 0; i < dg.vexNum; ++i) {
        printf("%d ", dist[i] >= INF ? -1 : dist[i]);
    }
    printf("\n");
    floyd(&dg, all_dist, all_path);
    printf("Floyd A->E: %d\n", all_dist[0][4]);

    InitGraph(&dag, "012345", 6, 1);
    AddEdge(&dag, 0, 1, 3);
    AddEdge(&dag, 0, 2, 2);
    AddEdge(&dag, 1, 3, 2);
    AddEdge(&dag, 2, 3, 4);
    AddEdge(&dag, 2, 4, 3);
    AddEdge(&dag, 3, 5, 2);
    AddEdge(&dag, 4, 5, 2);
    int topo_count = TopoSort(&dag, order);
    printf("TopoSort: ");
    for (int i = 0; i < topo_count; ++i) {
        printf("%c ", dag.vexs[order[i]]);
    }
    printf("\n");
    print_edges(&dag, "CriticalPath", edges, CriticalPath(&dag, edges));

    DestroyGraph(&ug);
    DestroyGraph(&dg);
    DestroyGraph(&dag);
    (void)path;
    (void)all_path;
    return 0;
}
