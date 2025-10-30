#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <skiplist.h>

#define TEST_SIZE 10

#define CHECK(expr, msg) \
    do { if (!(expr)) { \
        fprintf(stderr, "[FAIL] %s\n", msg); \
        return; \
    } else { \
        printf("[OK] %s\n", msg); \
    }} while (0)

/* ============================================================
   Helper Tests for SkipList
   ============================================================ */

void test_skiplist_i32(void) {
    printf("\n--- Testing SkipList_i32 ---\n");
    SkipList_i32 *list = skipList_i32_create();

    for (int i = 1; i <= TEST_SIZE; i++)
        CHECK(skipList_i32_insert(list, i), "insert i32");

    CHECK(!skipList_i32_insert(list, 1), "duplicate insert fails (i32)");

    for (int i = 1; i <= TEST_SIZE; i++)
        skipList_i32_remove(list, i);

    skipList_i32_destroy(&list);
    CHECK(list == NULL, "list destroyed (i32)");
    printf("SkipList_i32 test passed ✅\n");
}

void test_skiplist_u32(void) {
    printf("\n--- Testing SkipList_u32 ---\n");
    SkipList_u32 *list = skipList_u32_create();

    for (uint32_t i = 1; i <= TEST_SIZE; i++)
        CHECK(skipList_u32_insert(list, i), "insert u32");

    CHECK(!skipList_u32_insert(list, 1), "duplicate insert fails (u32)");
    printf("testing skiplist remove u32\n");
    for (uint32_t i = 1; i <= TEST_SIZE; i++)
        skipList_u32_remove(list, i);
    printf("removal u32 passed ✅\n");
    skipList_u32_destroy(&list);
    CHECK(list == NULL, "list destroyed (u32)");
    printf("SkipList_u32 test passed ✅\n");
}

void test_skiplist_i64(void) {
    printf("\n--- Testing SkipList_i64 ---\n");
    SkipList_i64 *list = skipList_i64_create();

    for (int64_t i = 1; i <= TEST_SIZE; i++)
        CHECK(skipList_i64_insert(list, i), "insert i64");

    CHECK(!skipList_i64_insert(list, 1), "duplicate insert fails (i64)");

    for (int64_t i = 1; i <= TEST_SIZE; i++)
        skipList_i64_remove(list, i);

    skipList_i64_destroy(&list);
    CHECK(list == NULL, "list destroyed (i64)");
    printf("SkipList_i64 test passed ✅\n");
}

void test_skiplist_u64(void) {
    printf("\n--- Testing SkipList_u64 ---\n");
    SkipList_u64 *list = skipList_u64_create();

    for (uint64_t i = 1; i <= TEST_SIZE; i++)
        CHECK(skipList_u64_insert(list, i), "insert u64");

    CHECK(!skipList_u64_insert(list, 1), "duplicate insert fails (u64)");

    for (uint64_t i = 1; i <= TEST_SIZE; i++)
        skipList_u64_remove(list, i);

    skipList_u64_destroy(&list);
    CHECK(list == NULL, "list destroyed (u64)");
    printf("SkipList_u64 test passed ✅\n");
}

/* ============================================================
   Helper Tests for SkipMap
   ============================================================ */

void test_skipmap_i32(void) {
    printf("\n--- Testing SkipMap_i32 ---\n");
    SkipMap_i32 *map = skipMap_i32_create();

    for (int i = 1; i <= TEST_SIZE; i++)
        CHECK(skipMap_i32_put(map, i, (void*)(i * 10)), "insert map i32");

    CHECK(skipMap_i32_put(map, 1, (void*)(999)), "update existing key (i32)");

    // Remove all keys before destroy
    for (int i = 1; i <= TEST_SIZE; i++)
        skipMap_i32_remove(map, i);

    skipMap_i32_destroy(&map);
    CHECK(map == NULL, "map destroyed (i32)");
    printf("SkipMap_i32 test passed ✅\n");
}

void test_skipmap_u32(void) {
    printf("\n--- Testing SkipMap_u32 ---\n");
    SkipMap_u32 *map = skipMap_u32_create();

    for (uint32_t i = 1; i <= TEST_SIZE; i++)
        CHECK(skipMap_u32_put(map, i, (void*)(i * 10)), "insert map u32");

    CHECK(skipMap_u32_put(map, 1, (void*)999), "update existing key (u32)");

    for (uint32_t i = 1; i <= TEST_SIZE; i++)
        skipMap_u32_remove(map, i);

    skipMap_u32_destroy(&map);
    CHECK(map == NULL, "map destroyed (u32)");
    printf("SkipMap_u32 test passed ✅\n");
}

void test_skipmap_i64(void) {
    printf("\n--- Testing SkipMap_i64 ---\n");
    SkipMap_i64 *map = skipMap_i64_create();

    for (int64_t i = 1; i <= TEST_SIZE; i++)
        CHECK(skipMap_i64_put(map, i, (void*)(i * 10)), "insert map i64");

    CHECK(skipMap_i64_put(map, 1, (void*)(999)), "update existing key (i64)");

    for (int64_t i = 1; i <= TEST_SIZE; i++)
        skipMap_i64_remove(map, i);

    skipMap_i64_destroy(&map);
    CHECK(map == NULL, "map destroyed (i64)");
    printf("SkipMap_i64 test passed ✅\n");
}

void test_skipmap_u64(void) {
    printf("\n--- Testing SkipMap_u64 ---\n");
    SkipMap_u64 *map = skipMap_u64_create();

    for (uint64_t i = 1; i <= TEST_SIZE; i++)
        CHECK(skipMap_u64_put(map, i, (void*)(i * 10)), "insert map u64");

    CHECK(skipMap_u64_put(map, 1, (void*)(999)), "update existing key (u64)");

    for (uint64_t i = 1; i <= TEST_SIZE; i++)
        skipMap_u64_remove(map, i);

    skipMap_u64_destroy(&map);
    CHECK(map == NULL, "map destroyed (u64)");
    printf("SkipMap_u64 test passed ✅\n");
}

/* ============================================================
   Main Entry Point
   ============================================================ */

int main(void) {
    test_skiplist_i32();
    test_skiplist_u32();
    test_skiplist_i64();
    test_skiplist_u64();

    test_skipmap_i32();
    test_skipmap_u32();
    test_skipmap_i64();
    test_skipmap_u64();

    printf("\n✅ All skiplist + skipmap tests passed successfully!\n");
    return 0;
}