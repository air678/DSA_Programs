#include <stdio.h>
#include <stdlib.h>

#define ORDER 3

typedef struct node {
    int keys[ORDER - 1];
    struct node *pointers[ORDER];
    int num_keys;
    struct node *next;
    int is_leaf;
} Node;

Node *root = NULL;

Node *create_node(int is_leaf) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (!new_node) exit(EXIT_FAILURE);
    new_node->num_keys = 0;
    new_node->next = NULL;
    new_node->is_leaf = is_leaf;
    for (int i = 0; i < ORDER; i++) new_node->pointers[i] = NULL;
    return new_node;
}

Node *search(int key) {
    Node *current = root;
    while (!current->is_leaf) {
        for (int i = 0; i < current->num_keys; i++) {
            if (key < current->keys[i]) {
                current = current->pointers[i];
                break;
            }
            if (i == current->num_keys - 1) {
                current = current->pointers[i + 1];
                break;
            }
        }
    }
    return current;
}

void insert(int key) {
    Node *leaf = search(key);
    if (leaf->num_keys < ORDER - 1) {
        int i = 0;
        while (i < leaf->num_keys && key > leaf->keys[i]) i++;
        for (int j = leaf->num_keys; j > i; j--) leaf->keys[j] = leaf->keys[j - 1];
        leaf->keys[i] = key;
        leaf->num_keys++;
    } else {
        Node *new_leaf = create_node(1);
        for (int i = 0; i < ORDER - 1; i++) new_leaf->keys[i] = leaf->keys[i + ORDER / 2];
        leaf->num_keys = ORDER / 2;
        new_leaf->num_keys = ORDER - ORDER / 2;
        new_leaf->next = leaf->next;
        leaf->next = new_leaf;
        if (key < new_leaf->keys[0]) insert(key);
        else insert(key);
    }
}

void print_tree(Node *node) {
    if (node == NULL) return;
    if (node->is_leaf) {
        for (int i = 0; i < node->num_keys; i++) printf("%d ", node->keys[i]);
        printf("-> ");
        print_tree(node->next);
    } else {
        for (int i = 0; i < node->num_keys; i++) {
            print_tree(node->pointers[i]);
            printf("%d ", node->keys[i]);
        }
        print_tree(node->pointers[node->num_keys]);
    }
}

int main() {
    root = create_node(1);
    int keys[] = {10, 20, 5, 30, 15};
    int num_keys = sizeof(keys) / sizeof(keys[0]);
    for (int i = 0; i < num_keys; i++) insert(keys[i]);
    print_tree(root);
    return 0;
}
