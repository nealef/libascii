#ifndef __ENVTABLE_H
# define __ENVTABLE_H

#define TABLE_SIZE 100

/**
 * Structure for a single Key-Value Pair (Node in the Linked List)
 */
typedef struct keyValuePair_t {
    char   *key;                  // Variable name
    char   *value;                // Variable value
    struct keyValuePair_t *next;  // Collision pointer
} keyValuePair_t;

/**
 * Structure for the Hash Table itself ---
 * It's an array of pointers to keyValuePair_t,
 * where each element is the head of of a linked list.
 */
typedef struct {
    keyValuePair_t *buckets[TABLE_SIZE]; // Array of linked list heads
} hashTable_t;

void htInitTable(hashTable_t *);
char *htAddValue(hashTable_t *, const char *, char *, int);
char *htFetchValue(hashTable_t *, const char *);
void htDeleteValue(hashTable_t *, const char *);
void htFreeTable(hashTable_t *);

#endif
