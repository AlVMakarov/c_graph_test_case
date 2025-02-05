#pragma once

#define TABLE_SIZE 128000 // заранее подогнал размер, так как реализация динамического размера структуры займет много времени...

/**
 * @brief Узел
 * Добавил простую обработку коллизий
 */
typedef struct Node
{
    char *key;
    int value;
    struct Node *next;
} Node;

/**
 * @brief Структура для хранения хэш таблицы
 *
 */
typedef struct HashTable
{
    Node *table[TABLE_SIZE];
} HashTable;


unsigned int hash(char *key);

HashTable *defineDict();

void add(HashTable *ht, char *key, int value);

int get(HashTable *ht, char *key);

void freeHashTable(HashTable *ht);
