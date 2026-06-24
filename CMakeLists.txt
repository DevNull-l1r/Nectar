cmake_minimum_required(VERSION 3.16)
project(EXEExecutor VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Multimedia)

qt_standard_project_setup()

add_executable(exe-executor
    main.cpp
    mainwindow.h
    mainwindow.cpp
)

target_link_libraries(exe-executor PRIVATE 
    Qt6::Widgets
    Qt6::Multimedia
    X11
)


