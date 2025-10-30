cmake_minimum_required(VERSION 3.20)
project(skiplist VERSION 1.0.0 LANGUAGES C)

# enable stricter warnings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)



option(BUILD_SHARED_LIBS "Build shared libraries (.so) instead of static (.a)" OFF)
option(BUILD_TESTS "Build skiplist test program" ON)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

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
            VERSION ${PROJECT_VERSION}
            SOVERSION 1
            EXPORT_NAME ${NAME}
    )
    install(TARGETS ${NAME}
            EXPORT skiplistTargets
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
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
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
# Export targets to a file
install(EXPORT skiplistTargets
        FILE skiplistTargets.cmake
        NAMESPACE SkipList::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/skiplist
)
# ============================================================
# Package configuration
# ============================================================
# Generate version file
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/skiplistConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion
)

# Generate config file that includes targets
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/skiplistConfig.cmake" "
include(\"\${CMAKE_CURRENT_LIST_DIR}/skiplistTargets.cmake\")
")

# Install config + version files
install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/skiplistConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/skiplistConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/skiplist
)

# test build option
if(BUILD_TESTS)
    add_subdirectory(test)
endif()