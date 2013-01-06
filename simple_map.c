#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "simple_map.h"


struct _simple_map {
    simple_map_elem_free_fn free_fn;
    struct _simple_map_data *data;
    
};

/**
 * Creates a new simple_map and uses free_fn as the function to destroy values
 * associated with a key. If free_fn is NULL then libc's free() function is
 * used
 */
simple_map *simple_map_new(simple_map_elem_free_fn free_fn) {

    simple_map *smap;

    smap = (simple_map *)malloc(sizeof(simple_map));
    memset(smap, 0, sizeof(simple_map));
    
    if(free_fn == NULL) {
        smap->free_fn = free;
    } else {
        smap->free_fn = free_fn;
    }

    smap->data = NULL;

    return smap;
}

/**
 * Associates the value in v with the key k. If k already exists in the
 * simple_map then the value v associated with it is freed with the function
 * provided in simple_map_new and the new value is associated.
 */
void simple_map_add(simple_map *map, int k, void *v) {

    struct _simple_map_data *data;
    struct _simple_map_data *tmp;
    
    assert(map != NULL);

    simple_map_rem(map, k);
    
    data = (struct _simple_map_data *)malloc(sizeof(struct _simple_map_data));
    data->key = k;
    data->v = v;
    data->next = NULL;

    if(map->data == NULL) {
        map->data = data;
    } else {
        tmp = map->data;
        while(tmp->next != NULL) {
            tmp = tmp->next;
        }

        tmp->next = data;
    }
}

/**
 * Returns 1 if the key exists, 0 otherwise
 */
int simple_map_key_exists(simple_map *map, int k) {
    struct _simple_map_data *data;

    assert(map != NULL);

    data = map->data;
    while(data != NULL) {
        if(data->key == k) {
            return 1;
        }
    }

    return 0;
}

/**
 * Removes the element associated with key k. The value associated is freed
 * using the function provided in simple_map_new
 * If the element does not exist, nothing happens.
 */
void simple_map_rem(simple_map *map, int k) {
    struct _simple_map_data *data;
    struct _simple_map_data *prev;

    assert(map != NULL);

    prev = data = map->data;
    while(data) {
        if(data->key == k) {
            map->free_fn(data->v);

            if(prev == data) {
                map->data = data->next;
            } else {
                prev->next = data->next;
            }

            free(data);
            break;
        }

        prev = data;
        data = data->next;

    }
}

void simple_map_free(simple_map *map) {
    struct _simple_map_data *data;

    assert(map != NULL);

    data = map->data;
    while(data) {
        map->free_fn(data->v);
        data = data->next;
    }

    free(data);
    free(map);
}

void *simple_map_get(simple_map *map, int k) {
    struct _simple_map_data *data;

    assert(map != NULL);

    data = map->data;
    while(data) {
        if(data->key == k) {
            return data->v;
        }
        
        data = data->next;
    }

    return NULL;
}

void simple_map_dump(simple_map *map) {
    struct _simple_map_data *data;

    assert(map != NULL);

    data = map->data;
    while(data) {
        //printf("Key: %d; data: %s\n", data->key, data->v);
        data = data->next;
    }

}
