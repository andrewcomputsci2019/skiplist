#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <skiplist.h>

#define TEST_SIZE 1000

// Helper for test messages
#define CHECK(expr, msg) \
    do { if (!(expr)) { \
        fprintf(stderr, "[FAIL] %s\n", msg); \
        return; \
    } else { \
        printf("[OK] %s\n", msg); \
    }} while (0)

void test_skiplist_u64(void) {
    printf("\n--- Testing skiplist_u64 ---\n");
    SkipList_u64 *list = skipList_u64_create();

    // Insert
    for (uint64_t i = 0; i < TEST_SIZE; i++)
        assert(skipList_u64_insert(list, i));

    // Remove half of them
    for (uint64_t i = 0; i < TEST_SIZE; i += 2)
        skipList_u64_remove(list, i);

    // Verify
    for (uint64_t i = 0; i < TEST_SIZE; i++) {
        bool found = skipList_u64_search(list, i);
        if (i % 2 == 0)
            CHECK(!found, "Removed element should not be found");
        else
            CHECK(found, "Remaining element should still exist");
    }

    printf("skiplist_u64 remove test passed ✅\n");
    skipList_u64_destroy(&list);
}

void test_skiplist_u32(void) {
    printf("\n--- Testing skiplist_u32 ---\n");
    SkipList_u32 *list = skipList_u32_create();

    for (uint32_t i = 0; i < TEST_SIZE; i++)
        assert(skipList_u32_insert(list, i));

    for (uint32_t i = 0; i < TEST_SIZE; i += 3)
        skipList_u32_remove(list, i);

    for (uint32_t i = 0; i < TEST_SIZE; i++) {
        bool found = skipList_u32_search(list, i);
        if (i % 3 == 0)
            CHECK(!found, "Removed element should not be found");
        else
            CHECK(found, "Remaining element should still exist");
    }

    printf("skiplist_u32 remove test passed ✅\n");
    skipList_u32_destroy(&list);
}

void test_skiplist_i32(void) {
    printf("\n--- Testing skiplist_i32 ---\n");
    SkipList_i32 *list = skipList_i32_create();

    for (int32_t i = 0; i < TEST_SIZE; i++)
        assert(skipList_i32_insert(list, i));

    for (int32_t i = -1; i < TEST_SIZE; i += 5)
        skipList_i32_remove(list, i);

    for (int32_t i = 0; i < TEST_SIZE; i++) {
        bool found = skipList_i32_search(list, i);
        if (i % 5 == 4)
            CHECK(!found, "Removed element should not be found");
        else
            CHECK(found, "Remaining element should still exist");
    }

    printf("skiplist_i32 remove test passed ✅\n");
    skipList_i32_destroy(&list);
}

void test_skiplist_i64(void) {
    printf("\n--- Testing skiplist_i64 ---\n");
    SkipList_i64 *list = skipList_i64_create();

    for (int64_t i = 0; i < TEST_SIZE; i++)
        assert(skipList_i64_insert(list, i));

    for (int64_t i = 0; i < TEST_SIZE; i += 4)
        skipList_i64_remove(list, i);

    for (int64_t i = 0; i < TEST_SIZE; i++) {
        bool found = skipList_i64_search(list, i);
        if (i % 4 == 0)
            CHECK(!found, "Removed element should not be found");
        else
            CHECK(found, "Remaining element should still exist");
    }

    printf("skiplist_i64 remove test passed ✅\n");
    skipList_i64_destroy(&list);
}

int main(void) {
    test_skiplist_i32();
    test_skiplist_i64();
    test_skiplist_u32();
    test_skiplist_u64();
    printf("\nAll remove tests completed successfully ✅\n");
    return 0;
}