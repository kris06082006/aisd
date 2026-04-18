cmake_minimum_required(VERSION 3.10)
project(graph)

add_compile_options(-g)

set(CMAKE_C_STANDARD 11)

set(SOURCES
    graph.c
    dialog.c
    main.c
    functions.c
    cases.c 
    algorithms.c
)

add_executable(main ${SOURCES})
target_link_libraries(main PUBLIC readline)

