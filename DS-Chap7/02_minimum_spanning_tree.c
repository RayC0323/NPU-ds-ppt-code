/*
 * 文件: DS-Chap7/02_minimum_spanning_tree.c
 * 主题: 最小生成树：Prim 和 Kruskal。
 * 说明: 本文件为自包含示例，包含当前主题相关的数据结构定义、操作函数、辅助函数和演示 main。
 * 运行: 直接编译本文件即可运行，不依赖 src 目录或其他章节文件。
 * 示例: gcc -std=c11 -Wall -Wextra -pedantic DS-Chap7/02_minimum_spanning_tree.c -o build/DS-Chap7/02_minimum_spanning_tree
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

/* ===== 主题演示入口 ===== */

/*
 * 函数: main
 * 功能: 构造带权无向图，分别用 Prim 和 Kruskal 求最小生成树并输出选边结果。
 * 参数: 无。
 * 返回: 程序正常结束返回 0。
 * PPT 对应: 最小生成树；对比 Prim 从顶点集合扩展和 Kruskal 按边权排序并查集判环。
 */
int main(void) {
    Graph g;
    Edge edges[MAXE];
    InitGraph(&g, "ABCDEF", 6, 0);
    AddEdge(&g, 0, 1, 6); AddEdge(&g, 0, 2, 1); AddEdge(&g, 0, 3, 5);
    AddEdge(&g, 1, 2, 5); AddEdge(&g, 1, 4, 3); AddEdge(&g, 2, 3, 5);
    AddEdge(&g, 2, 4, 6); AddEdge(&g, 2, 5, 4); AddEdge(&g, 3, 5, 2);
    AddEdge(&g, 4, 5, 6);
    print_edges(&g, "Prim", edges, Prim(&g, edges));
    print_edges(&g, "Kruskal", edges, Kruskal(&g, edges));
    DestroyGraph(&g);
    return 0;
}
