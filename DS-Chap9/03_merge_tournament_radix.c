/*
 * 文件: DS-Chap9/03_merge_tournament_radix.c
 * 主题: 归并、锦标赛和基数排序。
 * 说明: 本文件为自包含示例，包含当前主题相关的数据结构定义、操作函数、辅助函数和演示 main。
 * 运行: 直接编译本文件即可运行，不依赖 src 目录或其他章节文件。
 * 示例: gcc -std=c11 -Wall -Wextra -pedantic DS-Chap9/03_merge_tournament_radix.c -o build/DS-Chap9/03_merge_tournament_radix
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXNUM 100
#define RADIX 10

typedef int KeyType;
typedef int DataType;

typedef struct {
    KeyType key;
    DataType info;
} RecordNode;

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

/* ===== 主题演示入口 ===== */

/*
 * 函数: main
 * 功能: 使用同一组记录分别演示归并排序、锦标赛排序和基数排序。
 * 参数: 无。
 * 返回: 程序正常结束返回 0。
 * PPT 对应: 归并、锦标赛和分配收集类排序；观察有序子序列合并和按关键字位分配。
 */
int main(void) {
    SortObject obj = make_sample();
    mergeSort(&obj);
    print_records("mergeSort", &obj);
    obj = make_sample();
    tournamentSort(&obj);
    print_records("tournamentSort", &obj);

    int values[] = {329, 457, 657, 839, 436, 720, 355};
    int n = (int)(sizeof(values) / sizeof(values[0]));
    radixSort(values, n);
    printf("radixSort: ");
    for (int i = 0; i < n; ++i) printf("%d ", values[i]);
    printf("\n");
    return 0;
}
