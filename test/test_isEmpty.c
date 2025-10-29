#include <skiplist.h>
#include <assert.h>
#include <stdio.h>

// ============================================================
// Test: skipList_*_isEmpty
// ============================================================
// This test verifies that the skiplist correctly reports empty
// and non-empty states for all four numeric variants:
//   - i32, u32, i64, u64
// ============================================================

static void test_isEmpty_i32(void) {
    printf("[Test] skipList_i32_isEmpty ... ");
    SkipList_i32 *sl = skipList_i32_create();
    assert(skipList_i32_isEmpty(sl));

    assert(skipList_i32_insert(sl, 1));
    assert(!skipList_i32_isEmpty(sl));

    skipList_i32_destroy(&sl);
    printf("OK\n");
}

static void test_isEmpty_u32(void) {
    printf("[Test] skipList_u32_isEmpty ... ");
    SkipList_u32 *sl = skipList_u32_create();
    assert(skipList_u32_isEmpty(sl));

    assert(skipList_u32_insert(sl, 1));
    assert(!skipList_u32_isEmpty(sl));

    skipList_u32_destroy(&sl);
    printf("OK\n");
}

static void test_isEmpty_i64(void) {
    printf("[Test] skipList_i64_isEmpty ... ");
    SkipList_i64 *sl = skipList_i64_create();
    assert(skipList_i64_isEmpty(sl));

    assert(skipList_i64_insert(sl, 1));
    assert(!skipList_i64_isEmpty(sl));

    skipList_i64_destroy(&sl);
    printf("OK\n");
}

static void test_isEmpty_u64(void) {
    printf("[Test] skipList_u64_isEmpty ... ");
    SkipList_u64 *sl = skipList_u64_create();
    assert(skipList_u64_isEmpty(sl));

    assert(skipList_u64_insert(sl, 1));
    assert(!skipList_u64_isEmpty(sl));

    skipList_u64_destroy(&sl);
    printf("OK\n");
}

// ============================================================
// Main test entry
// ============================================================

int main(void) {
    printf("===== SkipList isEmpty Tests =====\n");
    test_isEmpty_i32();
    test_isEmpty_u32();
    test_isEmpty_i64();
    test_isEmpty_u64();
    printf("All isEmpty tests passed ✅\n");
    return 0;
}