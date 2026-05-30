/*
 * 文件: DS-Chap8/01_sequential_binary_search.c
 * 主题: 查找表：顺序查找和折半查找。
 * 说明: 本文件为自包含示例，包含当前主题相关的数据结构定义、操作函数、辅助函数和演示 main。
 * 运行: 直接编译本文件即可运行，不依赖 src 目录或其他章节文件。
 * 示例: gcc -std=c11 -Wall -Wextra -pedantic DS-Chap8/01_sequential_binary_search.c -o build/DS-Chap8/01_sequential_binary_search
 */

#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0
#define MAXSIZE 100

typedef int KeyType;
typedef int DataType;
typedef int BOOL;

typedef struct {
    KeyType key;
    DataType info;
} Item;

typedef struct {
    Item elem[MAXSIZE];
    int n;
} SeqDictionary;

typedef struct BinNode {
    KeyType key;
    DataType data;
    struct BinNode *left;
    struct BinNode *right;
} BinNode, *PBinTree;

typedef struct AVLNode {
    KeyType key;
    DataType data;
    int height;
    struct AVLNode *left;
    struct AVLNode *right;
} AVLNode, *PAVLTree;

typedef enum { EMPTY, OCCUPIED, DELETED } HashState;

typedef struct {
    KeyType key;
    HashState state;
} HashItem;

typedef struct {
    HashItem table[MAXSIZE];
    int size;
} HashDictionary;

/* ===== 当前主题相关的数据结构操作函数 ===== */

/*
 * 功能：顺序查找。
 * 参数：
 *   pdic     - 顺序字典表指针。
 *   key      - 待查找关键码。
 *   position - 输出查找到的位置。
 * 返回：找到返回 TRUE，否则返回 FALSE。
 * PPT 注释：从表的一端开始逐个比较关键码。
 */
int seqSearch(SeqDictionary *pdic, KeyType key, int *position) {
    for (int i = 0; i < pdic->n; ++i) {
        if (pdic->elem[i].key == key) {
            *position = i;
            return TRUE;
        }
    }
    *position = -1;
    return FALSE;
}

/*
 * 功能：折半查找。
 * 参数：
 *   pdic     - 已按关键码递增排列的顺序字典表。
 *   key      - 待查找关键码。
 *   position - 输出查找到的位置。
 * 返回：找到返回 TRUE，否则返回 FALSE。
 * PPT 注释：每次与中间记录比较，将查找范围缩小一半。
 */
int dichotomySearch(SeqDictionary *pdic, KeyType key, int *position) {
    int low = 0;
    int high = pdic->n - 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        if (pdic->elem[mid].key == key) {
            *position = mid;
            return TRUE;
        }
        if (key < pdic->elem[mid].key) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    *position = -1;
    return FALSE;
}

/* ===== 主题演示入口 ===== */

/*
 * 函数: main
 * 功能: 构造顺序查找表，分别执行顺序查找和折半查找并输出查找位置。
 * 参数: 无。
 * 返回: 程序正常结束返回 0。
 * PPT 对应: 静态查找表；比较线性扫描和折半缩小区间的查找过程。
 */
int main(void) {
    SeqDictionary dict;
    int keys[] = {7, 13, 19, 29, 31};
    int position = -1;
    dict.n = 5;
    for (int i = 0; i < dict.n; ++i) {
        dict.elem[i].key = keys[i];
        dict.elem[i].info = keys[i] * 10;
    }
    int seq_found = seqSearch(&dict, 19, &position);
    printf("seqSearch(19): %d, position=%d\n", seq_found, position);
    int bin_found = dichotomySearch(&dict, 29, &position);
    printf("dichotomySearch(29): %d, position=%d\n", bin_found, position);
    return 0;
}
