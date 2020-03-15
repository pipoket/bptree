/*
    Author: Soeun Kim
    Date: 27, Feb, 2020
    Description: B+ tree
*/

#include "bptree.h"


int maximum_key = 2;
int minimum_key = 1;
int minimum_children = 2;
NODE *root = NULL;


void set_maximum_key(int order) {
    if (order < 2) {
        printf("Input maximum key larger than 1\n");
#ifdef DEBUG
        printf("Use default maximum key %d\n", maximum_key);
        fflush(stdout);
#endif
        return;
    }

    maximum_key = order;
    minimum_key = order / 2;
    minimum_children = minimum_key + 1;
}

void initialize_tree(int order) {
    clear_tree();
    set_maximum_key(order);
    root = create_node(maximum_key);
}

void clear_tree() {
    if (root == NULL)
        return;

    KEY min_key = get_smallest_key(root);
    NODE *cursor = get_leaf(root, min_key);

    int total_key_count = 0;
    KEY *keys = (KEY*)malloc(0);
    do {
        total_key_count += cursor->key_count;
        keys = (KEY *)realloc(keys, sizeof(KEY) * total_key_count);
        for (int i = 0; i < cursor->key_count; i++) {
            keys[total_key_count - cursor->key_count + i] = cursor->keys[i];
        }
        cursor = cursor->next_leaf;
    } while(cursor != NULL);

    for (int i = 0; i < total_key_count; i++) {
#ifdef DEBUG
        printf("delete key %d\n", keys[i].key);
#endif
        delete_key_tree(root, keys[i]);
    }

    free(keys);
    free_node_mem(root, 1);
    root = NULL;
}

DATA *get_data(KEY key) {
    NODE *leaf = get_leaf(root, key);
    int idx = get_key_idx(leaf, key);
    if (idx == -1) {
#ifdef DEBUG
        printf("Cannot find key from b+ tree\n");
        fflush(stdout);
#endif
        return NULL;
    } else {
        return leaf->keys[idx].data;
    }
}

void insert_data(int key, DATA data) {
    // KEY, DATA 변경에 따라 수정 필요함
    KEY key_data = create_key_data(key, data);
    if (get_data(key_data) != NULL) {
        printf("Key %d already exists.\n", key);
        return;
    }

    NODE *leaf = get_leaf(root, key_data);
    insert_key_tree(leaf, key_data, NULL);
}

void delete_data(int key_value) {
    if (root == NULL) {
        printf("Empty tree");
        return;
    }
    
    printf("Delete key %d\n", key_value);
    KEY key = create_key(key_value);
    DATA *data = get_data(key);
    if (data == NULL) {
        printf("Cannot find key %d from b+ tree\n", key.key);
        return;
    }
    NODE *leaf = get_leaf(root, key);
    delete_key_tree(leaf, key);
    printf("Delete key %d complete\n", key_value);
}

void range_search(int min, int max) {
    // 입력받은 키값을 포함하여 검색
    KEY from = create_key(min);
    KEY to = create_key(max);
    NODE *leaf1 = get_leaf(root, from);
    NODE *leaf2 = get_leaf(root, to);

    printf("Range Search result: %d ~ %d\n", min, max);

    NODE *cursor = leaf1;
    if (cursor == NULL || min > max) {
        printf("Cannot find result.\n");
        fflush(stdout);
        return;
    }

    printf(" | ");
    do {
        int comma = 0;
        for(int i = 0; i < cursor->key_count; i++) {
            KEY key = cursor->keys[i];

            if ((compare_key(from, key) || is_key_equals(from, key)) 
                    && (compare_key(key, to) || is_key_equals(to, key))) {
                if (comma)
                    printf(", ");
                printf("%d (Data: %d)", key.key, *(key.data));
                comma = 1;
            } else {
                comma = 0;
            }
        }
        printf(" | ");
        cursor = cursor->next_leaf;
    } while(cursor != leaf2->next_leaf);

    printf("\nSearch result end\n");
    fflush(stdout);
}

void show_node(NODE *node) {
    printf("|");
    for (int i = 0; i < node->key_count; i++) {
        printf("%d", node->keys[i].key);
        if (node->keys[i].data != NULL)
            printf("(%d)", *(node->keys[i].data));
        if (i != node->key_count - 1)
            printf(", ");
    }
    printf("|\t");
}

void show_same_level_nodes(int same_level_node_count, NODE **same_level_nodes) {
    // 중복된 node의 가운데만 하나 남기고 나머지는 tab으로 표시
    int prev_idx = 0;
    NODE *prev = same_level_nodes[0];
    for(int i = 1; i < same_level_node_count + 1; i++) {
        if (i == same_level_node_count || prev != same_level_nodes[i]) {
            int center = (i - prev_idx) / 2 + prev_idx;
            for (int j = prev_idx; j < i; j++) {
                if (j != center)
                    same_level_nodes[j] = NULL;
            }
            prev_idx = i;
        }
        prev = same_level_nodes[i];
    }

    for(int i = 0; i < same_level_node_count; i++) {
        if (same_level_nodes[i] == NULL)
            printf("\t\t\t");
        else
            show_node(same_level_nodes[i]);
    }
}

void show_tree(NODE *node) {
    if (node == NULL || node->key_count == 0) {
        printf("Empty tree");
        return;
    }

    int degree = get_degree(node);

    int *node_count = (int *)malloc(sizeof(int) * degree);
    NODE ***nodes = (NODE ***)malloc(sizeof(NODE**) * degree);

    for (int i = 0; i < degree; i++) {
        get_same_level_nodes(node, i + 1, &(node_count[i]), &(nodes[i]));
    }

    for (int i = 0; i < degree; i++) {
        show_same_level_nodes(node_count[i], nodes[i]);
        printf("\n");
        free(nodes[i]);
    }

    free(node_count);
    free(nodes);
}
