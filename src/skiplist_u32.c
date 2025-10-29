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
#include <skiplist_u32.h>
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
    ##### skiplist_u32 impl ######
*/

typedef struct Node_u32_t{
    uint32_t key;
    uint32_t height;
    void * data;
    struct Node_u32_t *forward[]; // [0] next node at the zeroth level, [1] next node at the 1 level, ..., [curr_level] next node in linked list at this level
}Node_u32;

struct SkipList_u32_t
{
    uint32_t max_level; // log2(size) in general
    uint32_t size;
    // always the top level node
    Node_u32 * header;
};



static inline int getDynamicPromotionProb_u32(uint32_t size, uint32_t level){
    double p = pow((double)(size ? size : 2), -1.0 / level);
    if (p < P) p = P;
    if (p > P_Upper) p = P_Upper;
    return (int)(p*100);
}

uint32_t getRandomLevel_u32(uint32_t size, uint32_t max_level) {
    int level = 1;
    int prob = getDynamicPromotionProb_u32(size,max_level);
    while (((rand() % 101) <= prob) && level < SL_MAX_HEIGHT)
        level++;
    return level;
}

static inline uint32_t clamp_level_u32(uint32_t lvl) {
    if (lvl == 0) return 1;
    if (lvl > SL_MAX_HEIGHT) return SL_MAX_HEIGHT;
    return lvl;
}

static inline Node_u32 * getNode_u32(uint32_t level, uint32_t key) {
    level = clamp_level_u32(level);
    Node_u32 * node = (Node_u32 *)malloc(sizeof(Node_u32) + level * sizeof(Node_u32 *));
    assert(node);
    node->key = key;
    node->height = level;
    node->data = NULL;
    for (uint32_t i = 0; i < level; i++){
        node->forward[i] = NULL;
    }
    return node;
}


SkipList_u32 *skipList_u32_create()
{
    srand(time(NULL));
    SkipList_u32 * list = (SkipList_u32*)malloc(sizeof(SkipList_u32));
    assert(list);
    // header 
    list->max_level = 1;
    list->size = 0;
    list->header = getNode_u32(SL_MAX_HEIGHT,0);
    return list;
}



bool skipList_u32_insert_core(struct SkipList_u32_t * list, uint32_t id, void * data){
    Node_u32 * update[SL_MAX_HEIGHT];
    Node_u32 * x = list->header;

    // get all predcessor nodes
    for(int level = list->max_level - 1; level >= 0; level--){
        while(x->forward[level] && x->forward[level]->key < id){
            x = x->forward[level];
        }
        update[level] = x;
    }
    // check for existence
    x = x->forward[0];
    if(x && x->key == id){
        if(data){
            x->data = data;
            return true;
        }
        return false;
    }

    uint32_t height = getRandomLevel_u32(list->size, list->max_level);
    if(height > list->max_level){
        for(uint32_t i = list->max_level; i < height; i++) {
            update[i] = list->header;
        }
        list->max_level = height;
    }
    Node_u32 * insertionNode = getNode_u32(height,id);
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


bool skipList_u32_search_core(struct SkipList_u32_t * list, uint32_t id){
    Node_u32 * x = list->header;
    for(int i = list->max_level-1; i >=0; i--){
        while(x->forward[i] && x->forward[i]->key < id){
            x = x->forward[i];
        }
        if(x->forward[i] && x->forward[i]->key == id){
            return true;
        }
    }
    return false;
}

void * skipList_u32_search_and_return_core(struct SkipList_u32_t * list, uint32_t id){
    Node_u32 * x = list->header;
    for(int i = list->max_level-1; i >=0; i--){
        while(x->forward[i] && x->forward[i]->key < id){
            x = x->forward[i];
        }
        if(x->forward[i] && x->forward[i]->key == id){
            return x->forward[i]->data;
        }
    }
    return NULL;
}

void skipList_u32_removal_core(struct SkipList_u32_t * list, uint32_t id ) {
    Node_u32 * x = list->header;
    Node_u32 * update[SL_MAX_HEIGHT];
    for(int i = list->max_level-1; i >= 0; i--){
        while(x->forward[i] && x->forward[i]->key < id){
            x = x->forward[i];
        }
        update[i] = x;
    }
    x = x->forward[0];
    if(!x || x->key != id){
        return;
    }
    Node_u32 * removalNode = x;
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

void * skipList_u32_removal_and_return_core(struct SkipList_u32_t * list, uint32_t id){
    Node_u32 * x = list->header;
    Node_u32 * update[SL_MAX_HEIGHT];
    for(int i = list->max_level-1; i >= 0; i--){
        while(x->forward[i] && x->forward[i]->key < id){
            x = x->forward[i];
        }
        update[i] = x;
    }
    x = x->forward[0];
    if(!x || x->key != id){
        return NULL;
    }
    Node_u32 * removalNode = x;
    for(uint32_t i = 0; i < removalNode->height; i++){
        update[i]->forward[i] = removalNode->forward[i];
    }
    void * data = removalNode->data;
    free(removalNode);
    while(list->max_level > 1 && !(list->header->forward[list->max_level-1])){
        list->max_level -= 1;
    }
    list->size--;
    return data;
}


bool skipList_u32_insert(SkipList_u32 *list, uint32_t id)
{
    // Node_u32 *update[SL_MAX_HEIGHT]; //small stack allocation (pretty much analog to clasical recurssion implemenation but we save into this stack instead of making the change at each depth)
    // Node_u32 *x = list->header;

    // // get all predcessor nodes
    // for(int level = list->max_level - 1; level >= 0; level--){
    //     while(x->forward[level] != NULL && x->forward[level]->key < id){
    //         x = x->forward[level];
    //     }
    //     update[level] = x; // this is the node that comes before the insertion node at this level
    // }

    // x = x->forward[0];
    // // if key already exist then next item at depth zero is this id
    // if(x && x->key == id){
    //     return false; // already apart of set
    // }

    // uint32_t height = getRandomLevel_u32(list->size,list->max_level);
    // // need to update nodes if this level is higher, in this case the predcessor node is the header
    // if (height > list->max_level) {
    //     for(uint32_t i = list->max_level; i < height; i++){
    //         update[i] = list->header;
    //     }
    //     list->max_level = height;
    // }
    // Node_u32 * insertion_node = getNode_u32(height,id);
    // // go through stack and update pointers/links
    // for(uint32_t h = 0; h < height; h++){
    //     insertion_node->forward[h] = update[h]->forward[h];
    //     update[h]->forward[h] = insertion_node;
    // }
    // list->size++;
    // return true;

    return skipList_u32_insert_core(list,id,NULL);

}

void skipList_u32_remove(SkipList_u32 *list, uint32_t id)
{
    // Node_u32* update[SL_MAX_HEIGHT]; // we need to update all prev pointers for starting from the first height where node with id resides
    // Node_u32* x = list->header;
    // for(int level = list->max_level - 1; level >= 0; level--){
    //     while(x->forward[level] && x->forward[level]->key < id){
    //         x = x->forward[level];
    //     }
    //     update[level] = x;
    // }
    // // node wasnt found
    // if(!x->forward[0] || x->forward[0]->key != id){
    //     return;
    // }
    
    // // get pointer to node that is being removed
    // Node_u32 * removal_node = x->forward[0];
    // // fix pointers
    // for(uint32_t i = 0; i < removal_node->height; i++){
    //     update[i]->forward[i] = removal_node->forward[i];
    // }
    // free(removal_node);

    // // if the top level is now empty adjust max level
    // while (list->max_level > 1 && list->header->forward[list->max_level - 1] == NULL){
    //     list->max_level--;
    // }
    // list->size -= 1;

    skipList_u32_removal_core(list,id);
}

bool skipList_u32_search(SkipList_u32 *list, uint32_t search_id)
{
    // Node_u32 * x = list->header;
    // for(int level = list->max_level-1; level >= 0; level--){
    //     while(x->forward[level] != NULL && x->forward[level]->key < search_id){
    //         x = x->forward[level];
    //     }
    //     if(x->forward[level] && x->forward[level]->key == search_id){
    //         return true;
    //     }
    // }
    // return false;

    skipList_u32_search_core(list,search_id);
}

uint32_t skipList_u32_getSize(const SkipList_u32 *list)
{
    return list ? list->size : 0;
}

bool skipList_u32_isEmpty(const SkipList_u32 *list)
{
    return list ? list->size == 0 : true;
}

void skipList_u32_destroy(SkipList_u32 **list)
{
    if (!list || !*list) return;
    // iterate through skiplist deleting nodes
    SkipList_u32 * sl_list = *list;
    Node_u32 * x = sl_list->header->forward[0];
    while(x) {
        Node_u32 * next = x->forward[0];
        free(x);
        x = next;
    }
    free(sl_list->header);
    free(sl_list);
    *list = NULL; //prevent use after free
}

// Basically just prints linked list layer by layer
void skipList_u32_print(SkipList_u32 * list){
    for(int i = list->max_level -1; i >= 0; i--){
        printf("Level: %d\t-->", i);
        Node_u32 * x = list->header->forward[i];
        while(x){
            printf("%u-->",x->key);
            x = x->forward[i];
        }
        printf("nil\n");
    }
    return;
}



/*
    ##### END skiplist_u32 impl #######
*/



/*
    #### skipMap_u32 impl ####
*/


SkipMap_u32 * skipMap_u32_create(){
    srand(time(NULL));
    SkipMap_u32 * sm = (SkipMap_u32 *)malloc(sizeof(SkipMap_u32));
    assert(sm);
    sm->max_level = 1;
    sm->size = 0;
    sm->header = getNode_u32(SL_MAX_HEIGHT,0);
    return sm;
}

bool skipMap_u32_put(SkipMap_u32 *sm, uint32_t id, void *data)
{
    return skipList_u32_insert_core(sm, id, data);
}

void *skipMap_u32_get(SkipMap_u32 *sm, uint32_t id)
{
    return skipList_u32_search_and_return_core(sm,id);
}

void *skipMap_u32_remove(SkipMap_u32 *sm, uint32_t id)
{
    return skipList_u32_removal_and_return_core(sm,id);
}

bool skipMap_u32_contains(SkipMap_u32 *sm, uint32_t id)
{
    return skipList_u32_search_core(sm, id);
}

// todo 
void skipMap_u32_destroy(SkipMap_u32 **sm)
{
    if(!sm || !(*sm)) return;
    //the use of this function should be considered carefully
    //this will call free on all the data memebers of the nodes
    //if these data fields point to invalid addresses this will cause a segfault
    //not only that but if the skiplist doesnt own these data pointers, use after free could become possible
    //this should only be considered if the skiplist owns the pointers and the pointers are valid heap allocations
    Node_u32 * x = (*sm)->header->forward[0];
    while(x){
        Node_u32 * next = x->forward[0];
        free(x->data);
        x->data = NULL;
        free(x);
        x = next;
    }
    free((*sm)->header);
    free(*sm);
    *sm = NULL;
}

void skipMap_u32_print(SkipMap_u32 *sm)
{
    for(int i = sm->max_level-1; i >= 0; i--){
        Node_u32 * x = sm->header->forward[i];
        printf("level %d: -->\t", i);
        while(x){
            printf("(%u, %lX)-->",x->key, x->data);
        }
        printf("nil\n");
    }
}

/*
    #### END  skipMap_u32 impl ####
*/


