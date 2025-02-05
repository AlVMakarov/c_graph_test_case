#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

/**
 * @brief Хэш функция
 *
 * @param key ключ - строка, название программы
 * @return unsigned int
 */
unsigned int hash(char *key)
{
    unsigned int hashVal = 0;
    for (int i = 0; key[i] != '\0'; i++)
    {
        hashVal = 31 * hashVal + key[i];
    }
    return hashVal % TABLE_SIZE;
}
/**
 * @brief Инициализация хэщ-таблицы
 *
 * @return HashTable*
 */
HashTable *defineDict()
{
    HashTable *ht = (HashTable *)malloc(sizeof(HashTable));
    return ht;
}
/**
 * @brief Добавляем значение в хэш таблицу
 *
 * @param ht
 * @param key
 * @param value
 */
void add(HashTable *ht, char *key, int value)
{
    unsigned int index = hash(key);
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->key = strdup(key);
    newNode->value = value;
    newNode->next = NULL;
    // Обработка коллизий
    if (ht->table[index] == NULL)
    {
        ht->table[index] = newNode;
    }
    else
    {
        Node *current = ht->table[index];
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = newNode;
    }
}
/**
 * @brief Получаем значение по ключу из хэш-таблицы
 *
 * @param ht
 * @param key
 * @return int
 */
int get(HashTable *ht, char *key)
{
    unsigned int index = hash(key);
    Node *current = ht->table[index];
    while (current != NULL)
    {
        if (strcmp(current->key, key) == 0)
        {
            return current->value;
        }
        current = current->next;
    }
    return -1;
}
/**
 * @brief Освобождаем память чтобы избежать утечек
 *
 * @param ht
 */
void freeHashTable(HashTable *ht)
{
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        Node *current = ht->table[i];
        while (current != NULL)
        {
            Node *next = current->next;
            free(current->key);
            free(current);
            current = next;
        }
    }
    free(ht->table);
}