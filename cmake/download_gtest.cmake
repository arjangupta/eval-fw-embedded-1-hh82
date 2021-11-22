cmake_minimum_required(VERSION 2.8.12)

project(googletest-download NONE)

include(ExternalProject)
ExternalProject_Add(googletest
  GIT_REPOSITORY    https://github.com/google/googletest.git
  GIT_TAG           main
  SOURCE_DIR        "${CMAKE_SOURCE_DIR}/lib/gtest/googletest-src"
  BINARY_DIR        "${CMAKE_SOURCE_DIR}/lib/gtest/googletest-build"
  CONFIGURE_COMMAND ""
  BUILD_COMMAND     ""
  INSTALL_COMMAND   ""
  TEST_COMMAND      ""
)