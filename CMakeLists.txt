cmake_minimum_required(VERSION 3.31)

project(Scarlet)

set(CMAKE_CXX_STANDARD 20)

add_executable(
        Scarlet
        src/main.cpp
        src/TcpServer.hpp
        src/Http/HttpRequest.hpp
        src/Http/HttpResponse.hpp
        src/Logging/ServerLog.hpp
        src/Http/ScarletRequestType.hpp
        src/Socket.hpp
        src/Scarlet/ScarletResponse.hpp
)
