cmake_minimum_required(VERSION 3.9)

project(NiceLikeBackend)

set(CMAKE_CXX_STANDARD 17)

set(CURSES_NEED_NCURSES TRUE)
find_package(Curses REQUIRED)

find_library(CURSES_PANEL_LIBRARY NAMES panel DOC "The curses panel library")
if (NOT CURSES_PANEL_LIBRARY)
    message(FATAL_ERROR "Curses' panel library not found.")
endif ()

add_library(BackendNice STATIC
        src/BackendNice
        src/Window.hpp
        src/InputReader.hpp
        src/MenuWindow.hpp
        src/Commands.hpp
        src/Menu.hpp
        )

target_include_directories(BackendNice PUBLIC common-interface)
target_include_directories(BackendNice PRIVATE ${CURSES_INCLUDE_DIRS} /usr/include/ncurses)
target_link_libraries(BackendNice ${CURSES_LIBRARIES} ${CURSES_PANEL_LIBRARY})

include_directories(common-interface src)

add_executable(example1 examples/example1/main.cpp)
target_link_libraries(example1 BackendNice)