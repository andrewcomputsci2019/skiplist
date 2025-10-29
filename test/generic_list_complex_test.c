#include <skiplist_generic.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// ================================
// Complex key type: 2D coordinate
// ================================
typedef struct {
    int x;
    int y;
} Point;


#define CMP_POINT_XY(a, b) ({                     \
    const Point *pa__ = (const Point*)(a);        \
    const Point *pb__ = (const Point*)(b);        \
    int dx__ = (pa__->x > pb__->x) - (pa__->x < pb__->x); \
    dx__ != 0 ? dx__ :                            \
    ( (pa__->y > pb__->y) - (pa__->y < pb__->y) );\
})

static inline void free_point(Point * ptr){
    if(!ptr)return;
    printf("freeing point (%d,%d)\n", ptr->x,ptr->y);
    free(ptr);
}

DEFINE_GENERIC_SKIPLIST(SL_Point, Point*, CMP_POINT_XY, free_point)


#define TEST_SIZE 8



int main(void) {
    printf("=== Running SkipList<Point*> complex-type tests ===\n");

    // Sentinel (unused but required)
    Point *sentinel = malloc(sizeof(Point));
    sentinel->x = 0;
    sentinel->y = 0;

    // Create skiplist
    SkipList_SL_Point *sl = SkipList_SL_Point_create(sentinel);
    assert(sl);
    assert(SkipList_SL_Point_isEmpty(sl));

    // --- Insert test data ---
    for (int i = 0; i < TEST_SIZE; i++) {
        Point *p = malloc(sizeof(Point));
        p->x = i;
        p->y = TEST_SIZE - i;
        bool inserted = SkipList_SL_Point_insert(sl, p);
        assert(inserted);
    }

    assert(SkipList_SL_Point_size(sl) == TEST_SIZE);
    assert(!SkipList_SL_Point_isEmpty(sl));

    // --- Search test ---
    for (int i = 0; i < TEST_SIZE; i++) {
        Point tmp = { .x = i, .y = TEST_SIZE - i };
        Point *pkey = &tmp;
        assert(SkipList_SL_Point_search(sl, pkey));
    }

    // --- Remove half the points manually ---
    printf("Removing even keys manually:\n");
    for (int i = 0; i < TEST_SIZE; i += 2) {
        Point tmp = { .x = i, .y = TEST_SIZE - i };
        Point *key = &tmp;
        bool removed = SkipList_SL_Point_remove(sl, key);
        assert(removed);
    }

    // Verify half removed
    for (int i = 0; i < TEST_SIZE; i++) {
        Point tmp = { .x = i, .y = TEST_SIZE - i };
        Point *key = &tmp;
        bool found = SkipList_SL_Point_search(sl, key);
        if (i % 2 == 0)
            assert(!found);
        else
            assert(found);
    }

    // --- Destroy skiplist (auto cleanup remaining keys) ---
    printf("Destroying skiplist, expect cleanup for odd keys:\n");
    SkipList_SL_Point_destroy(&sl);
    assert(sl == NULL);

    // Sentinel cleanup (not managed)
    free(sentinel);

    printf("✅ SkipList<Point*> complex pointer test passed successfully.\n");
    return 0;
}
