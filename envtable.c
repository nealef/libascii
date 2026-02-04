/**
 * @file envtable.c
 * @brief Implement a simple hash table based storage mechanism
 *
 * Environment variables are held in EBCDIC by OE so we need to 
 * convert them to and from ASCII/EBCDIC. The addresses we return
 * via getenv need to be fixed (unless we unset a variable). So
 * we have to create a cache that we act on.
 *
 */
 
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <strings.h>
#include "global_a.h"
#include "envtable.h"
 
#define TABLE_SIZE 100
 
/**
 * Structure for retrieving multiple variables
 */
typedef struct {
   char    *prefix;          // Variable name prefix
   char    *res;             // Result area
   uint8_t flagBit;          // Flag bit value
   uint8_t size;             // Size of result
} varStubs_t;
 
static uint32_t hash(const char *);
 
/**
 *
 * @brief
 *
 * A simple hash function: sumps up the character values of 
 * the characters in the key and takes the modulo of the table 
 * size to get the bucket index.                        
 *
 * @param[in] key The key (variable name)
 * @return index into the table
 *
 */
 
static uint32_t
hash(const char *key)
{
    uint32_t hash_val = 0;
    while (*key != '\0') {
        hash_val = (hash_val * 31) + (*key);
        key++;
    }
    return hash_val % TABLE_SIZE;
}
 
/**
 *
 * @brief Initialize the hash table by setting all bucket pointers to NULL
 *
 * @param[in] ht Pointer to the hash table
 *
 */
 
void
htInitTable(hashTable_t *ht)
{
    if (ht == NULL) return;

    for (int i = 0; i < TABLE_SIZE; i++) {
        ht->buckets[i] = NULL;
    }
}
 
/**
 *
 * @brief Add (or update) a key-value pair. If the key value its 
 * value is updated, otherwise a new pair is added
 *
 * @param[in] ht    hash table
 * @param[in] key   variable name
 * @param[in] value variable value
 *
 * @return pointer to the value stored
 */
 
char *
htAddValue(hashTable_t *ht, const char *key, char *value, int change)
{
    if (ht == NULL || key == NULL || value == NULL) return NULL;
 
    uint32_t index = hash(key);
    keyValuePair_t *current = ht->buckets[index];
    keyValuePair_t *prev = NULL;
 
    /*
     * Traverse the linked list at this bucket to check if key already exists
     */
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            // Key found: update its value
            if (change == 1) {
                free(current->value); // Free old value memory
                current->value = strdup(value); // Duplicate and assign new value
                if (current->value == NULL) {
                    perror("Failed to duplicate value for update");
                    return NULL;
                }
            }
            return current->value;
        }
        prev = current;
        current = current->next;
    }
 
    /*
     * Key not found: create a new keyValuePair_t node
     */
    keyValuePair_t *new_pair = (keyValuePair_t *) malloc(sizeof(keyValuePair_t));
    if (new_pair == NULL) {
        perror("Failed to allocate memory for new key-value pair");
        return NULL;
    }
 
    new_pair->key = strdup(key);   // Duplicate key string
    new_pair->value = strdup(value); // Duplicate value string
    if (new_pair->key == NULL || new_pair->value == NULL) {
        perror("Failed to duplicate key or value string");
        free(new_pair->key);
        free(new_pair->value); // In case one failed after the other succeeded
        free(new_pair);
        return NULL;
    }
 
    new_pair->next = NULL; // New node is initially the last in its list
 
    // Add the new pair to the beginning of the linked list
    new_pair->next = ht->buckets[index];
    ht->buckets[index] = new_pair;
    return new_pair->value;
}
 
/**
 *
 * @brief Returns the value associated with the key or NULL if 
 * the key is not found
 *
 * @param[in] ht    hash table
 * @param[in] key   variable name
 *
 * @return pointer to the environment variable value or NULL
 */
 
char *
htFetchValue(hashTable_t *ht, const char *key)
{
    if (ht == NULL || key == NULL) return NULL;
 
    uint32_t index = hash(key);
    keyValuePair_t *current = ht->buckets[index];
 
    /*
     * Traverse the linked list at this bucket
     */
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return current->value;
        }
        current = current->next;
    }
    return NULL;
}
 
/**
 *
 * @brief Removes the key-value pair and frees its memory
 *
 * @param[in] ht    hash table
 * @param[in] key   variable name
 *
 */
 
void
htDeleteValue(hashTable_t *ht, const char *key)
{
    if (ht == NULL || key == NULL) return;
 
    uint32_t index = hash(key);
    keyValuePair_t *current = ht->buckets[index];
    keyValuePair_t *prev = NULL;
 
    /*
     * Traverse the linked list at this bucket
     */
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            // Key found: remove node from the list
            if (prev == NULL) {
                // Node to delete is the head of the list
                ht->buckets[index] = current->next;
            } else {
                // Node to delete is not the head
                prev->next = current->next;
            }
            free(current->key);   // Free memory for key string
            free(current->value); // Free memory for value string
            free(current);        // Free memory for the node itself
            return;
        }
        prev = current;
        current = current->next;
    }
}
/**
 *
 * @brief Free all memory allocated by the hash table
 *
 * @param[in] ht    hash table
 *
 */
 
void
htFreeTable(hashTable_t *ht)
{
    if (ht == NULL) return;
 
    for (int i = 0; i < TABLE_SIZE; i++) {
        keyValuePair_t *current = ht->buckets[i];
        while (current != NULL) {
            keyValuePair_t *temp = current;
            current = current->next;
            free(temp->key);
            free(temp->value);
            free(temp);
        }
        ht->buckets[i] = NULL; // Clear the bucket pointer
    }
}
