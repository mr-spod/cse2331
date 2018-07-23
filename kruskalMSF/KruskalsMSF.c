/*
//  kthOrder.c
//  cse2331
//
//  Created by Sean O'Donnell on 5/29/18.
//  Copyright © 2018 Sean O'Donnell. All rights reserved.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* UNION-FIND Data Structure */

typedef struct vertexNode {
    struct vertexNode *parent;
    int vertex;
} vertexNode;

void ufMakeSet(vertexNode *node) {
    node->parent = node;
}

vertexNode *ufFindSet(vertexNode *node) {
    if (node == node->parent) return node;
    node->parent = ufFindSet(node->parent);
    return node->parent;
}

void ufUnion(vertexNode *nodeX, vertexNode *nodeY) {
    vertexNode *xHead, *yHead;
    xHead = ufFindSet(nodeX);
    yHead = ufFindSet(nodeY);
    yHead->parent = xHead;
}

/**/

/* Min-Priority Queue Data Structure */

typedef struct edgeNode {
    struct edgeNode *next;
    vertexNode *source;
    vertexNode *destination;
    int weight;
} edgeNode;

void insert(edgeNode **head, edgeNode *new) {
    edgeNode *cursor, *prev = NULL;
    cursor = *head;
    while (cursor != NULL && cursor->weight < new->weight) {
        prev = cursor;
        cursor = cursor->next;
    }
    new->next = cursor;
    if (prev != NULL) {
        prev->next = new;
    } else {
        *head = new;
    }
}

edgeNode *removeMin(edgeNode **head) {
    edgeNode *min = *head;
    *head = min->next;
    return min;
}

edgeNode **readEdges(vertexNode **vertices, FILE *graphFile) {
    edgeNode *newEdge, *firstEdge, *head;
    int source, destination, weight;
    fscanf(graphFile, "%d,%d:%d\n", &source, &destination, &weight);
    firstEdge = malloc(sizeof(edgeNode));
    firstEdge->source = vertices[source];
    firstEdge->destination = vertices[destination];
    firstEdge->weight = weight;
    head = firstEdge;
    while (fscanf(graphFile, "%d,%d:%d\n", &source, &destination, &weight) == 3) {
        newEdge = malloc(sizeof(edgeNode));
        newEdge->source = vertices[source];
        newEdge->destination = vertices[destination];
        newEdge->weight = weight;
        insert(&head, newEdge);
    }
    return &head;
}

int main(int argc, const char * argv[]) {
    FILE *graphFile;
    int nodeCount, setCount, i;
    vertexNode **vertices, *newVertex;
    edgeNode **edgesHead = NULL, *edge;
    graphFile = fopen(argv[1], "r");

    /* array of vertex pointers */
    fscanf(graphFile, "%d\n", &nodeCount);
    setCount = nodeCount;
    vertices = malloc(nodeCount * sizeof(vertexNode*));
    for (i = 0; i < nodeCount; i++) {
        newVertex = malloc(sizeof(vertexNode));
        newVertex->vertex = i;
        ufMakeSet(newVertex);
        vertices[i] = newVertex;
    }

    /* list (min-priority queue) of edges */
    edgesHead = readEdges(vertices, graphFile);
    
    /* find MSF */
    edge = *edgesHead;
    while (edge != NULL && setCount > 1) {
      vertexNode *sourceSet = ufFindSet(edge->source);
      vertexNode *destinationSet = ufFindSet(edge->destination);
      if (sourceSet != destinationSet) {
        printf("%d,%d:%d\n", edge->source->vertex, edge->destination->vertex, edge->weight);
        ufUnion(edge->source, edge->destination);
        setCount--;
      }
      edge = edge->next;
    }

    return (0);
}
