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
   SkipList_i32 size test
   ============================================================ */
void test_skiplist_i32_size(void) {
    printf("\n--- Testing SkipList_i32::size ---\n");
    SkipList_i32 *list = skipList_i32_create();

    CHECK(skipList_i32_isEmpty(list), "new list is empty (i32)");
    CHECK(skipList_i32_getSize(list) == 0, "size == 0 initially (i32)");

    for (int i = 0; i < TEST_SIZE; i++)
        skipList_i32_insert(list, i);

    CHECK(skipList_i32_getSize(list) == TEST_SIZE, "size == TEST_SIZE after insert (i32)");
    CHECK(!skipList_i32_isEmpty(list), "list not empty after insert (i32)");

    // Remove half
    for (int i = 0; i < TEST_SIZE; i += 2)
        skipList_i32_remove(list, i);

    CHECK(skipList_i32_getSize(list) == TEST_SIZE / 2, "size == TEST_SIZE/2 after removals (i32)");

    // Remove remaining
    for (int i = 1; i < TEST_SIZE; i += 2)
        skipList_i32_remove(list, i);

    CHECK(skipList_i32_getSize(list) == 0, "size == 0 after full clear (i32)");
    CHECK(skipList_i32_isEmpty(list), "list empty after all removals (i32)");

    skipList_i32_destroy(&list);
    CHECK(list == NULL, "list destroyed (i32)");
    printf("SkipList_i32 size test passed ✅\n");
}

/* ============================================================
   SkipList_u32 size test
   ============================================================ */
void test_skiplist_u32_size(void) {
    printf("\n--- Testing SkipList_u32::size ---\n");
    SkipList_u32 *list = skipList_u32_create();

    CHECK(skipList_u32_isEmpty(list), "new list is empty (u32)");
    CHECK(skipList_u32_getSize(list) == 0, "size == 0 initially (u32)");

    for (uint32_t i = 0; i < TEST_SIZE; i++)
        skipList_u32_insert(list, i);

    CHECK(skipList_u32_getSize(list) == TEST_SIZE, "size == TEST_SIZE after insert (u32)");
    CHECK(!skipList_u32_isEmpty(list), "list not empty after insert (u32)");

    for (uint32_t i = 0; i < TEST_SIZE; i += 2)
        skipList_u32_remove(list, i);

    CHECK(skipList_u32_getSize(list) == TEST_SIZE / 2, "size == TEST_SIZE/2 after removals (u32)");

    for (uint32_t i = 1; i < TEST_SIZE; i += 2)
        skipList_u32_remove(list, i);

    CHECK(skipList_u32_getSize(list) == 0, "size == 0 after full clear (u32)");
    CHECK(skipList_u32_isEmpty(list), "list empty after all removals (u32)");

    skipList_u32_destroy(&list);
    CHECK(list == NULL, "list destroyed (u32)");
    printf("SkipList_u32 size test passed ✅\n");
}

/* ============================================================
   SkipList_i64 size test
   ============================================================ */
void test_skiplist_i64_size(void) {
    printf("\n--- Testing SkipList_i64::size ---\n");
    SkipList_i64 *list = skipList_i64_create();

    CHECK(skipList_i64_isEmpty(list), "new list is empty (i64)");
    CHECK(skipList_i64_getSize(list) == 0, "size == 0 initially (i64)");

    for (int64_t i = 0; i < TEST_SIZE; i++)
        skipList_i64_insert(list, i);

    CHECK(skipList_i64_getSize(list) == TEST_SIZE, "size == TEST_SIZE after insert (i64)");
    CHECK(!skipList_i64_isEmpty(list), "list not empty after insert (i64)");

    for (int64_t i = 0; i < TEST_SIZE; i += 2)
        skipList_i64_remove(list, i);

    CHECK(skipList_i64_getSize(list) == TEST_SIZE / 2, "size == TEST_SIZE/2 after removals (i64)");

    for (int64_t i = 1; i < TEST_SIZE; i += 2)
        skipList_i64_remove(list, i);

    CHECK(skipList_i64_getSize(list) == 0, "size == 0 after full clear (i64)");
    CHECK(skipList_i64_isEmpty(list), "list empty after all removals (i64)");

    skipList_i64_destroy(&list);
    CHECK(list == NULL, "list destroyed (i64)");
    printf("SkipList_i64 size test passed ✅\n");
}

/* ============================================================
   SkipList_u64 size test
   ============================================================ */
void test_skiplist_u64_size(void) {
    printf("\n--- Testing SkipList_u64::size ---\n");
    SkipList_u64 *list = skipList_u64_create();

    CHECK(skipList_u64_isEmpty(list), "new list is empty (u64)");
    CHECK(skipList_u64_getSize(list) == 0, "size == 0 initially (u64)");

    for (uint64_t i = 0; i < TEST_SIZE; i++)
        skipList_u64_insert(list, i);

    CHECK(skipList_u64_getSize(list) == TEST_SIZE, "size == TEST_SIZE after insert (u64)");
    CHECK(!skipList_u64_isEmpty(list), "list not empty after insert (u64)");

    for (uint64_t i = 0; i < TEST_SIZE; i += 2)
        skipList_u64_remove(list, i);

    CHECK(skipList_u64_getSize(list) == TEST_SIZE / 2, "size == TEST_SIZE/2 after removals (u64)");

    for (uint64_t i = 1; i < TEST_SIZE; i += 2)
        skipList_u64_remove(list, i);

    CHECK(skipList_u64_getSize(list) == 0, "size == 0 after full clear (u64)");
    CHECK(skipList_u64_isEmpty(list), "list empty after all removals (u64)");

    skipList_u64_destroy(&list);
    CHECK(list == NULL, "list destroyed (u64)");
    printf("SkipList_u64 size test passed ✅\n");
}

/* ============================================================
   Main entry point
   ============================================================ */
int main(void) {
    test_skiplist_i32_size();
    test_skiplist_u32_size();
    test_skiplist_i64_size();
    test_skiplist_u64_size();

    printf("\n✅ All skiplist size tests passed successfully!\n");
    return 0;
}
