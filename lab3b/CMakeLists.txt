cmake_minimum_required(VERSION 3.10)
project(hhh)

set(CMAKE_C_STANDARD 11)

set(SOURCES
    dialog.c
    functions.c
    main.c
    table.c
    cases.c
)

add_executable(hhh ${SOURCES})
target_link_libraries(hhh readline)
