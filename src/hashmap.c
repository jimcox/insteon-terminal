#include "hashmap.h"

#include <stdlib.h>

#define INITIAL_SIZE 256
#define MAX_CHAIN_LENGTH 8

int hashmap_init(hashmap *map, hashmap_hash_func hash_func,
                    hashmap_keycmp_func keycmp_func) {
    map->data = (hashmap_elem*) calloc(INITIAL_SIZE, sizeof(hashmap_elem));
    if (!map->data) return -1;
    
    map->table_size = INITIAL_SIZE;
    map->size = 0;
    map->hash_func = hash_func;
    map->keycmp_func = keycmp_func;

    return 0;
}

void hashmap_deinit(hashmap *map) {
    map->table_size = 0;
    map->size = 0;
    free(map->data);
}


int hashmap_put(hashmap *map, hashmap_key_ptr key, hashmap_value_ptr value) {
    int index = hashmap_get_pos(map, key);
    while (index < 0) { // While out of space
        if (hashmap_resize_rehash(map) < 0) return -1; // Out of Memory
        index = hashmap_get_pos(map, key);
    }

    // We have the index, set the data
    map->data[index].key = key;
    map->data[index].value = value;
    map->data[index].in_use = 1;
    map->size++;
    return 0;
}

int hashmap_get(hashmap *map, hashmap_key_ptr key, hashmap_value_ptr *result) {

    hashmap_keycmp_func keycmp_func = map->keycmp_func;

    int curr = hashmap_get_pos(map, key);
    for (int i = 0; i < MAX_CHAIN_LENGTH; i++) {
        if (map->data[curr].in_use == 1) {
            if (keycmp_func(map->data[curr].key, key) == 0) {
                *result = map->data[curr].value;
                return 0;
            }
        }
        curr = (curr + 1) % map->table_size;
    }
    return -1;
}

int hashmap_remove(hashmap *map, hashmap_key_ptr key) {

    hashmap_keycmp_func keycmp_func = map->keycmp_func;

    int curr = hashmap_get_pos(map, key);
    for (int i = 0; i < MAX_CHAIN_LENGTH; i++) {
        if (map->data[curr].in_use == 1) {
            if (keycmp_func(map->data[curr].key, key) == 0) {
                map->data[curr].in_use = 0;
                map->data[curr].value = 0;
                map->data[curr].key = 0;
                map->size--;
                return 0;
            }
        }
    }
    // Didn't find it
    return -1;
}

int hashmap_size(hashmap *map) {
    if (map) return map->size;
    else return 0;
}

int hashmap_get_pos(hashmap *map, hashmap_key_ptr key) {

    hashmap_hash_func hash_func = map->hash_func;
    hashmap_keycmp_func keycmp_func = map->keycmp_func;

    int curr = hash_func(key);
    for (int i = 0; i < MAX_CHAIN_LENGTH; i++) {
        if (map->data[curr].in_use == 0) return curr;

        if (map->data[curr].in_use == 1 &&
                keycmp_func(map->data[curr].key, key) == 0) return curr;

        curr = (curr + 1) % map->table_size;
    }
    // Table is full
    return -1;
}

int hashmap_resize_rehash(hashmap *map) {
    int old_table_size = map->table_size;
    hashmap_elem *old_data = map->data;
    // Double the size
    map->table_size = old_table_size * 2;
    map->data = (hashmap_elem*) calloc(map->table_size,
                                       sizeof(hashmap_elem));
    if (!map->data) return -1;
    for (int i = 0; i < old_table_size; i++) {
        if (old_data[i].in_use) {
            // Hash into new array
            if (!hashmap_put(map,
                        old_data[i].key, old_data[i].value)) {
                return -1;
            }
        }
    }
    
    // Free the old data
    free(old_data);
    return 0;
}

