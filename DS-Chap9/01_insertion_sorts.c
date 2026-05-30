/*
 * 文件: DS-Chap9/01_insertion_sorts.c
 * 主题: 插入类排序：直接插入、折半插入和希尔排序。
 * 说明: 本文件为自包含示例，包含当前主题相关的数据结构定义、操作函数、辅助函数和演示 main。
 * 运行: 直接编译本文件即可运行，不依赖 src 目录或其他章节文件。
 * 示例: gcc -std=c11 -Wall -Wextra -pedantic DS-Chap9/01_insertion_sorts.c -o build/DS-Chap9/01_insertion_sorts
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

/* ===== 主题演示入口 ===== */

/*
 * 函数: main
 * 功能: 使用同一组记录分别演示直接插入排序、折半插入排序和希尔排序。
 * 参数: 无。
 * 返回: 程序正常结束返回 0。
 * PPT 对应: 插入类排序；比较逐个插入、有序区折半定位和分组缩小增量的过程。
 */
int main(void) {
    SortObject obj = make_sample();
    insertSort(&obj);
    print_records("insertSort", &obj);
    obj = make_sample();
    BinSort(&obj);
    print_records("BinSort", &obj);
    obj = make_sample();
    shellSort(&obj, obj.n / 2);
    print_records("shellSort", &obj);
    return 0;
}
