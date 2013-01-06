#ifndef SIMPLE_MAP_H
#define SIMPLE_MAP_H

/*
 * Yes, I know this implementation is not too good, but it will suffice for
 * the scope of the project where it is being used. We might not have more than
 * 255 entries in this list, so it is no big deal having to go through ALL the
 * elements in the list searching for an element to be replaced or removed
 */

typedef void (*simple_map_elem_free_fn)(void *v);

struct _simple_map_data {
    int key;
    void *v;

    struct _simple_map_data *next;
};


typedef struct _simple_map simple_map;


simple_map *simple_map_new(simple_map_elem_free_fn free_fn);
void simple_map_add(simple_map *map, int k, void *v);
int simple_map_key_exists(simple_map *map, int k);
void simple_map_rem(simple_map *map, int k);
void simple_map_free(simple_map *map);
void simple_map_dump(simple_map *map);
void *simple_map_get(simple_map *map, int k);

#endif //SIMPLE_MAP_H
