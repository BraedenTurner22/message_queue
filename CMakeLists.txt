cmake_minimum_required(VERSION 3.25)

project(mq
    VERSION 0.1.0
    DESCRIPTION "A Kafka-style distributed message queue in C++20"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---- Options ---------------------------------------------------------------
set(MQ_SANITIZER "" CACHE STRING "Sanitizer to build with: '', 'address' (ASan+UBSan), or 'thread' (TSan)")
set_property(CACHE MQ_SANITIZER PROPERTY STRINGS "" address thread)
option(MQ_CLANG_TIDY "Run clang-tidy on library code (src/) during the build" OFF)
option(MQ_BUILD_TESTS "Build unit tests" ON)

# ---- Shared compile options -----------------------------------------------
# Every mq target links mq::options to pick up warnings and sanitizer flags.
# Third-party code (vcpkg ports) is not affected.
add_library(mq_options INTERFACE)
add_library(mq::options ALIAS mq_options)

target_compile_options(mq_options INTERFACE
    -Wall -Wextra -Wpedantic -Werror
    -Wconversion -Wsign-conversion -Wshadow)

if(MQ_SANITIZER STREQUAL "address")
    set(_mq_san_flags -fsanitize=address,undefined -fno-omit-frame-pointer -fno-sanitize-recover=all)
elseif(MQ_SANITIZER STREQUAL "thread")
    set(_mq_san_flags -fsanitize=thread -fno-omit-frame-pointer)
elseif(NOT MQ_SANITIZER STREQUAL "")
    message(FATAL_ERROR "Unknown MQ_SANITIZER '${MQ_SANITIZER}' (expected '', 'address', or 'thread')")
endif()

if(_mq_san_flags)
    target_compile_options(mq_options INTERFACE ${_mq_san_flags})
    target_link_options(mq_options INTERFACE ${_mq_san_flags})
endif()

# ---- Targets ---------------------------------------------------------------
add_subdirectory(src)

if(MQ_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
