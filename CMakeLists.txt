cmake_minimum_required(VERSION 3.31)

project(Scarlet)
project(ScarletClient)

set(CMAKE_CXX_STANDARD 20)

add_executable(
        Scarlet
        src/Server/main.cpp
        src/Server/TcpServer.hpp
        src/Server/Http/HttpRequest.hpp
        src/Server/Http/HttpResponse.hpp
        src/Server/Logging/ServerLog.hpp
        src/Server/Socket.hpp
        src/Server/Scarlet/ScarletResponse.hpp
        src/Server/Scarlet/ScarletType.hpp
        src/Server/Scarlet/ScarletRequestType.hpp
)

add_executable(
        ScarletClient
        src/Client/main.cpp
)
