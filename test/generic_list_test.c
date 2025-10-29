#include <skiplist_generic.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define TEST_SIZE 10
#define cmp_int_func(a,b) ((a > b) - (a < b))
DEFINE_GENERIC_SKIPLIST(SL_i32, int_fast32_t, cmp_int_func, NO_OP)


int main(void) {
    printf("=== Running SkipList<int_fast32_t> generic tests ===\n");

    // Create skiplist
    SkipList_SL_i32 *sl = SkipList_SL_i32_create(0);
    assert(sl != NULL);
    assert(SkipList_SL_i32_isEmpty(sl));
    assert(SkipList_SL_i32_size(sl) == 0);

    // Insert sequential keys
    for (int_fast32_t i = 1; i <= TEST_SIZE; i++) {
        bool inserted = SkipList_SL_i32_insert(sl, i);
        assert(inserted);
        assert(SkipList_SL_i32_search(sl, i));
    }
    assert(SkipList_SL_i32_size(sl) == TEST_SIZE);
    assert(!SkipList_SL_i32_isEmpty(sl));

    // Duplicates should fail to insert
    for (int_fast32_t i = 1; i <= TEST_SIZE; i++) {
        assert(!SkipList_SL_i32_insert(sl, i));
    }
    assert(SkipList_SL_i32_size(sl) == TEST_SIZE);

    // Remove every other key
    for (int_fast32_t i = 1; i <= TEST_SIZE; i += 2) {
        bool removed = SkipList_SL_i32_remove(sl, i);
        assert(removed);
        assert(!SkipList_SL_i32_search(sl, i));  // ensure it's gone
    }

    // Remaining elements should still exist
    for (int_fast32_t i = 2; i <= TEST_SIZE; i += 2) {
        assert(SkipList_SL_i32_search(sl, i));
    }

    // Size should now be half
    assert(SkipList_SL_i32_size(sl) == TEST_SIZE / 2);

    // Remove remaining keys
    for (int_fast32_t i = 2; i <= TEST_SIZE; i += 2) {
        bool removed = SkipList_SL_i32_remove(sl, i);
        assert(removed);
    }
    assert(SkipList_SL_i32_isEmpty(sl));
    assert(SkipList_SL_i32_size(sl) == 0);

    // Try removing nonexistent key
    assert(!SkipList_SL_i32_remove(sl, 999));

    // Cleanup
    SkipList_SL_i32_destroy(&sl);
    assert(sl == NULL);

    printf("✅ SkipList<int_fast32_t> tests passed successfully.\n");
    return 0;
}
