cmake_minimum_required(VERSION 3.16)
project(Nectar VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Multimedia)

qt_standard_project_setup()

add_executable(nectar
    main.cpp
    mainwindow.h
    mainwindow.cpp
)

target_link_libraries(exe-executor PRIVATE 
    Qt6::Widgets
    Qt6::Multimedia
    X11
)

install(FILES distribution/io.github.devnull_l1r.nectar.desktop
        DESTINATION share/applications)

install(FILES distribution/io.github.devnull_l1r.nectar.metainfo.xml
        DESTINATION share/metainfo)

install(FILES resources/logo.png
        DESTINATION share/icons/hicolor/256x256/apps
        RENAME io.github.devnull_l1r.nectar.png) 

install(FILES resources/menu_theme.ogg
        DESTINATION share/nectar/themes)
