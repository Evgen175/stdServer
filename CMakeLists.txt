cmake_minimum_required(VERSION 3.2)

project(stdServer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

add_executable(stdServer
    main.cpp
    my_server.h
    )
target_link_libraries(stdServer Threads::Threads)
