cmake_minimum_required(VERSION 3.14)

project(Tests LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(GCC_COMPILE_FLAGS "-Wall -Wextra -Werror")
set(GCC_COVERAGE_COMPILE_FLAGS "-ftest-coverage -fprofile-arcs")
set(GCC_COVERAGE_LINK_FLAGS "-lgcov")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS} ${GCC_COMPILE_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

find_package(Threads REQUIRED)
find_package(GTest REQUIRED)
find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)

add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})

add_executable(Tests
  tests.cpp
  ../Common/Common.h
  ../brick_game/snake/SnakeModel.h ../brick_game/snake/SnakeModel.cpp
  ../brick_game/tetris/TetrisModel.h ../brick_game/tetris/TetrisModel.cpp
  snake_tests.h
  tetris_tests.h
)

target_link_libraries(Tests Qt${QT_VERSION_MAJOR}::Core GTest::GTest Threads::Threads)
target_include_directories(Tests PRIVATE ../Common/ PRIVATE ../brick_game/snake/ PRIVATE ../brick_game/tetris/)
