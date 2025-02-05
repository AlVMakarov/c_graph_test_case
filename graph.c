#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "graph.h"
#include "hash.h"

/**
 * @brief Инициализация графа
 *
 * @return Graph*
 */
Graph *defineGraph()
{
    Graph *gr = (Graph *)malloc(sizeof(Graph));
    gr->i = 0; // счетчик - синхронизируем вызовы графа
    HashTable *hashtab = (HashTable *)malloc(sizeof(HashTable));
    gr->ht = hashtab; // инициализируем хэш-таблицу (ускориение поиска в графе)
    return gr;
}

/**
 * @brief Добавляем узел в граф
 *
 */
void addNode(Graph *gr, char *node)
{
    // Добавляем узел в граф
    GraphNode *nd = (GraphNode *)malloc(sizeof(GraphNode));
    nd->id = gr->i;
    nd->isvisited = 0;
    nd->val = strdup(node);
    nd->neighbors = NULL;
    // Добавляем узел в хэш таблицу
    add(gr->ht, node, gr->i);
    // Добавляем узел в граф, инкрементируем счетчик
    gr->table[gr->i] = nd;
    gr->i = gr->i + 1;
}

/**
 * @brief Добавляем соседей для коренного узла
 * Функция addNeighbor вызывается только из addEdge, там и делается проверка
 * на существование узлов в графе
 * @param gr
 * @param src коренной узел
 * @param dst смежный узел
 */
void addNeighbor(Graph *gr, char *src, char *dst)
{
    // Получаем значения (индексы в графе) по ключам (наименованиям программ)
    int srcgraphidx = get(gr->ht, src);
    int dstgraphidx = get(gr->ht, dst);

    GraphNode *grphnode = gr->table[srcgraphidx];
    Neighbor *current = grphnode->neighbors;
    Neighbor *neighb = (Neighbor *)malloc(sizeof(Neighbor));
    neighb->id = dstgraphidx;
    neighb->next = NULL;

    if (current == NULL)
    { // Список пуст
        grphnode->neighbors = neighb;
    }
    else
    {
        while (current->next != NULL)
        {
            if (current->id == dstgraphidx)
            {
                return;
            }
            current = current->next;
        }
        current->next = neighb;
    }
}

/**
 * @brief Добавляем ребро в граф
 *
 */
void addEdge(Graph *gr, char *src, char *dst)
{
    // Ищем индексы в хэш таблице
    int srchash = hash(src);
    int dsthash = hash(dst);
    // Получаем пары ключ-значения из хэш таблицы
    Node *srccurr = gr->ht->table[srchash];
    Node *dstcurr = gr->ht->table[dsthash];
    // Проверка на существование узла графа,
    // если узла еще не было, то и в хэш таблице его не будет
    if (srccurr == NULL)
    {
        addNode(gr, src);
    }
    if (dstcurr == NULL)
    {
        addNode(gr, dst);
    }

    addNeighbor(gr, src, dst);
}
/**
 * @brief Получаем связный список для соседей узла
 *
 * @param gr
 * @param idx
 * @return Neighbor*
 */
Neighbor *getNeighbors(Graph *gr, int idx)
{
    GraphNode *grphnode = gr->table[idx];
    Neighbor *current = grphnode->neighbors;
    return current;
}

/**
 * @brief Очистка графа
 *
 * @param gr
 */
void freeGraph(Graph *gr)
{
    int size = gr->i; // число узлов в графе
    for (int i = 0; i < size; i++)
    {
        GraphNode *current = gr->table[i];
        Neighbor *currneighb = current->neighbors;

        while (currneighb != NULL)
        {
            Neighbor *next = currneighb->next;
            free(currneighb);
            currneighb = next;
        }
        free(current);
    }
    free(gr);
}
