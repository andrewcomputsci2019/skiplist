cmake_minimum_required(VERSION 3.20)
project(skiplist LANGUAGES C)

# enable stricter warnings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)



option(BUILD_SHARED_LIBS "Build shared libraries (.so) instead of static (.a)" OFF)

# define include dir
include_directories(${CMAKE_SOURCE_DIR}/include)



# function to handle exporting cmake project libs
function(add_skiplist_variant NAME SRC_FILE)
    add_library(${NAME} ${SRC_FILE})
    target_include_directories(${NAME}
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
    )
    target_compile_definitions(${NAME} PRIVATE SKIPLIST_BUILD_${NAME})
    target_link_libraries(${NAME} PRIVATE m)
    set_target_properties(${NAME} PROPERTIES
        OUTPUT_NAME ${NAME}
        VERSION 1.0.0
        SOVERSION 1
    )
    install(TARGETS ${NAME}
        EXPORT skiplistTargets
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
    )
endfunction()


# ============================================================
# Build variants
# ============================================================
add_skiplist_variant(skiplist_i32 src/skiplist_i32.c)
add_skiplist_variant(skiplist_i64 src/skiplist_i64.c)
add_skiplist_variant(skiplist_u32 src/skiplist_u32.c)
add_skiplist_variant(skiplist_u64 src/skiplist_u64.c)


# === Unified Interface Library ===
add_library(skiplist INTERFACE)
# Export include directory for the interface
target_include_directories(skiplist INTERFACE
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(skiplist INTERFACE
    skiplist_i32
    skiplist_i64
    skiplist_u32
    skiplist_u64
)

# ============================================================
# Install rules
# ============================================================
# Install headers
install(DIRECTORY include/ DESTINATION include)

# Export CMake targets
install(TARGETS skiplist EXPORT skiplistTargets)

# Generate skiplistConfig.cmake for consumers
install(EXPORT skiplistTargets
    FILE skiplistConfig.cmake
    NAMESPACE SkipList::
    DESTINATION lib/cmake/skiplist
)


# todo put exe code below for test and benchmarking etc

# test build option
option(BUILD_TESTS "Build skiplist test program" ON)
if(BUILD_TESTS)
    add_subdirectory(test)
endif()