#ifndef __INSTEON_MAP_H__
#define __INSTEON_MAP_H__

typedef void* hashmap_value_ptr;
typedef void* hashmap_key_ptr;

typedef unsigned long (*hashmap_hash_func)(hashmap_key_ptr);

typedef int (*hashmap_keycmp_func)(hashmap_key_ptr, hashmap_key_ptr);

typedef struct {
    int in_use; // A bool
    hashmap_key_ptr key;
    hashmap_value_ptr value;
} hashmap_elem;

typedef struct {
	int table_size;
	int size;
    hashmap_hash_func hash_func;
    hashmap_keycmp_func keycmp_func;

	hashmap_elem *data;
} hashmap;

/*
 * Will setup the hashmap table/function pointers
 */
int hashmap_init(hashmap *map,
        hashmap_hash_func hash_func,
        hashmap_keycmp_func keycmp_func);

// Special types of maps
// Map from string to value
int hashmap_init_strmap(hashmap *map);
// Map from int to value
int hashmap_init_intmap(hashmap *map);

/*
 * Free the hashmap's associated data
 */
void hashmap_deinit(hashmap *map);


/*
 * Add an element allocated on the heap to the hashmap.
 * Returns 0 if success, -1 if out of memory
 */
int hashmap_put(hashmap *map, hashmap_key_ptr key, hashmap_value_ptr value);

/*
 * Get an element from the hashmap. Returns 0 if found, -1 if missing
 * Puts result in *result
 */
int hashmap_get(hashmap *map, hashmap_key_ptr key, hashmap_value_ptr *result);

/*
 * Removes an element. Returns 0 if an element was removed, -1 if
 * no element matched the string
 */
int hashmap_remove(hashmap *map, hashmap_key_ptr key);

/*
 * Get the current size of a hashmap
 */
int hashmap_size(hashmap *map);

/*
 * Will compute the hash position
 * for a given element, or return -1
 * if the map is full
 */
int hashmap_get_pos(hashmap *map, hashmap_key_ptr key);

/*
 * Will increase size of map by factor of 2 and rehash 
 */
int hashmap_resize_rehash(hashmap *map);

#endif
