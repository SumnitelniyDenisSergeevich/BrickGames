cmake_minimum_required(VERSION 3.5)

project(Snake VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(GCC_COMPILE_FLAGS "-Wall -Wextra -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COMPILE_FLAGS}")
message(${CMAKE_CXX_FLAGS})

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        SnakeModel.h SnakeModel.cpp
        ../../Gui/GraphicModel.h ../../Gui/GraphicModel.cpp
        ../../Common/Common.h
)

add_executable(Snake
    ${PROJECT_SOURCES}
)

target_link_libraries(Snake PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_include_directories(Snake PRIVATE ../../Gui PRIVATE ../../Common)
