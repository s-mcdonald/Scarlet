cmake_minimum_required(VERSION 3.31)

project(Scarlet)

set(CMAKE_CXX_STANDARD 20)

add_executable(
        Scarlet
        src/main.cpp
        src/NetworkInspector.cpp
        src/NetworkInspector.hpp
        src/TcpServer.hpp
        src/HttpRequest.cpp
        src/Logging/ServerLog.hpp
        src/Http/ScarletRequest.hpp
)
