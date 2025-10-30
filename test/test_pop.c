//
// Created by Andrew Pegg on 10/29/25.
//



#include <skiplist.h>
#include <stdio.h>
#include <assert.h>

void test_pop_i32() {
    printf("test_pop_i32()\n");
    SkipList_i32 * sl = skipList_i32_create();
    SkipMap_i32 * sm = skipMap_i32_create();

    //insert 10 lines of data
    printf("[test_pop_i32] inserting 10 elements into skiplist\n");
    for (int i = 0; i < 10; i++) {
        assert(skipList_i32_insert(sl,i));
    }
    // now pop them
    printf("[test_pop_i32] removing 10 elements from skiplist and checking order\n");
    for (int i = 0; i < 10; i++) {
        int p;
        assert(skipList_i32_pop(sl,&p));
        assert(p == i);
    }
    assert(skipList_i32_isEmpty(sl));
    skipList_i32_destroy(&sl);
    printf("[test_pop_i32] inserting 10 elements into skipMap\n");
    for (int i = 0; i < 10; i++) {
        assert(skipMap_i32_put(sm,i,(void*)i));
    }
    // now pop the data
    printf("[test_pop_i32] removing 10 elements from skipMap and checking order\n");
    for (int i = 0; i < 10; i++) {
        struct SM_i32_kv p;
        assert(skipMap_i32_pop(sm,&p));
        assert(p.key == i && p.key == (int)p.value);
    }
    assert(skipMap_i32_isEmpty(sm));
    skipMap_i32_destroy(&sm);
    printf("[test_pop_i32] ✅\n");
}

void test_pop_i64() {
    printf("test_pop_i64()\n");
    SkipList_i64 * sl = skipList_i64_create();
    SkipMap_i64 * sm = skipMap_i64_create();

    //insert 10 lines of data
    printf("[test_pop_i64] inserting 10 elements into skiplist\n");
    for (int i = 0; i < 10; i++) {
        assert(skipList_i64_insert(sl,i));
    }
    // now pop them
    printf("[test_pop_i64] removing 10 elements from skiplist and checking order\n");
    for (int i = 0; i < 10; i++) {
        int64_t p;
        assert(skipList_i64_pop(sl,&p));
        assert(p == i);
    }
    assert(skipList_i64_isEmpty(sl));
    skipList_i64_destroy(&sl);
    printf("[test_pop_i64] inserting 10 elements into skipMap\n");
    for (int i = 0; i < 10; i++) {
        assert(skipMap_i64_put(sm,i,(void*)i));
    }
    // now pop the data
    printf("[test_pop_i64] removing 10 elements from skipMap and checking order\n");
    for (int i = 0; i < 10; i++) {
        struct SM_i64_kv p;
        assert(skipMap_i64_pop(sm,&p));
        assert(p.key == i && p.key == (int64_t)p.value);
    }
    assert(skipMap_i64_isEmpty(sm));
    skipMap_i64_destroy(&sm);
    printf("[test_pop_i32] ✅\n");
}

void test_pop_u32() {
    printf("test_pop_u32()\n");
    SkipList_u32 * sl = skipList_u32_create();
    SkipMap_u32 * sm = skipMap_u32_create();

    //insert 10 lines of data
    printf("[test_pop_u32] inserting 10 elements into skiplist\n");
    for (int i = 0; i < 10; i++) {
        assert(skipList_u32_insert(sl,i));
    }
    // now pop them
    printf("[test_pop_u32] removing 10 elements from skiplist and checking order\n");
    for (int i = 0; i < 10; i++) {
        int p;
        assert(skipList_u32_pop(sl,&p));
        assert(p == i);
    }
    assert(skipList_u32_isEmpty(sl));
    skipList_u32_destroy(&sl);
    printf("[test_pop_u32] inserting 10 elements into skipMap\n");
    for (int i = 0; i < 10; i++) {
        assert(skipMap_u32_put(sm,i,(void*)i));
    }
    // now pop the data
    printf("[test_pop_u32] removing 10 elements from skipMap and checking order\n");
    for (int i = 0; i < 10; i++) {
        struct SM_u32_kv p;
        assert(skipMap_u32_pop(sm,&p));
        assert(p.key == i && p.key == (uint32_t)p.value);
    }
    assert(skipMap_u32_isEmpty(sm));
    skipMap_u32_destroy(&sm);
    printf("[test_pop_u32] ✅\n");
}

void test_pop_u64() {
    printf("test_pop_u64()\n");
    SkipList_u64 * sl = skipList_u64_create();
    SkipMap_u64 * sm = skipMap_u64_create();

    //insert 10 lines of data
    printf("[test_pop_u64] inserting 10 elements into skiplist\n");
    for (int i = 0; i < 10; i++) {
        assert(skipList_u64_insert(sl,i));
    }
    // now pop them
    printf("[test_pop_u64] removing 10 elements from skiplist and checking order\n");
    for (int i = 0; i < 10; i++) {
        uint64_t p;
        assert(skipList_u64_pop(sl,&p));
        assert(p == i);
    }
    assert(skipList_u64_isEmpty(sl));
    skipList_u64_destroy(&sl);
    printf("[test_pop_u64] inserting 10 elements into skipMap\n");
    for (int i = 0; i < 10; i++) {
        assert(skipMap_u64_put(sm,i,(void*)i));
    }
    // now pop the data
    printf("[test_pop_u64] removing 10 elements from skipMap and checking order\n");
    for (int i = 0; i < 10; i++) {
        struct SM_u64_kv p;
        assert(skipMap_u64_pop(sm,&p));
        assert(p.key == i && p.key == (uint64_t)p.value);
    }
    assert(skipMap_u64_isEmpty(sm));
    skipMap_u64_destroy(&sm);
    printf("[test_pop_u64] ✅\n");
}

int main() {
    test_pop_i32();
    test_pop_u32();
    test_pop_u64();
    test_pop_i64();
}