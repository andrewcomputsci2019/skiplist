#include <skiplist.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#define TEST_SIZE 1000

#define CHECK(expr, msg) \
    do { if (!(expr)) { \
        fprintf(stderr, "[FAIL] %s\n", msg); \
        return; \
    } else { \
        printf("[OK] %s\n", msg); \
    }} while (0)

/* ============================================================
   Helper test for SkipList_i32
   ============================================================ */
void test_skiplist_i32_search(void) {
    printf("\n--- Testing SkipList_i32::search ---\n");
    SkipList_i32 *list = skipList_i32_create();

    for (int i = 0; i < TEST_SIZE; i++)
        assert(skipList_i32_insert(list, i));

    // Verify each element is found
    for (int i = 0; i < TEST_SIZE; i++)
        CHECK(skipList_i32_search(list, i), "search existing (i32)");

    // Search for elements not in list
    for (int i = TEST_SIZE; i < TEST_SIZE + 10; i++)
        CHECK(!skipList_i32_search(list, i), "search missing (i32)");

    // Remove and verify absence
    for (int i = 0; i < TEST_SIZE; i += 10)
        skipList_i32_remove(list, i);

    for (int i = 0; i < TEST_SIZE; i++) {
        bool found = skipList_i32_search(list, i);
        if (i % 10 == 0)
            CHECK(!found, "removed element not found (i32)");
        else
            CHECK(found, "remaining element found (i32)");
    }

    skipList_i32_destroy(&list);
    CHECK(list == NULL, "list destroyed (i32)");
    printf("SkipList_i32 search test passed ✅\n");
}

/* ============================================================
   Helper test for SkipList_u32
   ============================================================ */
void test_skiplist_u32_search(void) {
    printf("\n--- Testing SkipList_u32::search ---\n");
    SkipList_u32 *list = skipList_u32_create();

    for (uint32_t i = 0; i < TEST_SIZE; i++)
        assert(skipList_u32_insert(list, i));

    for (uint32_t i = 0; i < TEST_SIZE; i++)
        CHECK(skipList_u32_search(list, i), "search existing (u32)");

    for (uint32_t i = TEST_SIZE; i < TEST_SIZE + 10; i++)
        CHECK(!skipList_u32_search(list, i), "search missing (u32)");

    for (uint32_t i = 0; i < TEST_SIZE; i += 10)
        skipList_u32_remove(list, i);

    for (uint32_t i = 0; i < TEST_SIZE; i++) {
        bool found = skipList_u32_search(list, i);
        if (i % 10 == 0)
            CHECK(!found, "removed element not found (u32)");
        else
            CHECK(found, "remaining element found (u32)");
    }

    skipList_u32_destroy(&list);
    CHECK(list == NULL, "list destroyed (u32)");
    printf("SkipList_u32 search test passed ✅\n");
}

/* ============================================================
   Helper test for SkipList_i64
   ============================================================ */
void test_skiplist_i64_search(void) {
    printf("\n--- Testing SkipList_i64::search ---\n");
    SkipList_i64 *list = skipList_i64_create();

    for (int64_t i = 0; i < TEST_SIZE; i++)
        assert(skipList_i64_insert(list, i));

    for (int64_t i = 0; i < TEST_SIZE; i++)
        CHECK(skipList_i64_search(list, i), "search existing (i64)");

    for (int64_t i = TEST_SIZE; i < TEST_SIZE + 10; i++)
        CHECK(!skipList_i64_search(list, i), "search missing (i64)");

    for (int64_t i = 0; i < TEST_SIZE; i += 10)
        skipList_i64_remove(list, i);

    for (int64_t i = 0; i < TEST_SIZE; i++) {
        bool found = skipList_i64_search(list, i);
        if (i % 10 == 0)
            CHECK(!found, "removed element not found (i64)");
        else
            CHECK(found, "remaining element found (i64)");
    }

    skipList_i64_destroy(&list);
    CHECK(list == NULL, "list destroyed (i64)");
    printf("SkipList_i64 search test passed ✅\n");
}

/* ============================================================
   Helper test for SkipList_u64
   ============================================================ */
void test_skiplist_u64_search(void) {
    printf("\n--- Testing SkipList_u64::search ---\n");
    SkipList_u64 *list = skipList_u64_create();

    for (uint64_t i = 0; i < TEST_SIZE; i++)
        assert(skipList_u64_insert(list, i));

    for (uint64_t i = 0; i < TEST_SIZE; i++)
        CHECK(skipList_u64_search(list, i), "search existing (u64)");

    for (uint64_t i = TEST_SIZE; i < TEST_SIZE + 10; i++)
        CHECK(!skipList_u64_search(list, i), "search missing (u64)");

    for (uint64_t i = 0; i < TEST_SIZE; i += 10)
        skipList_u64_remove(list, i);

    for (uint64_t i = 0; i < TEST_SIZE; i++) {
        bool found = skipList_u64_search(list, i);
        if (i % 10 == 0)
            CHECK(!found, "removed element not found (u64)");
        else
            CHECK(found, "remaining element found (u64)");
    }

    skipList_u64_destroy(&list);
    CHECK(list == NULL, "list destroyed (u64)");
    printf("SkipList_u64 search test passed ✅\n");
}

/* ============================================================
   Main Entry Point
   ============================================================ */
int main(void) {
    test_skiplist_i32_search();
    test_skiplist_u32_search();
    test_skiplist_i64_search();
    test_skiplist_u64_search();

    printf("\n✅ All skiplist search tests passed successfully!\n");
    return 0;
}