#include <skipmap_generic.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// ====== Complex Data Example ======
typedef struct {
    double radius;
    char *label; // dynamically allocated label to test cleanup
} Circle;

// Comparison by radius (ascending)
#define CIRCLE_CMP(a,b) (((a).radius > (b).radius) - ((a).radius < (b).radius))

// On remove: free the label
static inline void free_circle_data(Circle c) {
    printf("cleaning up circle %s\n",c.label);
    if (c.label) {
        free(c.label);
        c.label = NULL;
    }
}

static inline void notify_key_remove_func(Circle c){
    printf("circle with radius key %lf is being removd\n",c.radius);
}

// Instantiate a generic SkipMap for Circle keys and Circle data
DEFINE_GENERIC_SKIPMAP(SM_Circle, Circle, Circle, CIRCLE_CMP, notify_key_remove_func, free_circle_data)

#define TEST_SIZE 5

int main(void) {
    printf("=== Running SkipMap<Circle, Circle> tests ===\n");

    // Create map with sentinel key and value
    Circle sentinel = { .radius = 0.0, .label = NULL };
    SkipMap_SM_Circle *sm = SkipMap_SM_Circle_create(sentinel, sentinel);
    assert(sm != NULL);
    assert(SkipMap_SM_Circle_isEmpty(sm));

    // Insert circles with increasing radius
    for (int i = 0; i < TEST_SIZE; i++) {
        Circle key = { .radius = i + 1.0, .label = NULL };
        Circle data = {
            .radius = i + 1.0,
            .label = malloc(32)
        };
        snprintf(data.label, 32, "Circle_%d", i + 1);
        assert(SkipMap_SM_Circle_put(sm, key, data));
    }

    assert(SkipMap_SM_Circle_size(sm) == TEST_SIZE);

    // Verify each key is contained and retrievable
    for (int i = 0; i < TEST_SIZE; i++) {
        Circle key = { .radius = i + 1.0, .label = NULL };
        assert(SkipMap_SM_Circle_contains(sm, key));

        Circle got;
        bool found = SkipMap_SM_Circle_get(sm, key, &got);
        assert(found);
        printf("Found key radius=%.1f → value.label=%s\n", key.radius, got.label);
    }

    // Overwrite (update) some values
    for (int i = 0; i < TEST_SIZE; i += 2) {
        Circle key = { .radius = i + 1.0, .label = NULL };
        Circle data = {
            .radius = i + 1.0,
            .label = malloc(32)
        };
        snprintf(data.label, 32, "Circle_updated_%d", i + 1);
        assert(SkipMap_SM_Circle_put(sm, key, data));
    }

    // Remove every circle, triggering free_circle_data
    for (int i = 0; i < TEST_SIZE/2; i++) {
        Circle key = { .radius = i + 1.0, .label = NULL };
        Circle removed = {0};
        bool ok = SkipMap_SM_Circle_remove(sm, key, &removed);
        assert(ok);
        // removed.label is already freed by callback
    }
    for (int i = TEST_SIZE/2; i < TEST_SIZE; i++) {
        struct SM_SM_Circle_kv p;
        bool ok = SkipMap_SM_Circle_pop(sm,&p);
        assert(ok);
        printf("Popped return (%lf,%s)\n",p.key.radius,p.value.label);
        free(p.value.label);
    }

    assert(SkipMap_SM_Circle_isEmpty(sm));
    assert(SkipMap_SM_Circle_size(sm) == 0);

    // Destroy map (no leaks if cleanup worked)
    SkipMap_SM_Circle_destroy(&sm);
    assert(sm == NULL);

    printf("✅ SkipMap<Circle, Circle> complex data tests passed successfully.\n");
    sm = SkipMap_SM_Circle_create(sentinel,sentinel);
    assert(sm != NULL);
    assert(SkipMap_SM_Circle_isEmpty(sm));
    for (int i = 0; i < TEST_SIZE; i++) {
        Circle key = { .radius = i + 1.0, .label = NULL };
        Circle data = {
            .radius = i + 1.0,
            .label = malloc(32)
        };
        snprintf(data.label, 32, "Circle_%d", i + 1);
        assert(SkipMap_SM_Circle_put(sm, key, data));
    }

    assert(SkipMap_SM_Circle_size(sm) == TEST_SIZE);
    SkipMap_SM_Circle_destroy(&sm);
    assert(sm == NULL);
    printf("✅ SkipMap<Circle, Circle> complex data tests passed successfully, with cleanup.\n");
    return 0;
}