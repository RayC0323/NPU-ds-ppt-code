/*
 * 文件: DS-Chap9/02_exchange_selection_heap_sorts.c
 * 主题: 交换/选择/堆排序：冒泡、快速、直接选择和堆排序。
 * 说明: 本文件为自包含示例，包含当前主题相关的数据结构定义、操作函数、辅助函数和演示 main。
 * 运行: 直接编译本文件即可运行，不依赖 src 目录或其他章节文件。
 * 示例: gcc -std=c11 -Wall -Wextra -pedantic DS-Chap9/02_exchange_selection_heap_sorts.c -o build/DS-Chap9/02_exchange_selection_heap_sorts
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXNUM 100
#define RADIX 10

typedef int KeyType;
typedef int DataType;

/*
 * 结构体: RecordNode
 * 作用: 排序记录结构：key 是排序关键字，info 是随记录一起移动的数据。
 */
typedef struct {
    KeyType key;
    DataType info;
} RecordNode;

/*
 * 结构体: SortObject
 * 作用: 排序顺序表结构：record 保存待排序记录，n 保存记录个数。
 */
typedef struct {
    RecordNode record[MAXNUM];
    int n;  /* 记录个数 */
} SortObject;

/* ===== 当前主题相关的数据结构操作函数 ===== */

/*
 * 功能：构造排序章节统一使用的测试顺序表。
 * 参数：无。
 * 返回：包含 8 个记录的 SortObject，关键字来自 PPT 示例序列。
 */
static SortObject make_sample(void) {
    int keys[] = {49, 38, 65, 97, 13, 76, 27, 49};
    SortObject obj;
    obj.n = (int)(sizeof(keys) / sizeof(keys[0]));
    for (int i = 0; i < obj.n; ++i) {
        obj.record[i].key = keys[i];
        obj.record[i].info = keys[i];
    }
    return obj;
}

/*
 * 功能：打印排序算法运行后的关键字序列。
 * 参数：
 *   name - 当前算法名称，用于输出标签。
 *   obj  - 待打印的顺序表对象。
 * 返回：无；直接输出到标准输出。
 */
static void print_records(const char *name, const SortObject *obj) {
    printf("%s: ", name);
    for (int i = 0; i < obj->n; ++i) {
        printf("%d ", obj->record[i].key);
    }
    printf("\n");
}

/*
 * 功能：起泡排序，按关键字递增排序。
 * 参数：
 *   pvector - 指向待排序顺序表的指针。
 * 返回：无。
 * PPT 注释：每趟把较大的记录“起泡”到后部；若一趟没有交换则说明已排好序。
 */
void bubbleSort(SortObject *pvector) {
    for (int i = 0; i < pvector->n - 1; ++i) {
        int no_swap = 1;
        for (int j = 0; j < pvector->n - i - 1; ++j) {
            if (pvector->record[j + 1].key < pvector->record[j].key) {
                RecordNode temp = pvector->record[j];
                pvector->record[j] = pvector->record[j + 1];
                pvector->record[j + 1] = temp;
                no_swap = 0;
            }
        }
        if (no_swap) {
            break;
        }
    }
}

/*
 * 功能：快速排序。
 * 参数：
 *   pvector - 指向待排序顺序表的指针。
 *   l       - 当前待排序区间左端下标。
 *   r       - 当前待排序区间右端下标。
 * 返回：无。
 * PPT 注释：以枢轴记录所在的位置作分界线，将整个序列分成两个子序列。
 */
void quickSort(SortObject *pvector, int l, int r) {
    if (l >= r) {
        return;
    }
    int i = l;
    int j = r;
    RecordNode temp = pvector->record[i];  /* 枢纽点的值 */
    while (i != j) {
        while (i < j && pvector->record[j].key >= temp.key) {
            --j;
        }
        if (i < j) {
            pvector->record[i++] = pvector->record[j];
        }
        while (i < j && pvector->record[i].key <= temp.key) {
            ++i;
        }
        if (i < j) {
            pvector->record[j--] = pvector->record[i];
        }
    }
    pvector->record[i] = temp;
    quickSort(pvector, l, i - 1);
    quickSort(pvector, i + 1, r);
}

/*
 * 功能：直接选择排序。
 * 参数：
 *   pvector - 指向待排序顺序表的指针。
 * 返回：无。
 * PPT 注释：每一趟在 n-i 个记录中选取关键字最小的记录作为有序序列第 i 个记录。
 */
void selectSort(SortObject *pvector) {
    for (int i = 0; i < pvector->n - 1; ++i) {
        int k = i;
        for (int j = i + 1; j < pvector->n; ++j) {
            if (pvector->record[j].key < pvector->record[k].key) {
                k = j;
            }
        }
        if (k != i) {
            RecordNode temp = pvector->record[i];
            pvector->record[i] = pvector->record[k];
            pvector->record[k] = temp;
        }
    }
}

/*
 * 功能：计算顺序存储完全二叉树中结点 i 的左孩子下标。
 * 参数：
 *   i - 父结点下标，采用 0 基下标。
 * 返回：左孩子下标 2*i+1。
 */
static int leftChild(int i) {
    return 2 * i + 1;
}

/*
 * 功能：一次堆调整，将下标 i 的记录在 [0,n) 堆范围内向下筛选。
 * 参数：
 *   pvector - 堆所在的顺序表。
 *   i       - 待调整结点下标。
 *   n       - 当前堆的有效长度。
 * 返回：无。
 * PPT 注释：最大堆中，若当前结点关键字比较大的孩子小，则当前结点下沉。
 */
void sift(SortObject *pvector, int i, int n) {
    int child;
    RecordNode temp = pvector->record[i];
    child = leftChild(i);
    while (child < n) {
        if (child + 1 < n && pvector->record[child].key < pvector->record[child + 1].key) {
            ++child;
        }
        if (temp.key < pvector->record[child].key) {
            pvector->record[i] = pvector->record[child];
            i = child;
            child = leftChild(i);
        } else {
            break;
        }
    }
    pvector->record[i] = temp;
}

/*
 * 功能：堆排序。
 * 参数：
 *   pvector - 指向待排序顺序表的指针。
 * 返回：无。
 * PPT 注释：先构建初始最大堆，再将堆顶与最后一个元素交换并缩小堆。
 */
void heapSort(SortObject *pvector) {
    for (int i = pvector->n / 2 - 1; i >= 0; --i) {
        sift(pvector, i, pvector->n);
    }
    for (int i = pvector->n - 1; i > 0; --i) {
        RecordNode temp = pvector->record[0];
        pvector->record[0] = pvector->record[i];
        pvector->record[i] = temp;
        sift(pvector, 0, i);
    }
}

/* ===== 主题演示入口 ===== */

/*
 * 函数: main
 * 功能: 使用同一组记录分别演示冒泡排序、快速排序、简单选择排序和堆排序。
 * 参数: 无。
 * 返回: 程序正常结束返回 0。
 * PPT 对应: 交换类和选择类排序；对比相邻交换、分区递归、选择最小值和堆调整。
 */
int main(void) {
    SortObject obj = make_sample();
    bubbleSort(&obj);
    print_records("bubbleSort", &obj);
    obj = make_sample();
    quickSort(&obj, 0, obj.n - 1);
    print_records("quickSort", &obj);
    obj = make_sample();
    selectSort(&obj);
    print_records("selectSort", &obj);
    obj = make_sample();
    heapSort(&obj);
    print_records("heapSort", &obj);
    return 0;
}
