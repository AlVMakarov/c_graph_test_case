#pragma once
#include "hash.h"
/**
 * @brief Граф - будем хранить связи между программами
 * Структура для смежных узлов
 *
 */
typedef struct Neighbor
{
    int id;
    struct Neighbor *next;
} Neighbor;
/**
 * @brief Структура для узла графа
 *
 */
typedef struct GraphNode
{
    int id;
    int isvisited;
    char *val;
    Neighbor *neighbors;
} GraphNode;

typedef struct Graph
{
    int i;
    HashTable *ht;
    GraphNode *table[TABLE_SIZE];
} Graph;

Graph *defineGraph();

void addNode(Graph *, char *);

void addNeighbor(Graph *, char *, char *);

void addEdge(Graph *, char *, char *);

Neighbor *getNeighbors(Graph *, int);

void freeGraph(Graph *);
