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
#include <skiplist_i32.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>


#ifndef p
    #define P 0.33
#endif
#ifndef P_Upper
    #define P_Upper 0.70
#endif

typedef struct Node_i32_t{
    int32_t key;
    uint32_t height;
    void* data;
    struct Node_i32_t * forward[];
}Node_i32;


struct SkipList_i32_t{
    uint32_t max_level;
    uint32_t size;
    Node_i32 * header;
};


/*_______________________________________

    utility functions
_________________________________________*/

static inline int getDynamicPromotionProb_i32(uint32_t size, uint32_t level){
    double p = pow((double)(size ? size : 2), -1.0 / level);
    if (p < P) p = P;
    if (p > P_Upper) p = P_Upper;
    return (int)(p*100);
}

uint32_t getRandomLevel_i32(uint32_t size, uint32_t max_level) {
    int level = 1;
    int prob = getDynamicPromotionProb_i32(size,max_level);
    while (((rand() % 101) <= prob) && level < SL_MAX_HEIGHT)
        level++;
    return level;
}

static inline uint32_t clamp_level_i32(uint32_t lvl) {
    if (lvl == 0) return 1;
    if (lvl > SL_MAX_HEIGHT) return SL_MAX_HEIGHT;
    return lvl;
}

static inline Node_i32 * getNode_i32(uint32_t level, int32_t key) {
    level = clamp_level_i32(level);
    Node_i32 * node = (Node_i32 *)malloc(sizeof(Node_i32) + level * sizeof(Node_i32 *));
    assert(node);
    node->key = key;
    node->height = level;
    node->data = NULL;
    for (uint32_t i = 0; i < level; i++){
        node->forward[i] = NULL;
    }
    return node;
}


/* _________________________________________________

    int_32 core functions
_____________________________________________________*/



bool skipList_i32_insert_core(struct SkipList_i32_t * list, int32_t key, void * data){
    Node_i32 * x = list->header;
    Node_i32 * update[SL_MAX_HEIGHT];

    for(int i = list->max_level-1; i >= 0; i--){
        while(x->forward[i] && x->forward[i]->key < key){
            x = x->forward[i];
        }
        update[i] = x;
    }
    
    // check for existence
    x = x->forward[0];
    if(x && x->key == key){
        if(data){
            x->data = data;
            return true;
        }
        return false;
    }
    // get randonly selected height
    uint32_t height = getRandomLevel_i32(list->size,list->max_level);
    if(height > list->max_level){
        for(uint32_t i = list->max_level; i < height; i++){
            update[i] = list->header;
        }
        list->max_level = height;
    }
    Node_i32 * insertionNode = getNode_i32(height, key);
    if(data){
        insertionNode->data = data;
    }
    //update pointers
    for(uint32_t i = 0; i < height; i++){
        insertionNode->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = insertionNode;
    }
    list->size++;
    return true;
}


bool skipList_i32_search_core(struct SkipList_i32_t * list, int32_t key){
    Node_i32 * x = list->header;
    for(int i = list->max_level - 1; i >= 0; i--){
        while(x->forward[i] && x->key < key){
            x = x->forward[i];
        }
        if(x->forward[i] && x->forward[i]->key == key){
            return true;
        }
    }
    return false;
}


void * skipList_i32_search_and_return_core(struct SkipList_i32_t * list, int32_t key){
    Node_i32 * x = list->header;
    for(int i = list->max_level - 1; i >= 0; i--){
        while(x->forward[i] && x->key < key){
            x = x->forward[i];
        }
        if(x->forward[i] && x->forward[i]->key == key){
            return x->forward[i]->data;
        }
    }
    return NULL;
}


void skipList_i32_removal_core(struct SkipList_i32_t * list, int32_t key){
    Node_i32 * x = list->header;
    Node_i32 * update[SL_MAX_HEIGHT];
    
    for(int i = list->max_level - 1; i >= 0; i--){
        while(x->forward[i] && x->forward[i]->key < key){
            x = x->forward[i];
        }
        update[i] = x;
    }

    // check to see if key even exist
    x = x->forward[0];
    if(!x || x->key != key){
        return;
    }

    Node_i32 * removalNode = x;
    for(int i = 0; i < removalNode->height; i++){
        update[i]->forward[i] = removalNode->forward[i];
    }
    //release allocation
    free(removalNode);

    //coalesce height
    while(list->max_level > 1 && !(list->header->forward[list->max_level-1])){
        list->max_level -= 1;
    }
    list->size--;
}


void * skipList_i32_removal_and_return_core(struct SkipList_i32_t * list, int32_t key){
    Node_i32 * x = list->header;
    Node_i32 * update[SL_MAX_HEIGHT];
    
    for(int i = list->max_level - 1; i >= 0; i--){
        while(x->forward[i] && x->forward[i]->key < key){
            x = x->forward[i];
        }
        update[i] = x;
    }

    // check to see if key even exist
    x = x->forward[0];
    if(!x || x->key != key){
        return NULL;
    }

    Node_i32 * removalNode = x;
    for(int i = 0; i < removalNode->height; i++){
        update[i]->forward[i] = removalNode->forward[i];
    }
    void * data = removalNode->data;
    //release allocation
    free(removalNode);

    //coalesce height
    while(list->max_level > 1 && !(list->header->forward[list->max_level-1])){
        list->max_level -= 1;
    }
    list->size--;
    return data;
}



/*_____________________________________

    skipList i32 impl
_______________________________________*/

SkipList_i32 *skipList_i32_create(void)
{
    SkipList_i32 * sl = (SkipList_i32 *)malloc(sizeof(SkipList_i32));
    assert(sl);
    sl->max_level = 1;
    sl->size = 0;
    sl->header = getNode_i32(SL_MAX_HEIGHT, 0);
    return sl;
}

bool skipList_i32_insert(SkipList_i32 *list, int32_t id)
{
    return skipList_i32_insert_core(list, id, NULL);
}

void skipList_i32_remove(SkipList_i32 *list, int32_t id)
{
    return skipList_i32_removal_core(list, id);
}

bool skipList_i32_search(SkipList_i32 *list, int32_t search_id)
{
    return skipList_i32_search_core(list, search_id);
}

uint32_t skipList_i32_getSize(const SkipList_i32 *list)
{
    return list ? list->size : 0;
}

bool skipList_i32_isEmpty(const SkipList_i32 *list)
{
    return list ? list->size == 0 : true;
}

bool skipList_i32_pop(SkipList_i32 *list, int32_t *removedID) {
    if(skipList_i32_isEmpty(list)||!removedID) {
        return false;
    }
    Node_i32 * x = list->header->forward[0];
    *removedID = x->key;
    for (uint32_t i = 0; i < x->height; i++) {
        list->header->forward[i] = x->forward[i];
    }
    free(x);
    list->size--;
    return true;
}

void skipList_i32_destroy(SkipList_i32 **list)
{
    if (!list || !*list) return;
    SkipList_i32 * sl_list = *list;
    //iterate through lower level deleting nodes
    Node_i32 * x = sl_list->header->forward[0];
    while(x) {
        Node_i32 * next = x->forward[0];
        free(x);
        x = next;
    }
    free(sl_list->header);
    free(sl_list);
    *list = NULL; //prevent use after free
}

void skipList_i32_print(SkipList_i32 *list)
{
    for(int i = list->max_level-1; i >= 0; i--){
        Node_i32 * x = list->header->forward[i];
        printf("level %d: -->\t", i);
        while(x){
            printf("%d-->", x->key);
            x = x->forward[i];
        }
        printf("nil\n");
    }
}


/*__________________________

    SkipMap i32 impl
___________________________*/

SkipMap_i32 *skipMap_i32_create(void)
{
    SkipMap_i32 * sm = (SkipMap_i32 *)malloc(sizeof(SkipMap_i32));
    assert(sm);
    sm->max_level = 1;
    sm->size = 0;
    sm->header = getNode_i32(SL_MAX_HEIGHT, 0);
    return sm;
}

bool skipMap_i32_put(SkipMap_i32 *sm, int32_t id, void *data)
{
    return skipList_i32_insert_core(sm,id,data);
}

void *skipMap_i32_get(SkipMap_i32 *sm, int32_t id)
{
    return skipList_i32_search_and_return_core(sm,id);
}

void *skipMap_i32_remove(SkipMap_i32 *sm, int32_t id)
{
    return skipList_i32_removal_and_return_core(sm,id);
}

bool skipMap_i32_contains(SkipMap_i32 *sm, int32_t id)
{
    return skipList_i32_search_core(sm,id);
}

uint32_t skipMap_i32_getSize(const SkipMap_i32 *sm) {
    return sm ? sm->size : 0;
}

bool skipMap_i32_isEmpty(const SkipMap_i32 *sm) {
    return sm ? sm->size == 0 : true;
}

bool skipMap_i32_pop(SkipList_i32 *list, struct SM_i32_kv *sm) {
    if(skipMap_i32_isEmpty(list) || !sm) {
        return false;
    }
    sm->key = list->header->forward[0]->key;
    sm->value = list->header->forward[0]->data;
    //need to relink pointers removing this node
    Node_i32 * x = list->header->forward[0];
    for (uint32_t i = 0; i < x->height; i++) {
        list->header->forward[i] = x->forward[i];
    }
    free(x);
    list->size--;
    return true;
}

void skipMap_i32_destroy(SkipMap_i32 **sm)
{
    if(!sm || !(*sm)) return;
    //the use of this function should be considered carefully
    //this will call free on all the data memebers of the nodes
    //if these data fields point to invalid addresses this will cause a segfault
    //not only that but if the skiplist doesnt own these data pointers, use after free could become possible
    //this should only be considered if the skiplist owns the pointers and the pointers are valid heap allocations
    Node_i32 * x = (*sm)->header->forward[0];
    while(x){
        Node_i32 * next = x->forward[0];
        free(x->data);
        x->data = NULL;
        free(x);
        x = next;
    }
    free((*sm)->header);
    free((*sm));
    *sm = NULL;
}

void skipMap_i32_print(SkipMap_i32 *sm)
{
    for(int i = sm->max_level-1; i >= 0; i--){
        Node_i32 * x = sm->header->forward[i];
        printf("level %d: -->\t", i);
        while(x){
            printf("(%d, %lX)-->",x->key, (unsigned long)x->data);
        }
        printf("nil\n");
    }      
}
