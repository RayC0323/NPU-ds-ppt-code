/*
 * 文件: DS-Chap8/04_hash_table.c
 * 主题: 哈希表：线性探测插入和查找。
 * 说明: 本文件为自包含示例，包含当前主题相关的数据结构定义、操作函数、辅助函数和演示 main。
 * 运行: 直接编译本文件即可运行，不依赖 src 目录或其他章节文件。
 * 示例: gcc -std=c11 -Wall -Wextra -pedantic DS-Chap8/04_hash_table.c -o build/DS-Chap8/04_hash_table
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
 * 功能：哈希函数。
 * 参数：
 *   key  - 关键码。
 *   size - 哈希表容量。
 * 返回：key 对应的初始哈希地址。
 */
static int hash(KeyType key, int size) {
    return key % size;
}

/*
 * 功能：初始化哈希表。
 * 参数：
 *   phash - 哈希表指针。
 *   size  - 哈希表容量。
 * 返回：无；所有槽位状态被置为 EMPTY。
 */
void initHash(HashDictionary *phash, int size) {
    phash->size = size;
    for (int i = 0; i < size; ++i) {
        phash->table[i].state = EMPTY;
    }
}

/*
 * 功能：哈希表线性探测查找。
 * 参数：
 *   phash    - 哈希表指针。
 *   key      - 待查找关键码。
 *   position - 输出查找成功的位置。
 * 返回：找到返回 TRUE，否则返回 FALSE。
 * PPT 注释：按造表时设定的冲突处理方法找“下一地址”。
 */
int linearSearch(HashDictionary *phash, KeyType key, int *position) {
    int start = hash(key, phash->size);
    for (int step = 0; step < phash->size; ++step) {
        int pos = (start + step) % phash->size;
        if (phash->table[pos].state == EMPTY) {
            *position = -1;
            return FALSE;
        }
        if (phash->table[pos].state == OCCUPIED && phash->table[pos].key == key) {
            *position = pos;
            return TRUE;
        }
    }
    *position = -1;
    return FALSE;
}

/*
 * 功能：哈希表线性探测插入。
 * 参数：
 *   phash - 哈希表指针。
 *   key   - 待插入关键码。
 * 返回：插入成功或关键码已存在返回 TRUE；表满返回 FALSE。
 */
int linearInsert(HashDictionary *phash, KeyType key) {
    int start = hash(key, phash->size);
    for (int step = 0; step < phash->size; ++step) {
        int pos = (start + step) % phash->size;
        if (phash->table[pos].state != OCCUPIED) {
            phash->table[pos].key = key;
            phash->table[pos].state = OCCUPIED;
            return TRUE;
        }
        if (phash->table[pos].key == key) {
            return TRUE;
        }
    }
    return FALSE;
}

/* ===== 主题演示入口 ===== */

/*
 * 函数: main
 * 功能: 初始化散列表，用线性探测插入关键字，并查找指定关键字的位置。
 * 参数: 无。
 * 返回: 程序正常结束返回 0。
 * PPT 对应: 散列表开放定址法；观察冲突后按线性探测寻找下一个空槽。
 */
int main(void) {
    HashDictionary table;
    int keys[] = {19, 14, 23, 1, 68, 20, 84};
    int position = -1;
    initHash(&table, 11);
    for (int i = 0; i < 7; ++i) linearInsert(&table, keys[i]);
    int found = linearSearch(&table, 68, &position);
    printf("linearSearch hash(68): %d, position=%d\n", found, position);
    return 0;
}
