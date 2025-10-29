/*
 * SkipList Library
 * Copyright (C) 2025  Andrew Pegg
 *
 * The SkipList Library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, version 3 of the License only.
 *
 * The SkipList Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <https://www.gnu.org/licenses/lgpl-3.0.html>.
 */
#include <skiplist_u64.h>
/*malloc import*/
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>




/*
    Skiplist implemenation
    refernced Algorithms in C Thrid Edition, Robert Sedgewick
*/

// use a p factor of a thrid
// means a thrid of the time its expected we increase our depth
#ifndef p
    #define P 0.33
#endif
#ifndef P_Upper
    #define P_Upper 0.70
#endif


/*
    64 bit impl below
*/

// size 24 + variable (8 * x)
typedef struct Node_u64_t {
    uint64_t key;
    char pad[4];
    uint32_t height;
    void* data;
    struct Node_u64_t * forward[];
}Node_u64;

struct SkipList_u64_t {
    uint32_t size;
    uint32_t max_level;
    Node_u64 * header; 
};



/* _________________________________________________

    uint_64 core functions
_____________________________________________________*/

static inline int getDynamicPromotionProb_u64(uint32_t size, uint32_t level){
    double p = pow((double)(size ? size : 2), -1.0 / level);
    if (p < P) p = P;
    if (p > P_Upper) p = P_Upper;
    return (int)(p*100);
}

uint32_t getRandomLevel_u64(uint32_t size, uint32_t max_level) {
    int level = 1;
    int prob = getDynamicPromotionProb_u64(size,max_level);
    while (((rand() % 101) <= prob) && level < SL_MAX_HEIGHT)
        level++;
    return level;
}

static inline uint32_t clamp_level_u64(uint32_t lvl) {
    if (lvl == 0) return 1;
    if (lvl > SL_MAX_HEIGHT) return SL_MAX_HEIGHT;
    return lvl;
}

static inline Node_u64 * getNode_u64(uint32_t level, uint64_t key) {
    level = clamp_level_u64(level);
    Node_u64 * node = (Node_u64 *)malloc(sizeof(Node_u64) + level * sizeof(Node_u64 *));
    assert(node);
    node->key = key;
    node->height = level;
    node->data = NULL;
    for (uint32_t i = 0; i < level; i++){
        node->forward[i] = NULL;
    }
    return node;
}



bool skipList_u64_insert_core(struct SkipList_u64_t * list, uint64_t key, void * data){
    Node_u64 * update[SL_MAX_HEIGHT];
    Node_u64 * x = list->header;
    for(int i = list->max_level - 1; i >= 0; i--){
        while(x->forward[i] && x->forward[i]->key < key){
            x = x->forward[i];
        }
        update[i] = x;
    }
    x = x->forward[0];
    if(x && x->key == key){
        if(data){
            x->data = data;
            return true;
        }
        return false;
    }
    uint32_t height = getRandomLevel_u64(list->size, list->max_level);
    if(height > list->max_level){
        for(uint32_t i = list->max_level; i < height; i++){
            update[i] = list->header;
        }
        list->max_level = height;
    }
    Node_u64 * insertionNode = getNode_u64(height, key);
    if(data){
        insertionNode->data = data;
    }
    for(uint32_t i = 0; i < height; i++){
        insertionNode->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = insertionNode;
    }
    list->size++;
    return true;
}

bool skipList_u64_search_core(struct SkipList_u64_t * list, uint64_t key){
    Node_u64 * x = list->header;
    for(int i = list->max_level - 1; i >= 0; i--){
        while(x->forward[i] && x->forward[i]->key < key){
            x = x->forward[i];
        }
        if(x->forward[i] && x->forward[i]->key == key){
            return true;
        }
    }
    return false;
}

void * skipList_u64_search_and_return_core(struct SkipList_u64_t * list, uint64_t key){
    Node_u64 * x = list->header;
    for(int i = list->max_level - 1; i >= 0; i--){
        while(x->forward[i] && x->forward[i]->key < key){
            x = x->forward[i];
        }
        if(x->forward[i] && x->forward[i]->key == key){
            return x->forward[i]->data;
        }
    }
    return NULL;
}


void skipList_u64_remove_core(struct SkipList_u64_t * list, uint64_t key){
    Node_u64 * update[SL_MAX_HEIGHT];
    Node_u64 * x = list->header;

    for(int i = list->max_level - 1; i >= 0; i--){
        while(x->forward[i] && x->forward[i]->key < key){
            x = x->forward[i];
        }
        update[i] = x;
    }

    // check for existence
    x = x->forward[0];
    if(!x || x->key != key){
        return;
    }

    Node_u64 * removalNode = x;
    for(uint32_t i = 0; i < removalNode->height; i++){
        update[i]->forward[i] = removalNode->forward[i];
    }
    free(removalNode);
    //coalesce height
    while(list->max_level > 1 && !(list->header->forward[list->max_level-1])){
        list->max_level -= 1;
    }
    list->size--;
}

void * skipList_u64_remove_and_return_core(struct SkipList_u64_t * list, uint64_t key){
    Node_u64 * update[SL_MAX_HEIGHT];
    Node_u64 * x = list->header;

    for(int i = list->max_level - 1; i >= 0; i--){
        while(x->forward[i] && x->forward[i]->key < key){
            x = x->forward[i];
        }
        update[i] = x;
    }

    // check for existence
    x = x->forward[0];
    if(!x || x->key != key){
        return NULL;
    }

    Node_u64 * removalNode = x;
    for(uint32_t i = 0; i < removalNode->height; i++){
        update[i]->forward[i] = removalNode->forward[i];
    }
    void * data = removalNode->data;
    free(removalNode);
    //coalesce height
    while(list->max_level > 1 && !(list->header->forward[list->max_level-1])){
        list->max_level -= 1;
    }
    list->size--;
    return data;
}





/*___________________________________________

    uint64 SkipList impl
______________________________________________*/

SkipList_u64 *skipList_u64_create(void)
{
    SkipList_u64 * sl = (SkipList_u64 *)malloc(sizeof(SkipList_u64));
    assert(sl);
    sl->max_level = 1;
    sl->size = 0;
    sl->header = getNode_u64(SL_MAX_HEIGHT, 0);
    return sl;
}

bool skipList_u64_insert(SkipList_u64 *list, uint64_t id)
{
    return skipList_u64_insert_core(list, id, NULL);
}

void skipList_u64_remove(SkipList_u64 *list, uint64_t id)
{
    skipList_u64_remove_core(list, id);
}

bool skipList_u64_search(SkipList_u64 *list, uint64_t search_id)
{
    return skipList_u64_search_core(list, search_id);
}

uint32_t skipList_u64_getSize(const SkipList_u64 *list)
{
    return list ? list->size : 0;
}

bool skipList_u64_isEmpty(const SkipList_u64 *list)
{
    return list ? list->size == 0: false;
}

// todo
void skipList_u64_destroy(SkipList_u64 **list)
{
    if (!list || !*list) return;
    // iterate through skiplist deleting nodes
    SkipList_u64 * sl_list = *list;
    Node_u64 * x = sl_list->header->forward[0];
    while(x) {
        Node_u64 * next = x->forward[0];
        free(x);
        x = next;
    }
    free(sl_list->header);
    free(sl_list);
    *list = NULL; //prevent use after free
}

void skipList_u64_print(SkipList_u64 *list)
{
    for(int i = list->max_level -1; i >= 0; i--){
        printf("Level: %d\t-->", i);
        Node_u64 * x = list->header->forward[i];
        while(x){
            printf("%lu-->",x->key);
            x = x->forward[i];
        }
        printf("\n");
    }
    return;
}


/*_______________________________________

    uint64 SkipMap impl
__________________________________________*/

SkipMap_u64 *skipMap_u64_create(void)
{
    SkipMap_u64 * sm = (SkipMap_u64 * )malloc(sizeof(SkipMap_u64));
    assert(sm);
    sm->max_level = 1;
    sm->size = 0;
    sm->header = getNode_u64(SL_MAX_HEIGHT,0);
    return sm;
}

bool skipMap_u64_put(SkipMap_u64 *sm, uint64_t id, void *data)
{
    return skipList_u64_insert_core(sm, id, data);
}

void *skipMap_u64_get(SkipMap_u64 *sm, uint64_t id)
{
    return skipList_u64_search_and_return_core(sm,id);
}

void *skipMap_u64_remove(SkipMap_u64 *sm, uint64_t id)
{
    return skipList_u64_remove_and_return_core(sm,id);
}

bool skipMap_u64_contains(SkipMap_u64 *sm, uint64_t id)
{
    return skipList_u64_search(sm, id);
}

uint32_t skipMap_u64_size(SkipMap_u64 *sm)
{
    return sm ? sm->size : 0;
}

bool skipMap_u64_isEmpty(SkipMap_u64 *sm)
{
    return sm ? sm->size == 0 : true;
}

void skipMap_u64_destroy(SkipMap_u64 **sm)
{
    if(!sm || !(*sm))return;
    //the use of this function should be considered carefully
    //this will call free on all the data memebers of the nodes
    //if these data fields point to invalid addresses this will cause a segfault
    //not only that but if the skiplist doesnt own these data pointers, use after free could become possible
    //this should only be considered if the skiplist owns the pointers and the pointers are valid heap allocations
    Node_u64 * x = (*sm)->header->forward[0];
    while(x){
        Node_u64 * next = x->forward[0];
        free(x->data);
        x->data = NULL;
        free(x);
        x = next;
    }
    free((*sm)->header);
    free(*sm);
    *sm = NULL;
}

void skipMap_u64_print(SkipMap_u64 *sm)
{
    for(int i = sm->max_level-1; i >= 0; i--){
        Node_u64 * x = sm->header->forward[i];
        printf("level %d: -->\t", i);
        while(x){
            printf("(%lu, %lX)-->",x->key, x->data);
        }
        printf("nil\n");
    }
}
