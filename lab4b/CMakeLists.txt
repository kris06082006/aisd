cmake_minimum_required(VERSION 3.10)
project(hhh)

set(CMAKE_C_STANDARD 11)

set(SOURCES
    functions.c
    LLRB_tree.c
    dialog.c
    main.c
    cases.c
)

add_executable(hhh ${SOURCES})
target_link_libraries(hhh readline)
