#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash.h"
#include "graph.h"

#define MAX_LINE_LENGTH 256
#define DEP_PATH "el-1-last/deps/" // префикс пути к папке с зависимостями
#define DEP_PSTFX ".deps"          // постфикс файла с зависимостями

/**
 * @brief
 *
 * @return int
 */

void pathWalk(char *fname, Graph *gr) // префикс для отладки программы
{

    FILE *fop;

    char *path;

    char str[MAX_LINE_LENGTH];

    size_t size = strlen(DEP_PATH) + strlen(fname) + strlen(DEP_PSTFX) + 1; //
    path = (char *)malloc(size * sizeof(char));
    sprintf(path, "%s%s%s", DEP_PATH, fname, DEP_PSTFX);

    fop = fopen(path, "r");
    if (fop == NULL)
    {
        free(path);
        return;
    }

    while (fgets(str, sizeof(str), fop) != NULL)
    {
        if (strchr(str, '"') != NULL) // пропуск строк без информации о программах
        {
            continue;
        }

        str[strcspn(str, "\n")] = 0;

        addEdge(gr, fname, str);

        pathWalk(str, gr);
    }

    fclose(fop);
    free(path);

    return;
}

/**
 * @brief Для примера - простой обход списка
 *
 * @param gr
 * @param src
 * @param cnt
 */
void graphNoDFS(Graph *gr, int src, int *cnt)
{
    GraphNode *grnode = gr->table[src];
    ++*cnt;

    Neighbor *current = grnode->neighbors;
    while (current != NULL)
    {
        int nid = current->id;
        GraphNode *ngrnode = gr->table[nid];
        if (ngrnode->isvisited == 0)
        {
            graphNoDFS(gr, nid, cnt);
        }
        current = current->next;
    }
}

/**
 * @brief Обход графа в глубину. Граф у нас ориентированный.
 *
 * @param gr граф, который будем обходить
 * @param src узел, с которого начинаем обход
 * @param cnt счетчик, для расчета посещенных узлов графа
 */
void graphDFS(Graph *gr, int src, int *cnt)
{
    GraphNode *grnode = gr->table[src];
    grnode->isvisited = 1;
    ++*cnt;

    Neighbor *current = grnode->neighbors;
    while (current != NULL)
    {
        int nid = current->id;
        GraphNode *ngrnode = gr->table[nid];
        if (ngrnode->isvisited == 0)
        {
            graphDFS(gr, nid, cnt);
        }
        current = current->next;
    }
}

int main()
{
    FILE *fp;
    FILE *fpp;

    char *lstPath = "el-1-last/lists/";

    char *srcfName = "all.src";
    char *dstfName;
    char *path;

    size_t size = strlen(lstPath) + strlen(srcfName) + 1;
    path = (char *)malloc(size * sizeof(char));
    sprintf(path, "%s%s", lstPath, srcfName);

    fp = fopen(path, "r");
    if (fp == NULL)
    {
        perror("Can`t open all.src file");
        return 1;
    }

    char line[MAX_LINE_LENGTH];

    // инициализируем граф
    Graph *graph = defineGraph();
    /**
     * Читаем файлы из списка all.src
     *
     */

    while (fgets(line, sizeof(line), fp) != NULL)
    {

        line[strcspn(line, "\n")] = 0; // удаляем символ конца строки

        // Смежный узел
        dstfName = strdup(line);

        addEdge(graph, srcfName, dstfName);

        free(path);
        size = strlen(lstPath) + strlen(line) + 1;
        path = (char *)malloc(size * sizeof(char));
        sprintf(path, "%s%s", lstPath, line);
        // printf("%s\n", path);
        /**
         * Получили имя файла из all.src - читаем
         *
         */
        fpp = fopen(path, "r");
        if (fpp == NULL)
        {
            perror("Can`t open src file");
            return 1;
        }

        /**
         * Выводим список зависимостей для программы
         *
         */
        srcfName = strdup(dstfName);

        while (fgets(line, sizeof(line), fpp) != NULL)
        {

            line[strcspn(line, "\n")] = 0; // удаляем символ конца строки
            // Читаем src файл - название файла: корень графа
            // прочитанное значение - смежный с ним узел
            dstfName = strdup(line);
            addEdge(graph, srcfName, dstfName);

            char *ffn;
            size_t lffn = strlen(line) + 1;
            ffn = (char *)malloc(lffn * sizeof(char));
            sprintf(ffn, "%s", line);

            // printf("%s\n", ffn);

            pathWalk(ffn, graph); // папки в el-1-last/deps я проходил рекурсивно,
        }
    }
    // Число уникальных элементов:
    int uniq = graph->i;
    

    // Матрица смежности:
    int *adjMatrix = (int *)calloc(uniq * uniq, sizeof(int));
    // Заполняем матрицу:
    for (int row = 0; row < uniq; row++)
    {
        GraphNode *grnode = graph->table[row];
        Neighbor *current = grnode->neighbors;
        while (current != NULL)
        {
            int column = current->id;
            adjMatrix[row * uniq + column] = 1; // заполняем матрицу смежности
            current = current->next;
        }
    }

    // Для ответа на 3 вопрос я обойду полученный граф в глубину. Каждый раз я буду заходить в узел и если я его не посещал, то увеличу счетчик на 1.
    // Этим я гарантирую, что обойду все элементы вложенных списков ровно 1 раз.
    int countLinear = 0;
    int countDFS = 0;
    graphNoDFS(graph, 0, &countLinear);
    graphDFS(graph, 0, &countDFS);

    printf("1.Число уникальных элементов в списках:%d\n", uniq+1);
    printf("\n3. Тут я сравнил сколько раз мы посетим каждый элмент, если будем собирать по списку линейно,\nлибо если пройдемся по списку как по графу, не собирая уже собранные программы.\n\n");
    printf("Число вхождений при линейной сборке: %d\n", countLinear);
    printf("Число вхождений при использовании алгоритма обхода в глубину: %d\n\n", countDFS);
    printf("Грубо оценить время, потраченное на сборку, можно разделив нацело на 8 число, полученное при алгоритме поиска в глубину. \n");
    printf("На самом деле ожидаю, что истинное значение больше, так как не всегда будет доступно для сборки ровно 8 программ и некоторые ядра могут простаивать.\n\n");
    printf("Грубая оценка: %d мин\n\n", countDFS / 8 + 1);
    printf("2. Тут я рассмотрел два вывода, более оптимальным считаю списки смежности, но вывод матрицы смежности тоже предусмотрел.\n");
    printf("Оставил по умолчанию вывод списков смежности, чтобы рассмотреть матрицы смежности, можно расскоментировать кусок кода, но стоит помнить о размере матрицы!\n\n");

    // Вывод матрицы смежности получается очень большой, можно раскомментировать и посмотреть на результат, но он очень неинформативный....
    // for (int row = 0; row < uniq; row++)
    // {
    //     for (int col = 0; col < uniq; col++)
    //     {
    //         printf("%d;", adjMatrix[row * uniq + col]);
    //     }
    //     printf("\n");
    // }

    // // По моему мнению лучше глянуть на списки смежности:
    // // <узел графа>: <смежный узел 1>, <смежный узел 2>, <смежный узел 3> ...
    for (int row = 0; row < uniq; row++)
    {
        GraphNode *grnode = graph->table[row];
        printf("%s: ", grnode->val);
        Neighbor *current = grnode->neighbors;
        while (current != NULL)
        {
            int nid = current->id;
            GraphNode *ngrnode = graph->table[nid];
            printf("%s, ", ngrnode->val);
            current = current->next;
        }
        printf("\n");
    }

    freeGraph(graph);
    free(adjMatrix);
    return 0;
}