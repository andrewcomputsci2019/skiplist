cmake_minimum_required(VERSION 3.20)
project(skiplist_tests LANGUAGES C)


function(add_skiplist_test NAME SRC_FILE)

add_executable(${NAME} ${SRC_FILE})
target_link_libraries(${NAME} PRIVATE skiplist m)
    
endfunction()


add_skiplist_test(test_insert test_insert.c)
add_skiplist_test(test_isEmpty test_isEmpty.c)
add_skiplist_test(test_link_i32 test_link_i32.c)
add_skiplist_test(test_link_u32 test_link_u32.c)
add_skiplist_test(test_link_i64 test_link_i64.c)
add_skiplist_test(test_link_u64 test_link_u64.c)
add_skiplist_test(test_remove test_remove.c)
add_skiplist_test(test_search test_search.c)
add_skiplist_test(test_size test_size.c)
add_skiplist_test(generic_map_test generic_map_test.c)
add_skiplist_test(generic_map_complex_test generic_map_complex_test.c)
add_skiplist_test(generic_list_test generic_list_test.c)
add_skiplist_test(generic_list_complex_test generic_list_complex_test.c)

add_executable(bench_mark main.c)
target_link_libraries(bench_mark PRIVATE skiplist m)
add_executable(bestcase_bench_mark best_case_benchmark.c)
target_link_libraries(bestcase_bench_mark PRIVATE skiplist m)