# clang-tidy is scoped to this directory, so it checks our library code
# but not tests/ (GoogleTest macros are noisy under tidy).
if(MQ_CLANG_TIDY)
    find_program(CLANG_TIDY_EXE NAMES clang-tidy clang-tidy-18 REQUIRED)
    set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY_EXE})
endif()

# mq_common: small shared utilities (CRC32C, errors, byte helpers, ...).
# Header-only for now; switch to a STATIC library once it has .cpp files:
#   add_library(mq_common STATIC common/crc32c.cpp ...)
add_library(mq_common INTERFACE)
add_library(mq::common ALIAS mq_common)
target_include_directories(mq_common INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(mq_common INTERFACE mq::options)

# Phase 1 adds the storage engine here, e.g.:
#   add_library(mq_storage STATIC storage/record_batch.cpp storage/segment.cpp)
#   add_library(mq::storage ALIAS mq_storage)
#   target_include_directories(mq_storage PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
#   target_link_libraries(mq_storage PUBLIC mq::common mq::options)
