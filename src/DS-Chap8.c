/*
 * 文件: src/DS-Chap8.c
 * 说明: 第 8 章查找公共实现：顺序/折半查找、二叉排序树、AVL 树与散列表。
 * 来源: 根据课程 PPT 中的代码片段整理为可运行 C11 程序。
 * 注释: 每个函数前都包含用途、参数、返回值和对应 PPT 算法说明；主题入口文件会按知识点调用这里的公共函数。
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

typedef struct BinNode {
    KeyType key;
    DataType data;
    struct BinNode *left;
    struct BinNode *right;
} BinNode, *PBinTree;

/*
 * 功能：二叉排序树查找。
 * 参数：
 *   ptree    - 当前子树根指针。
 *   key      - 待查找关键码。
 *   parent   - 当前子树根的父结点。
 *   position - 输出查找结果；成功为目标结点，失败为最后访问的父结点。
 * 返回：找到返回 TRUE，否则返回 FALSE。
 */
BOOL searchBST(PBinTree ptree, KeyType key, PBinTree parent, PBinTree *position) {
    if (ptree == NULL) {
        *position = parent;
        return FALSE;
    }
    if (key == ptree->key) {
        *position = ptree;
        return TRUE;
    }
    if (key < ptree->key) {
        return searchBST(ptree->left, key, ptree, position);
    }
    return searchBST(ptree->right, key, ptree, position);
}

/*
 * 功能：向二叉排序树插入或更新结点。
 * 参数：
 *   T    - 指向当前子树根指针的指针。
 *   key  - 插入关键码。
 *   data - 数据域。
 * 返回：无；若关键码已存在则更新数据域。
 */
void InsertNode(PBinTree *T, KeyType key, DataType data) {
    if (*T == NULL) {
        *T = (PBinTree)malloc(sizeof(BinNode));
        if (*T == NULL) {
            exit(EXIT_FAILURE);
        }
        (*T)->key = key;
        (*T)->data = data;
        (*T)->left = NULL;
        (*T)->right = NULL;
        return;
    }
    if (key < (*T)->key) {
        InsertNode(&(*T)->left, key, data);
    } else if (key > (*T)->key) {
        InsertNode(&(*T)->right, key, data);
    } else {
        (*T)->data = data;
    }
}

/*
 * 功能：删除当前根结点。
 * 参数：
 *   p - 指向待删除结点指针的指针。
 * 返回：无。
 * 说明：分三种情况处理：无右子树、无左子树、左右子树均存在。
 */
static void Delete(PBinTree *p) {
    PBinTree q;
    PBinTree s;
    if ((*p)->right == NULL) {
        q = *p;
        *p = (*p)->left;
        free(q);
    } else if ((*p)->left == NULL) {
        q = *p;
        *p = (*p)->right;
        free(q);
    } else {
        q = *p;
        s = (*p)->left;
        while (s->right != NULL) {
            q = s;
            s = s->right;
        }
        (*p)->key = s->key;
        (*p)->data = s->data;
        if (q != *p) {
            q->right = s->left;
        } else {
            q->left = s->left;
        }
        free(s);
    }
}

/*
 * 功能：删除二叉排序树中关键码等于 key 的结点。
 * 参数：
 *   T   - 指向当前子树根指针的指针。
 *   key - 待删除关键码。
 * 返回：删除成功返回 TRUE，未找到返回 FALSE。
 */
BOOL DeleteBST(PBinTree *T, KeyType key) {
    if (*T == NULL) {
        return FALSE;
    }
    if (key == (*T)->key) {
        Delete(T);
        return TRUE;
    }
    if (key < (*T)->key) {
        return DeleteBST(&(*T)->left, key);
    }
    return DeleteBST(&(*T)->right, key);
}

/*
 * 功能：中序打印二叉排序树。
 * 参数：
 *   T - 二叉排序树根指针。
 * 返回：无；输出结果应为递增关键码序列。
 */
void inorderBST(PBinTree T) {
    if (T == NULL) {
        return;
    }
    inorderBST(T->left);
    printf("%d ", T->key);
    inorderBST(T->right);
}

/*
 * 功能：销毁二叉排序树。
 * 参数：
 *   T - 待释放二叉排序树根指针。
 * 返回：无。
 */
void destroyBST(PBinTree T) {
    if (T == NULL) {
        return;
    }
    destroyBST(T->left);
    destroyBST(T->right);
    free(T);
}

typedef struct AVLNode {
    KeyType key;
    DataType data;
    int height;
    struct AVLNode *left;
    struct AVLNode *right;
} AVLNode, *PAVLTree;

/*
 * 功能：取得 AVL 结点高度。
 * 参数：
 *   T - AVL 结点指针。
 * 返回：空结点高度为 0，非空返回其 height 字段。
 */
static int height(PAVLTree T) {
    return T == NULL ? 0 : T->height;
}

/*
 * 功能：返回两个整数中的较大值。
 * 参数：
 *   a, b - 待比较整数。
 * 返回：较大者。
 */
static int max_int(int a, int b) {
    return a > b ? a : b;
}

/*
 * 功能：根据左右子树高度更新 AVL 结点高度。
 * 参数：
 *   T - 待更新结点。
 * 返回：无。
 */
static void updateHeight(PAVLTree T) {
    T->height = max_int(height(T->left), height(T->right)) + 1;
}

/*
 * 功能：AVL 树 LL 型调整，即右旋。
 * 参数：
 *   a - 失衡子树根结点。
 * 返回：旋转后的新根结点。
 */
PAVLTree LL(PAVLTree a) {
    PAVLTree b = a->left;
    a->left = b->right;
    b->right = a;
    updateHeight(a);
    updateHeight(b);
    return b;
}

/*
 * 功能：AVL 树 RR 型调整，即左旋。
 * 参数：
 *   a - 失衡子树根结点。
 * 返回：旋转后的新根结点。
 */
PAVLTree RR(PAVLTree a) {
    PAVLTree b = a->right;
    a->right = b->left;
    b->left = a;
    updateHeight(a);
    updateHeight(b);
    return b;
}

/*
 * 功能：AVL 树 LR 型调整。
 * 参数：
 *   a - 失衡子树根结点。
 * 返回：调整后的新根。
 * PPT 注释：先对左子树 RR，再对根 LL。
 */
PAVLTree LR(PAVLTree a) {
    a->left = RR(a->left);
    return LL(a);
}

/*
 * 功能：AVL 树 RL 型调整。
 * 参数：
 *   a - 失衡子树根结点。
 * 返回：调整后的新根。
 * PPT 注释：先对右子树 LL，再对根 RR。
 */
PAVLTree RL(PAVLTree a) {
    a->right = LL(a->right);
    return RR(a);
}

/*
 * 功能：向 AVL 树插入关键码，并在回溯时保持平衡。
 * 参数：
 *   T    - 当前子树根结点。
 *   key  - 插入关键码。
 *   data - 数据域。
 * 返回：插入并调整后的当前子树根。
 */
PAVLTree InsertAVL(PAVLTree T, KeyType key, DataType data) {
    if (T == NULL) {
        T = (PAVLTree)malloc(sizeof(AVLNode));
        if (T == NULL) {
            exit(EXIT_FAILURE);
        }
        T->key = key;
        T->data = data;
        T->height = 1;
        T->left = NULL;
        T->right = NULL;
        return T;
    }
    if (key < T->key) {
        T->left = InsertAVL(T->left, key, data);
        if (height(T->left) - height(T->right) == 2) {
            T = key < T->left->key ? LL(T) : LR(T);
        }
    } else if (key > T->key) {
        T->right = InsertAVL(T->right, key, data);
        if (height(T->right) - height(T->left) == 2) {
            T = key > T->right->key ? RR(T) : RL(T);
        }
    } else {
        T->data = data;
    }
    updateHeight(T);
    return T;
}

/*
 * 功能：中序打印 AVL 树。
 * 参数：
 *   T - AVL 树根指针。
 * 返回：无。
 */
void inorderAVL(PAVLTree T) {
    if (T == NULL) {
        return;
    }
    inorderAVL(T->left);
    printf("%d ", T->key);
    inorderAVL(T->right);
}

/*
 * 功能：销毁 AVL 树。
 * 参数：
 *   T - 待释放 AVL 树根指针。
 * 返回：无。
 */
void destroyAVL(PAVLTree T) {
    if (T == NULL) {
        return;
    }
    destroyAVL(T->left);
    destroyAVL(T->right);
    free(T);
}

typedef enum { EMPTY, OCCUPIED, DELETED } HashState;

typedef struct {
    KeyType key;
    HashState state;
} HashItem;

typedef struct {
    HashItem table[MAXSIZE];
    int size;
} HashDictionary;

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

/*
 * 功能：第 8 章查找算法演示入口。
 * 参数：无。
 * 返回：0 表示程序正常结束。
 * 说明：演示顺序/折半查找、二叉排序树、AVL 树和哈希表线性探测。
 */
int main(void) {
    SeqDictionary dict;
    int keys[] = {7, 13, 19, 29, 31};
    int position;
    dict.n = 5;
    for (int i = 0; i < dict.n; ++i) {
        dict.elem[i].key = keys[i];
        dict.elem[i].info = keys[i] * 10;
    }
    int found_seq = seqSearch(&dict, 19, &position);
    printf("seqSearch(19): %d, position=%d\n", found_seq, position);
    int found_bin = dichotomySearch(&dict, 29, &position);
    printf("dichotomySearch(29): %d, position=%d\n", found_bin, position);

    PBinTree bst = NULL;
    int bst_keys[] = {45, 24, 53, 12, 37, 93, 28};
    for (int i = 0; i < 7; ++i) {
        InsertNode(&bst, bst_keys[i], bst_keys[i] * 10);
    }
    PBinTree found = NULL;
    printf("searchBST(37): %d\n", searchBST(bst, 37, NULL, &found));
    DeleteBST(&bst, 24);
    printf("BST after delete 24: ");
    inorderBST(bst);
    printf("\n");

    PAVLTree avl = NULL;
    int avl_keys[] = {30, 20, 40, 10, 25, 22, 50};
    for (int i = 0; i < 7; ++i) {
        avl = InsertAVL(avl, avl_keys[i], avl_keys[i]);
    }
    printf("AVL inorder: ");
    inorderAVL(avl);
    printf("\nAVL root=%d, height=%d\n", avl->key, avl->height);

    HashDictionary hash_table;
    int hash_keys[] = {19, 14, 23, 1, 68, 20, 84};
    initHash(&hash_table, 11);
    for (int i = 0; i < 7; ++i) {
        linearInsert(&hash_table, hash_keys[i]);
    }
    int found_hash = linearSearch(&hash_table, 68, &position);
    printf("linearSearch hash(68): %d, position=%d\n", found_hash, position);

    destroyBST(bst);
    destroyAVL(avl);
    return 0;
}
