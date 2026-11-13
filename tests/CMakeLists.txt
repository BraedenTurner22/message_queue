find_package(GTest CONFIG REQUIRED)
include(GoogleTest)

# mq_add_test(<name> SOURCES <files...> LIBS <targets...>)
function(mq_add_test name)
    cmake_parse_arguments(ARG "" "" "SOURCES;LIBS" ${ARGN})
    add_executable(${name} ${ARG_SOURCES})
    target_link_libraries(${name} PRIVATE ${ARG_LIBS} mq::options GTest::gtest_main)
    gtest_discover_tests(${name} DISCOVERY_MODE PRE_TEST)
endfunction()

mq_add_test(common_tests
    SOURCES common/version_test.cpp
    LIBS mq::common)

# Phase 1:
# mq_add_test(storage_tests
#     SOURCES storage/record_batch_test.cpp storage/segment_test.cpp
#     LIBS mq::storage)
