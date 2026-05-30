# 数据结构 PPT 代码复现

本目录可以作为一个独立的小型 C 项目上传到 GitHub。代码按章节和主题拆分，每个主题文件都可以单独编译运行。

## 目录结构

```text
.
├── DS-Chap2/ ... DS-Chap9/   # 按主题拆分的自包含 C 程序
├── src/                      # 每章完整演示程序，可作为总览版本
├── Makefile                  # Linux/macOS/Git Bash 构建入口
├── build.ps1                 # Windows PowerShell 构建入口
└── .github/workflows/ci.yml  # GitHub Actions 编译和运行检查
```

每个主题文件都是自包含 C 程序：文件中直接包含当前主题相关的数据结构定义、操作函数、辅助函数、详细注释和演示 `main`。因此可以只打开或提交某一个 `.c` 文件，不需要同时依赖 `src/` 目录。

## 构建

使用 GNU Make：

```sh
make all      # 编译 src 下完整章节程序和所有主题程序
make topics   # 只编译主题程序
make run      # 编译并运行所有主题程序
make clean    # 删除 build/
```

使用 Windows PowerShell：

```powershell
.\build.ps1          # 编译全部 .c
.\build.ps1 -Run     # 编译并运行主题程序
.\build.ps1 -Clean   # 清理 build/ 后重新编译
```

单独编译某个主题：

```sh
gcc -std=c11 -Wall -Wextra -pedantic DS-Chap2/04_merge_sorted_linked_list.c -o build/merge_list
```

## 主题文件

### DS-Chap2

- `01_sequential_list.c` - 顺序表基本操作
- `02_set_and_sequential_merge.c` - 集合并/交与有序顺序表归并
- `03_singly_linked_list.c` - 单链表基本操作
- `04_merge_sorted_linked_list.c` - 合并两个有序单链表
- `05_static_linked_list.c` - 静态链表备用空间链
- `06_double_linked_list.c` - 双向链表插入删除
- `07_josephus_and_polynomial.c` - Josephus 环和一元多项式加法

### DS-Chap3

- `01_stacks.c` - 顺序栈和链栈
- `02_expression_evaluation.c` - 括号匹配与表达式求值
- `03_recursion_examples.c` - 递归示例
- `04_queues.c` - 循环顺序队列和链队列

### DS-Chap5

- `01_sparse_matrix_transpose.c` - 稀疏矩阵转置
- `02_matrix_multiply_crosslist.c` - 稀疏矩阵乘法和十字链表
- `03_generalized_list.c` - 广义表

### DS-Chap6

- `01_binary_tree_traversal.c` - 二叉树遍历
- `02_threaded_binary_tree.c` - 中序线索二叉树
- `03_huffman.c` - Huffman 编码

### DS-Chap7

- `01_graph_traversal.c` - DFS/BFS
- `02_minimum_spanning_tree.c` - Prim/Kruskal
- `03_shortest_path.c` - Dijkstra/Floyd
- `04_topological_critical_path.c` - 拓扑排序和关键路径

### DS-Chap8

- `01_sequential_binary_search.c` - 顺序查找和折半查找
- `02_binary_search_tree.c` - 二叉排序树
- `03_avl_tree.c` - AVL 树
- `04_hash_table.c` - 哈希表线性探测

### DS-Chap9

- `01_insertion_sorts.c` - 插入类排序
- `02_exchange_selection_heap_sorts.c` - 交换/选择/堆排序
- `03_merge_tournament_radix.c` - 归并/锦标赛/基数排序

## 说明

- `src/DS-Chap*.c` 可以直接单独编译运行，演示整章算法。
- `DS-Chap*/NN_topic.c` 是更细粒度的自包含题目文件，便于只学习、运行或提交某个知识点。
- 主题文件中的函数按实际调用关系裁剪：保留当前主题用到的数据结构操作和辅助函数，不再只保留 `main`。
- 生成物在 `.gitignore` 中忽略，不需要提交 `build/`、`bin/` 或 `.exe`。
