cmake_minimum_required(VERSION 3.14)
project(googletest_samples)

# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 11)


include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)


enable_testing()

add_library(bouncer include/bouncer.h src/bouncer.cpp)

add_executable(
  test_bouncer
  test/test_bouncer.cpp
)

target_link_libraries(
  test_bouncer
  gtest_main
  bouncer
)

gtest_discover_tests(test_bouncer)


