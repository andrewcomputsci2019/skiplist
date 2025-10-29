#define _POSIX_C_SOURCE 200809L
#include <skiplist.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

#define REPEATS 5
#define US_PER_SEC 1000000L
#define NS_PER_SEC 1000000000L




static inline long time_diff_ns(struct timespec start, struct timespec end) {
    return (end.tv_sec - start.tv_sec) * NS_PER_SEC + (end.tv_nsec - start.tv_nsec);
}

static int cmp_long(const void *a, const void *b) {
    long x = *(const long *)a, y = *(const long *)b;
    return (x > y) - (x < y);
}

static long compute_mode(long *vals, int n) {
    qsort(vals, n, sizeof(long), cmp_long);
    long mode = vals[0], best_count = 1, cur_count = 1;
    for (int i = 1; i < n; i++) {
        if (vals[i] == vals[i - 1]) cur_count++;
        else cur_count = 1;
        if (cur_count > best_count) {
            best_count = cur_count;
            mode = vals[i];
        }
    }
    return mode;
}

static double compute_median(long *vals, int n) {
    qsort(vals, n, sizeof(long), cmp_long);
    if (n % 2 == 0) return (vals[n/2 - 1] + vals[n/2]) / 2.0;
    return vals[n/2];
}

void shuffle(uint64_t *arr, uint64_t n) {
    for (uint64_t i = n - 1; i > 0; i--) {
        uint64_t j = rand() % (i + 1);
        uint64_t tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
    }
}

void benchmark_ops(const char *label, FILE *csv, uint64_t ops) {
    long insert_times[REPEATS], search_times[REPEATS], remove_times[REPEATS];
    volatile uint64_t removed_count = 0;
    volatile uint64_t add_count = 0;
    volatile uint64_t search_count = 0;
    uint64_t *keys = malloc(sizeof(uint64_t) * ops);
    for (uint64_t i = 0; i < ops; i++){ 
        keys[i] = i;
    }

    for (int r = 0; r < REPEATS; r++) {
        printf("ops: %lu, repeat: %d\n", ops,r);
        shuffle(keys, ops);
        SkipList_u64 *sl = skipList_u64_create();
        struct timespec start, end;

        // ===== SkipList insert =====
        clock_gettime(CLOCK_MONOTONIC, &start);
        for (uint64_t i = 0; i < ops; i++){
            bool res = skipList_u64_insert(sl, keys[i]);
            add_count++;
        }
        clock_gettime(CLOCK_MONOTONIC, &end);
        insert_times[r] = time_diff_ns(start, end);
        shuffle(keys, ops);
        // ===== SkipList search =====
        clock_gettime(CLOCK_MONOTONIC, &start);
        for (uint64_t i = 0; i < ops; i++){
            bool res = skipList_u64_search(sl, keys[i]);
            search_count++;
        }
        clock_gettime(CLOCK_MONOTONIC, &end);
        search_times[r] = time_diff_ns(start, end);
        shuffle(keys, ops);
        // ===== SkipList remove =====
        clock_gettime(CLOCK_MONOTONIC, &start);
        for (uint64_t i = 0; i < ops; i++){
            skipList_u64_remove(sl, keys[i]);
            removed_count++;
        }
        clock_gettime(CLOCK_MONOTONIC, &end);
        remove_times[r] = time_diff_ns(start, end);

        skipList_u64_destroy(&sl);
    }

    free(keys);

    long min_i = insert_times[0], max_i = insert_times[0];
    long min_s = search_times[0], max_s = search_times[0];
    long min_r = remove_times[0], max_r = remove_times[0];

    for (int i = 1; i < REPEATS; i++) {
        if (insert_times[i] < min_i) min_i = insert_times[i];
        if (insert_times[i] > max_i) max_i = insert_times[i];
        if (search_times[i] < min_s) min_s = search_times[i];
        if (search_times[i] > max_s) max_s = search_times[i];
        if (remove_times[i] < min_r) min_r = remove_times[i];
        if (remove_times[i] > max_r) max_r = remove_times[i];
    }

    double median_i = compute_median(insert_times, REPEATS);
    double median_s = compute_median(search_times, REPEATS);
    double median_r = compute_median(remove_times, REPEATS);

    long mode_i = compute_mode(insert_times, REPEATS);
    long mode_s = compute_mode(search_times, REPEATS);
    long mode_r = compute_mode(remove_times, REPEATS);
    long sum_i = 0, sum_s = 0, sum_r = 0;
    for (int i = 0; i < REPEATS; i++) {
        sum_i += insert_times[i];
        sum_s += search_times[i];
        sum_r += remove_times[i];
    }
    double avg_i = (double)sum_i / REPEATS;
    double avg_s = (double)sum_s / REPEATS;
    double avg_r = (double)sum_r / REPEATS;
    fprintf(csv,
        "%lu,"
        "%ld,%ld,%.2f,%ld,%.2f,"    // insert
        "%ld,%ld,%.2f,%ld,%.2f,"    // search
        "%ld,%ld,%.2f,%ld,%.2f\n",  // remove
        ops,
        min_i, max_i, median_i, mode_i, avg_i,
        min_s, max_s, median_s, mode_s, avg_s,
        min_r, max_r, median_r, mode_r, avg_r
    );
    printf("\n=== %s (%lu ops) ===\n", label, ops);
    printf("Insert: min=%ld ns, max=%ld ns, median=%.2f ns, mode=%ld ns, avg=%.2f ns\n",
        min_i, max_i, median_i, mode_i, avg_i);
    printf("Search: min=%ld ns, max=%ld ns, median=%.2f ns, mode=%ld ns, avg=%.2f ns\n",
        min_s, max_s, median_s, mode_s, avg_s);
    printf("Remove: min=%ld ns, max=%ld ns, median=%.2f ns, mode=%ld ns, avg=%.2f ns\n",
        min_r, max_r, median_r, mode_r, avg_r);
    printf("Insert Count: %lu,Search count: %lu, Remove Count: %lu\n", add_count, search_count, removed_count);
}

int main(void) {
    srand(time(NULL));

    uint64_t test_sizes[] = {100, 500, 1000, 5000, 10000, 50000, 100000,500000,1000000,5000000,10000000};
    size_t n_sizes = sizeof(test_sizes) / sizeof(test_sizes[0]);

    FILE *csv = fopen("benchmark_results.csv", "w");
    if (!csv) {
        perror("fopen");
        return 1;
    }

    fprintf(csv,
        "Ops,"
        "Insert_min,Insert_max,Insert_median,Insert_mode,Insert_avg,"
        "Search_min,Search_max,Search_median,Search_mode,Search_avg,"
        "Remove_min,Remove_max,Remove_median,Remove_mode,Remove_avg\n");

    for (size_t i = 0; i < n_sizes; i++)
        benchmark_ops("SkipList_u64", csv, test_sizes[i]);

    fclose(csv);
    printf("\n✅ Benchmark results saved to benchmark_results.csv\n");
    return 0;
}