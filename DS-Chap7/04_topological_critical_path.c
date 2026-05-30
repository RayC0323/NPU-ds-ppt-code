/*
 * 文件: DS-Chap7/04_topological_critical_path.c
 * 主题: 有向无环图：拓扑排序和 AOE 网关键路径。
 * 说明: 本文件为自包含示例，包含当前主题相关的数据结构定义、操作函数、辅助函数和演示 main。
 * 运行: 直接编译本文件即可运行，不依赖 src 目录或其他章节文件。
 * 示例: gcc -std=c11 -Wall -Wextra -pedantic DS-Chap7/04_topological_critical_path.c -o build/DS-Chap7/04_topological_critical_path
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

/* ===== 主题演示入口 ===== */

/*
 * 函数: main
 * 功能: 构造 DAG 执行拓扑排序，再构造 AOE 网求关键路径并输出关键活动。
 * 参数: 无。
 * 返回: 程序正常结束返回 0。
 * PPT 对应: 拓扑排序与关键路径；通过入度表、最早/最迟发生时间识别关键活动。
 */
int main(void) {
    Graph dag;
    int order[MAXV];
    Edge critical[MAXE];
    InitGraph(&dag, "012345", 6, 1);
    AddEdge(&dag, 0, 1, 3); AddEdge(&dag, 0, 2, 2); AddEdge(&dag, 1, 3, 2);
    AddEdge(&dag, 2, 3, 4); AddEdge(&dag, 2, 4, 3); AddEdge(&dag, 3, 5, 2);
    AddEdge(&dag, 4, 5, 2);
    int count = TopoSort(&dag, order);
    printf("TopoSort: ");
    for (int i = 0; i < count; ++i) printf("%c ", dag.vexs[order[i]]);
    printf("\n");
    print_edges(&dag, "CriticalPath", critical, CriticalPath(&dag, critical));
    DestroyGraph(&dag);
    return 0;
}
