/*
//  kthOrder.c
//  cse2331
//
//  Created by Sean O'Donnell on 5/29/18.
//  Copyright © 2018 Sean O'Donnell. All rights reserved.
*/

#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    struct node *left;
    struct node *right;
    long data;
    long leftSize;
} node;

void btreeInsert(node *new, node **rootptr) {
    node *parent = NULL, *cursor;
    /* Find parent */
    cursor = *rootptr;
    while (cursor != NULL) {
        parent = cursor;
        if (new->data < cursor->data) {
            cursor->leftSize += 1;
            cursor = cursor->left;
        } else {
            cursor = cursor->right;
        }
    }
    /* Insert node below parent */
    if (parent == NULL) {
        *rootptr = new;
    } else if (new->data < parent->data) {
        parent->left = new;
    } else {
        parent->right = new;
    }
}

node *readTree(FILE *dataFile) {
    node *root = NULL, *new;
    long element;
    while (fscanf(dataFile, "%ld\n", &element) == 1) {
        new = malloc(sizeof(node));
        new->data = element;
        new->right = NULL;
        new->left = NULL;
        new->leftSize = 0;
        btreeInsert(new, &root);
    }
    return root;
}

long kthOrder(int k, node *root) {
    node *current;
    current = root;
    while (k != current->leftSize + 1) {
        if (k <= current->leftSize) {
            current = current->left;
        } else {
            k -= current->leftSize + 1;
            current = current->right;
        }
    }
    return current->data;
}

void reportKthOrderStats(FILE *positionFile, node *root) {
    int position;
    while (fscanf(positionFile, "%i\n", &position) == 1) {
        printf("%ld\n", kthOrder(position, root));
    }
}

int main(int argc, const char * argv[]) {
    const char *data, *range;
    FILE *dataFile, *rangeFile;
    node *root;

    data = argv[1];
    range = argv[2];
    dataFile = fopen(data, "r");
    rangeFile = fopen(range, "r");

    root = readTree(dataFile);
    reportKthOrderStats(rangeFile, root);

    return (0);
}
