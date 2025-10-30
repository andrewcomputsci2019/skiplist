
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

#include <skiplist_u32.h>
#include <skiplist_u64.h>
#include <skiplist_i32.h>
#include <skiplist_i64.h>


#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
    #define HAS_C11 1
#else
    #define HAS_C11 0
    #warning "Generics wont be included in buildouput"
#endif


#ifdef HAS_C11

/*──────────────────────────────────────────────
    Generic creation
  ──────────────────────────────────────────────*/
#define skipList_create(T) \
    _Generic((T), \
        int32_t:  skipList_i32_create, \
        int64_t:  skipList_i64_create, \
        uint32_t: skipList_u32_create, \
        uint64_t: skipList_u64_create \
    )()

#define skipMap_create(T) \
    _Generic((T), \
        int32_t:  skipMap_i32_create, \
        int64_t:  skipMap_i64_create, \
        uint32_t: skipMap_u32_create, \
        uint64_t: skipMap_u64_create \
    )()

/*──────────────────────────────────────────────
    SkipList (set) unified ops
  ──────────────────────────────────────────────*/
#define skipList_insert(list, id) \
    _Generic((id), \
        int32_t:  skipList_i32_insert, \
        int64_t:  skipList_i64_insert, \
        uint32_t: skipList_u32_insert, \
        uint64_t: skipList_u64_insert \
    )(list, id)

#define skipList_remove(list, id) \
    _Generic((id), \
        int32_t:  skipList_i32_remove, \
        int64_t:  skipList_i64_remove, \
        uint32_t: skipList_u32_remove, \
        uint64_t: skipList_u64_remove \
    )(list, id)

#define skipList_search(list, id) \
    _Generic((id), \
        int32_t:  skipList_i32_search, \
        int64_t:  skipList_i64_search, \
        uint32_t: skipList_u32_search, \
        uint64_t: skipList_u64_search \
    )(list, id)

#define skipList_getSize(list) \
    _Generic((list), \
        SkipList_i32*: skipList_i32_getSize, \
        SkipList_i64*: skipList_i64_getSize, \
        SkipList_u32*: skipList_u32_getSize, \
        SkipList_u64*: skipList_u64_getSize \
    )(list)

#define skipList_isEmpty(list) \
    _Generic((list), \
        SkipList_i32*: skipList_i32_isEmpty, \
        SkipList_i64*: skipList_i64_isEmpty, \
        SkipList_u32*: skipList_u32_isEmpty, \
        SkipList_u64*: skipList_u64_isEmpty \
    )(list)

#define skipList_destroy(list) \
    _Generic((list), \
        SkipList_i32**: skipList_i32_destroy, \
        SkipList_i64**: skipList_i64_destroy, \
        SkipList_u32**: skipList_u32_destroy, \
        SkipList_u64**: skipList_u64_destroy \
    )(list)

/*──────────────────────────────────────────────
    SkipMap (key → value) unified ops
  ──────────────────────────────────────────────*/
#define skipMap_put(sm, id, data) \
    _Generic((id), \
        int32_t:  skipMap_i32_put, \
        int64_t:  skipMap_i64_put, \
        uint32_t: skipMap_u32_put, \
        uint64_t: skipMap_u64_put \
    )(sm, id, data)

#define skipMap_get(sm, id) \
    _Generic((id), \
        int32_t:  skipMap_i32_get, \
        int64_t:  skipMap_i64_get, \
        uint32_t: skipMap_u32_get, \
        uint64_t: skipMap_u64_get \
    )(sm, id)

#define skipMap_remove(sm, id) \
    _Generic((id), \
        int32_t:  skipMap_i32_remove, \
        int64_t:  skipMap_i64_remove, \
        uint32_t: skipMap_u32_remove, \
        uint64_t: skipMap_u64_remove \
    )(sm, id)

#define skipMap_contains(sm, id) \
    _Generic((id), \
        int32_t:  skipMap_i32_contains, \
        int64_t:  skipMap_i64_contains, \
        uint32_t: skipMap_u32_contains, \
        uint64_t: skipMap_u64_contains \
    )(sm, id)

#define skipMap_destroy(sm) \
    _Generic((sm), \
        SkipMap_i32**: skipMap_i32_destroy, \
        SkipMap_i64**: skipMap_i64_destroy, \
        SkipMap_u32**: skipMap_u32_destroy, \
        SkipMap_u64**: skipMap_u64_destroy \
    )(sm)
#endif