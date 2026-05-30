/*
 * 文件: src/DS-Chap9.c
 * 说明: 第 9 章排序公共实现：插入、交换、选择、归并、锦标赛和基数排序。
 * 来源: 根据课程 PPT 中的代码片段整理为可运行 C11 程序。
 * 注释: 每个函数前都包含用途、参数、返回值和对应 PPT 算法说明；主题入口文件会按知识点调用这里的公共函数。
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
 * 功能：直接插入排序，按关键字递增排序。
 * 参数：
 *   pvector - 指向待排序顺序表的指针，函数会原地修改 record 数组。
 * 返回：无。
 * PPT 注释：每步将一个待排序记录按关键字大小插入到前面已排序表中的适当位置。
 */
void insertSort(SortObject *pvector) {
    for (int i = 1; i < pvector->n; ++i) {
        RecordNode temp = pvector->record[i];
        int j = i - 1;
        while (j >= 0 && temp.key < pvector->record[j].key) {
            pvector->record[j + 1] = pvector->record[j];
            --j;
        }
        pvector->record[j + 1] = temp;
    }
}

/*
 * 功能：折半插入排序。
 * 参数：
 *   pvector - 指向待排序顺序表的指针，函数会原地排序。
 * 返回：无。
 * PPT 注释：先在有序子表中用二分法确定插入位置，再整体后移记录。
 */
void BinSort(SortObject *pvector) {
    for (int i = 1; i < pvector->n; ++i) {
        RecordNode temp = pvector->record[i];
        int left = 0;
        int right = i - 1;
        while (left <= right) {
            int mid = (left + right) / 2;
            if (temp.key < pvector->record[mid].key) {
                right = mid - 1;
            } else {
                left = mid + 1;
            }
        }
        for (int j = i - 1; j >= left; --j) {
            pvector->record[j + 1] = pvector->record[j];
        }
        pvector->record[left] = temp;
    }
}

/*
 * 功能：希尔排序。
 * 参数：
 *   pvector - 指向待排序顺序表的指针。
 *   d       - 初始增量，之后每趟减半直到 0。
 * 返回：无。
 * PPT 注释：子序列不是简单逐段分割，而是由相隔某个增量的记录组成。
 */
void shellSort(SortObject *pvector, int d) {
    for (int increment = d; increment > 0; increment /= 2) {
        for (int i = increment; i < pvector->n; ++i) {
            RecordNode temp = pvector->record[i];
            int j = i - increment;
            while (j >= 0 && temp.key < pvector->record[j].key) {
                pvector->record[j + increment] = pvector->record[j];
                j -= increment;
            }
            pvector->record[j + increment] = temp;
        }
    }
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

/*
 * 功能：归并两个相邻有序子序列。
 * 参数：
 *   r    - 原始记录数组。
 *   r1   - 归并结果数组。
 *   low  - 第一个有序段起点。
 *   m    - 第一个有序段终点，第二段从 m+1 开始。
 *   high - 第二个有序段终点。
 * 返回：无；结果写入 r1[low..high]。
 */
void merge(RecordNode r[], RecordNode r1[], int low, int m, int high) {
    int i = low;
    int j = m + 1;
    int k = low;
    while (i <= m && j <= high) {
        if (r[i].key <= r[j].key) {
            r1[k++] = r[i++];
        } else {
            r1[k++] = r[j++];
        }
    }
    while (i <= m) {
        r1[k++] = r[i++];
    }
    while (j <= high) {
        r1[k++] = r[j++];
    }
}

/*
 * 功能：完成一趟 2-路归并。
 * 参数：
 *   r      - 输入数组。
 *   r1     - 输出数组。
 *   n      - 记录总数。
 *   length - 当前每个有序段的长度。
 * 返回：无。
 * PPT 注释：对相邻长度为 length 的序列进行两两归并，尾段不足时直接复制或单独归并。
 */
void mergePass(RecordNode r[], RecordNode r1[], int n, int length) {
    int i = 0;
    while (i + 2 * length - 1 < n) {
        merge(r, r1, i, i + length - 1, i + 2 * length - 1);
        i += 2 * length;
    }
    if (i + length - 1 < n - 1) {
        merge(r, r1, i, i + length - 1, n - 1);
    } else {
        for (int j = i; j < n; ++j) {
            r1[j] = r[j];
        }
    }
}

/*
 * 功能：非递归 2-路归并排序。
 * 参数：
 *   pvector - 指向待排序顺序表的指针。
 * 返回：无。
 * PPT 注释：length 从 1 开始，每两趟在原表和辅助表之间交替归并。
 */
void mergeSort(SortObject *pvector) {
    RecordNode temp[MAXNUM];
    int length = 1;
    while (length < pvector->n) {
        mergePass(pvector->record, temp, pvector->n, length);
        length *= 2;
        mergePass(temp, pvector->record, pvector->n, length);
        length *= 2;
    }
}

/*
 * 功能：锦标赛排序示例实现。
 * 参数：
 *   pvector - 指向待排序顺序表的指针。
 * 返回：无。
 * PPT 注释：树形选择排序又称锦标赛排序；这里用数组标记模拟反复选最小关键字。
 */
void tournamentSort(SortObject *pvector) {
    SortObject result;
    int used[MAXNUM] = {0};
    result.n = pvector->n;
    for (int k = 0; k < pvector->n; ++k) {
        int best = -1;
        for (int i = 0; i < pvector->n; ++i) {
            if (!used[i] && (best == -1 || pvector->record[i].key < pvector->record[best].key)) {
                best = i;
            }
        }
        used[best] = 1;
        result.record[k] = pvector->record[best];
    }
    *pvector = result;
}

/*
 * 功能：十进制 LSD 基数排序。
 * 参数：
 *   values - 待排序整数数组，要求非负。
 *   n      - 数组长度。
 * 返回：无；数组原地变为递增序列。
 * PPT 注释：每一趟按当前关键字位进行分配，再按桶序收集。
 */
void radixSort(int values[], int n) {
    int max_value = values[0];
    for (int i = 1; i < n; ++i) {
        if (values[i] > max_value) {
            max_value = values[i];
        }
    }
    for (int exp = 1; max_value / exp > 0; exp *= RADIX) {
        int output[MAXNUM];
        int count[RADIX] = {0};
        for (int i = 0; i < n; ++i) {
            ++count[(values[i] / exp) % RADIX];
        }
        for (int i = 1; i < RADIX; ++i) {
            count[i] += count[i - 1];
        }
        for (int i = n - 1; i >= 0; --i) {
            int digit = (values[i] / exp) % RADIX;
            output[--count[digit]] = values[i];
        }
        memcpy(values, output, (size_t)n * sizeof(values[0]));
    }
}

/*
 * 功能：第 9 章排序算法演示入口。
 * 参数：无。
 * 返回：0 表示程序正常结束。
 * 说明：依次运行 PPT 中出现的主要排序算法，并打印排序结果用于验证。
 */
int main(void) {
    SortObject obj;

    obj = make_sample();
    insertSort(&obj);
    print_records("insertSort", &obj);

    obj = make_sample();
    BinSort(&obj);
    print_records("BinSort", &obj);

    obj = make_sample();
    shellSort(&obj, obj.n / 2);
    print_records("shellSort", &obj);

    obj = make_sample();
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

    obj = make_sample();
    mergeSort(&obj);
    print_records("mergeSort", &obj);

    obj = make_sample();
    tournamentSort(&obj);
    print_records("tournamentSort", &obj);

    int radix_values[] = {329, 457, 657, 839, 436, 720, 355};
    int radix_n = (int)(sizeof(radix_values) / sizeof(radix_values[0]));
    radixSort(radix_values, radix_n);
    printf("radixSort: ");
    for (int i = 0; i < radix_n; ++i) {
        printf("%d ", radix_values[i]);
    }
    printf("\n");
    return 0;
}
