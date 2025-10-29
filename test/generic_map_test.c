#include <skipmap_generic.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>

#define TEST_SIZE 10
#define INT_COMP_FUNC(a,b) ((a > b) - (a < b))

// Instantiate the skip map for int keys and uint values
DEFINE_GENERIC_SKIPMAP(SM_I32, int_fast32_t, uint_fast32_t, INT_COMP_FUNC, NO_OP, NO_OP)

int main(void) {
    printf("=== Running SkipMap_SM_I32 basic tests ===\n");

    SkipMap_SM_I32 *sm = SkipMap_SM_I32_create(0, 0);
    assert(sm != NULL);
    assert(SkipMap_SM_I32_isEmpty(sm));
    assert(SkipMap_SM_I32_size(sm) == 0);

    // Insert test entries
    for (int_fast32_t i = 0; i < TEST_SIZE; i++) {
        bool inserted = SkipMap_SM_I32_put(sm, i, (uint_fast32_t)(i * 10));
        assert(inserted);
    }

    // Validate size
    assert(SkipMap_SM_I32_size(sm) == TEST_SIZE);
    assert(!SkipMap_SM_I32_isEmpty(sm));

    // Check contains + get for each element
    for (int_fast32_t i = 0; i < TEST_SIZE; i++) {
        assert(SkipMap_SM_I32_contains(sm, i));
        uint_fast32_t value = 0;
        bool found = SkipMap_SM_I32_get(sm, i, &value);
        assert(found);
        assert(value == (uint_fast32_t)(i * 10));
    }

    // Check overwrite (should update existing key)
    for (int_fast32_t i = 0; i < TEST_SIZE; i++) {
        bool updated = SkipMap_SM_I32_put(sm, i, (uint_fast32_t)(i * 20));
        assert(updated); // since it overwrote an existing key
        uint_fast32_t value = 0;
        bool found = SkipMap_SM_I32_get(sm, i, &value);
        assert(found);
        assert(value == (uint_fast32_t)(i * 20));
    }

    // Remove half of the elements
    for (int_fast32_t i = 0; i < TEST_SIZE; i += 2) {
        uint_fast32_t removed_val = 0;
        bool removed = SkipMap_SM_I32_remove(sm, i, &removed_val);
        assert(removed);
        assert(removed_val == (uint_fast32_t)(i * 20));
    }

    // Verify that removed elements are gone
    for (int_fast32_t i = 0; i < TEST_SIZE; i++) {
        if (i % 2 == 0) {
            assert(!SkipMap_SM_I32_contains(sm, i));
            uint_fast32_t dummy = 12345;
            assert(!SkipMap_SM_I32_get(sm, i, &dummy));
        } else {
            assert(SkipMap_SM_I32_contains(sm, i));
            uint_fast32_t value = 0;
            assert(SkipMap_SM_I32_get(sm, i, &value));
            assert(value == (uint_fast32_t)(i * 20));
        }
    }

    // Check size again
    uint32_t expected_size = TEST_SIZE / 2;
    assert(SkipMap_SM_I32_size(sm) == expected_size);

    // Remove all remaining
    for (int_fast32_t i = 1; i < TEST_SIZE; i += 2) {
        uint_fast32_t val = 0;
        bool removed = SkipMap_SM_I32_remove(sm, i, &val);
        assert(removed);
    }

    assert(SkipMap_SM_I32_isEmpty(sm));
    assert(SkipMap_SM_I32_size(sm) == 0);

    // Destroy and ensure pointer is nulled
    SkipMap_SM_I32_destroy(&sm);
    assert(sm == NULL);

    printf("✅ SkipMap_SM_I32 tests passed successfully.\n");
    return 0;
}