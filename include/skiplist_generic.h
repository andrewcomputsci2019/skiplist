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
#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>

#ifndef SL_MAX_HEIGHT
#define SL_MAX_HEIGHT 32
#endif

#ifndef P
#define P 0.33
#endif

#ifndef P_Upper
#define P_Upper 0.70
#endif

#ifndef NO_OP
#define NO_OP(x) ((void)(x))
#endif


#define DEFINE_GENERIC_SKIPLIST(NAME, KEY_TYPE, CMP_FUNC, ON_REMOVE_FUNC_KEY) \
                                                                                     \
    typedef struct Node_##NAME##_t {                                                 \
        KEY_TYPE key;                                                                \
        uint32_t height;                                                             \
        struct Node_##NAME##_t * forward[];                                          \
    }Node_##NAME;                                                                    \
                                                                                     \
    typedef struct SkipList_##NAME##_t {                                              \
        uint32_t size;                                                               \
        uint32_t max_level;                                                          \
        Node_##NAME * header;                                                        \
    }SkipList_##NAME;                                                                 \
                                                                                     \
    static inline uint32_t clamp_level_##NAME(uint32_t lvl){                         \
        if(lvl == 0) return 1;                                                       \
        if(lvl > SL_MAX_HEIGHT) return SL_MAX_HEIGHT;                                \
        return lvl;                                                                  \
    }                                                                                \
                                                                                     \
    static inline int getDynamicPromotionProb_##NAME(uint32_t size, uint32_t level){ \
        double p = pow((double)(size ? size : 2), -1.0 / level);                     \
        if (p < P) p = P;                                                            \
        if (p > P_Upper) p = P_Upper;                                                \
        return (int)(p*100);                                                         \
    }                                                                                \
                                                                                     \
    static inline uint32_t getRandomLevel_##NAME(uint32_t size, uint32_t max_level){ \
        uint32_t lvl = 1;                                                            \
        int prob = getDynamicPromotionProb_##NAME(size, max_level);                  \
        while(((rand()%100) <= prob) && lvl < SL_MAX_HEIGHT){                        \
            lvl++;                                                                   \
        }                                                                            \
        return lvl;                                                                  \
    }                                                                                \
                                                                                     \
    static inline Node_##NAME * getNode_##NAME(uint32_t level, KEY_TYPE key) { \
        level = clamp_level_##NAME(level);                                                     \
        Node_##NAME * node = (Node_##NAME *)malloc(sizeof(Node_##NAME) + level * sizeof(Node_##NAME)); \
        assert(node);                                                                          \
        node->key = key;                                                                       \
        node->height = level;                                                                  \
        for(uint32_t i = 0; i < level; i++){                                                   \
            node->forward[i] = NULL;                                                           \
        }                                                                                      \
        return node;                                                                           \
    }                                                                                          \
                                                                                               \
    static inline bool SkipList_##NAME##_insert(SkipList_##NAME * sm, KEY_TYPE key){                                  \
        Node_##NAME * update[SL_MAX_HEIGHT];                                                                        \
        Node_##NAME * x = sm->header;                                                                               \
        for(int i = sm->max_level-1; i >= 0; i--){                                                                  \
            while(x->forward[i] && CMP_FUNC(x->forward[i]->key,key) < 0) x = x->forward[i];                         \
            update[i] = x;                                                                                          \
        }                                                                                                           \
        x = x->forward[0];                                                                                          \
        if(x && CMP_FUNC(x->key,key) == 0){                                                                         \
            return false;                                                                                           \
        }                                                                                                           \
        uint32_t height = getRandomLevel_##NAME(sm->size, sm->max_level);                                           \
        if(height > sm->max_level){                                                                                 \
            for(uint32_t i = sm->max_level; i < height; i++){                                                       \
                update[i] = sm->header;                                                                             \
            }                                                                                                       \
            sm->max_level = height;                                                                                 \
        }                                                                                                           \
        Node_##NAME * insertionNode = getNode_##NAME(height, key);                                            \
        for(uint32_t i = 0; i < height; i++){                                                                       \
            insertionNode->forward[i] = update[i]->forward[i];                                                      \
            update[i]->forward[i] = insertionNode;                                                                  \
        }                                                                                                           \
        sm->size++;                                                                                                 \
        return true;                                                                                                \
    }                                                                                                               \
                                                                                                                    \
    static inline bool SkipList_##NAME##_search(SkipList_##NAME * sm, KEY_TYPE key){                                  \
        Node_##NAME * x = sm->header;                                                                               \
        for(int i = sm->max_level - 1; i >= 0; i--){                                                                \
            while(x->forward[i] && CMP_FUNC(x->forward[i]->key, key) < 0)x = x->forward[i];                         \
            if(x->forward[i] && CMP_FUNC(x->forward[i]->key, key) == 0) return true;                                \
        }                                                                                                           \
        return false;                                                                                               \
    }                                                                                                               \
                                                                                                                    \
    static inline bool SkipList_##NAME##_remove(SkipList_##NAME * sm, KEY_TYPE key){                                  \
        Node_##NAME * update[SL_MAX_HEIGHT];                                                                        \
        Node_##NAME * x = sm->header;                                                                               \
        for(int i = sm->max_level-1; i>=0; i--){                                                                    \
            while(x->forward[i] && CMP_FUNC(x->forward[i]->key, key) < 0) x = x->forward[i];                        \
            update[i] = x;                                                                                          \
        }                                                                                                           \
        x = x->forward[0];                                                                                          \
        if(!x || CMP_FUNC(x->key, key) != 0) return false;                                                          \
        Node_##NAME * removalNode = x;                                                                              \
        for(uint32_t i = 0; i < removalNode->height; i++){                                                          \
            update[i]->forward[i] = removalNode->forward[i];                                                        \
        }                                                                                                           \
        ON_REMOVE_FUNC_KEY(removalNode->key);                                                                       \
        free(removalNode);                                                                                          \
        sm->size--;                                                                                                 \
        while(sm->max_level > 1 && !(sm->header->forward[sm->max_level-1]))sm->max_level--;                         \
        return true;                                                                                                \
    }                                                                                                               \
                                                                                                                    \
    static inline uint32_t SkipList_##NAME##_size(SkipList_##NAME * sm){                                              \
        return sm ? sm->size : 0;                                                                                   \
    }                                                                                                               \
                                                                                                                    \
    static inline bool SkipList_##NAME##_isEmpty(SkipList_##NAME * sm){                                               \
        return sm ? sm->size == 0 : true;                                                                           \
    }                                                                                                               \
                                                                                                                    \
    static inline void SkipList_##NAME##_destroy(SkipList_##NAME ** sm){                                              \
        if(!sm || !(*sm)) return;                                                                                   \
        Node_##NAME * x = (*sm)->header->forward[0];                                                                \
        while(x){                                                                                                   \
            Node_##NAME * next = x->forward[0];                                                                     \
            ON_REMOVE_FUNC_KEY(x->key);                                                                             \
            free(x);                                                                                                \
            x = next;                                                                                               \
        }                                                                                                           \
        free((*sm)->header);                                                                                        \
        free(*sm);                                                                                                  \
        *sm = NULL;                                                                                                 \
    }                                                                                                               \
                                                                                                                    \
    static inline SkipList_##NAME * SkipList_##NAME##_create(KEY_TYPE sentinel_k){                                    \
        SkipList_##NAME * sm = (SkipList_##NAME*)malloc(sizeof(SkipList_##NAME));                                      \
        assert(sm);                                                                                                 \
        sm->size = 0;                                                                                               \
        sm->max_level = 1;                                                                                          \
        sm->header = getNode_##NAME(SL_MAX_HEIGHT,sentinel_k);                                                      \
        return sm;                                                                                                  \
    }                                                                                                               \
    //end