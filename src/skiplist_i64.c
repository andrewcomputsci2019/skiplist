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
#include <skiplist_i64.h>
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

typedef struct Node_i64_t{
    int64_t key;
    char padding[4];
    uint32_t height;
    void * data;
    struct Node_i64_t * forward[];
}Node_i64;



struct SkipList_i64_t{
    uint32_t size;
    uint32_t max_level;
    Node_i64 * header;
};



/*_______________________________

    utitlity functions
__________________________________*/

static inline int getDynamicPromotionProb_i64(uint32_t size, uint32_t level){
    double p = pow((double)(size ? size : 2), -1.0 / level);
    if (p < P) p = P;
    if (p > P_Upper) p = P_Upper;
    return (int)(p*100);
}

uint32_t getRandomLevel_i64(uint32_t size, uint32_t max_level) {
    int level = 1;
    int prob = getDynamicPromotionProb_i64(size,max_level);
    while (((rand() % 101) <= prob) && level < SL_MAX_HEIGHT)
        level++;
    return level;
}

static inline uint32_t clamp_level_i64(uint32_t lvl) {
    if (lvl == 0) return 1;
    if (lvl > SL_MAX_HEIGHT) return SL_MAX_HEIGHT;
    return lvl;
}

static inline Node_i64 * getNode_i64(uint32_t level, int64_t key) {
    level = clamp_level_i64(level);
    Node_i64 * node = (Node_i64 *)malloc(sizeof(Node_i64) + level * sizeof(Node_i64 *));
    assert(node);
    node->key = key;
    node->height = level;
    node->data = NULL;
    for (uint32_t i = 0; i < level; i++){
        node->forward[i] = NULL;
    }
    return node;
}




/*______________________________

    skiplist i64 core functions 
________________________________*/





bool skipList_i64_insert_core(struct SkipList_i64_t * list, int64_t key, void * data){
    Node_i64 * x = list->header;
    Node_i64 * update[SL_MAX_HEIGHT];

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
    uint32_t height = getRandomLevel_i64(list->size,list->max_level);
    if(height > list->max_level){
        for(uint32_t i = list->max_level; i < height; i++){
            update[i] = list->header;
        }
        list->max_level = height;
    }
    Node_i64 * insertionNode = getNode_i64(height, key);
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


bool skipList_i64_search_core(struct SkipList_i64_t * list, int64_t key){
    Node_i64 * x = list->header;
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


void * skipList_i64_search_and_return_core(struct SkipList_i64_t * list, int64_t key){
    Node_i64 * x = list->header;
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


void skipList_i64_removal_core(struct SkipList_i64_t * list, int64_t key){
    Node_i64 * x = list->header;
    Node_i64 * update[SL_MAX_HEIGHT];
    
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

    Node_i64 * removalNode = x;
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


void * skipList_i64_removal_and_return_core(struct SkipList_i64_t * list, int64_t key){
    Node_i64 * x = list->header;
    Node_i64 * update[SL_MAX_HEIGHT];
    
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

    Node_i64 * removalNode = x;
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




/*__________________________

    skiplist i64 impl
______________________________*/

SkipList_i64 *skipList_i64_create(void)
{
    SkipList_i64 * sl = (SkipList_i64 * )malloc(sizeof(SkipList_i64));
    assert(sl);
    sl->max_level = 1;
    sl->size = 0;
    sl->header = getNode_i64(SL_MAX_HEIGHT,0);
    return sl;
}

bool skipList_i64_insert(SkipList_i64 *list, int64_t id)
{
    return skipList_i64_insert_core(list, id, NULL);
}

void skipList_i64_remove(SkipList_i64 *list, int64_t id)
{
    return skipList_i64_removal_core(list,id);
}

bool skipList_i64_search(SkipList_i64 *list, int64_t search_id)
{
    return skipList_i64_search_core(list,search_id);
}

uint32_t skipList_i64_getSize(const SkipList_i64 *list)
{
    return list ? list->size : 0;
}

bool skipList_i64_isEmpty(const SkipList_i64 *list)
{
    return list ? list->size == 0 : true;
}

void skipList_i64_destroy(SkipList_i64 **list)
{
    if (!list || !*list) return;
    SkipList_i64 * sl_list = *list;
    //iterate through lower level deleting nodes
    Node_i64 * x = sl_list->header->forward[0];
    while(x) {
        Node_i64 * next = x->forward[0];
        free(x);
        x = next;
    }
    free(sl_list->header);
    free(sl_list);
    *list = NULL; //prevent use after free
}

void skipList_i64_print(SkipList_i64 *list)
{
    for(int i = list->max_level-1; i >= 0; i--){
        Node_i64 * x = list->header->forward[i];
        printf("level %d: -->\t", i);
        while(x){
            printf("%ld-->", x->key);
            x = x->forward[i];
        }
        printf("nil\n");
    }
}



/*_______________________

    SkipMap i64 impl
_________________________*/


SkipMap_i64 *skipMap_i64_create(void)
{
    SkipMap_i64 * sm = (SkipMap_i64 *)malloc(sizeof(SkipMap_i64));
    assert(sm);
    sm->size = 0;
    sm->max_level = 1;
    sm->header = getNode_i64(SL_MAX_HEIGHT, 0);
    return sm;
}

bool skipMap_i64_put(SkipMap_i64 *sm, int64_t id, void *data)
{
    return skipList_i64_insert_core(sm, id, data);
}

void *skipMap_i64_get(SkipMap_i64 *sm, int64_t id)
{
    return skipList_i64_search_and_return_core(sm, id);
}

void *skipMap_i64_remove(SkipMap_i64 *sm, int64_t id)
{
    return skipList_i64_removal_and_return_core(sm,id);
}

bool skipMap_i64_contains(SkipMap_i64 *sm, int64_t id)
{
    return skipList_i64_search_core(sm,id);
}

void skipMap_i64_destroy(SkipMap_i64 **sm)
{
    if(!sm || !(*sm)) return;
    //the use of this function should be considered carefully
    //this will call free on all the data memebers of the nodes
    //if these data fields point to invalid addresses this will cause a segfault
    //not only that but if the skiplist doesnt own these data pointers, use after free could become possible
    //this should only be considered if the skiplist owns the pointers and the pointers are valid heap allocations
    Node_i64 * x = (*sm)->header->forward[0];
    while(x){
        Node_i64 * next = x->forward[0];
        free(x->data);
        x->data = NULL;
        free(x);
        x = next;
    }
    free((*sm)->header);
    free((*sm));
    *sm = NULL;
}

void skipMap_i64_print(SkipMap_i64 *sm)
{
    for(int i = sm->max_level-1; i >= 0; i--){
        Node_i64 * x = sm->header->forward[i];
        printf("level %d: -->\t", i);
        while(x){
            printf("(%ld, %lX)-->",x->key, x->data);
        }
        printf("nil\n");
    }      
}
