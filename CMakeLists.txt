cmake_minimum_required(VERSION 3.17)
project(testbeastclion)

set(CMAKE_CXX_STANDARD 14)
set(Boost_SYSTEM_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/lib)

add_executable(testbeastclion
        main.cpp
        Transport/HttpTransport.cpp
        Transport/HttpTransport.h
        Transport/WebSocketTransport.cpp
        Transport/WebSocketTransport.h
        Transport/WebSocketServer.cpp
        Transport/WebSocketServer.h
        Room.cpp
        Room.h
        Peer.cpp
        Peer.h
        Message.cpp
        Message.h
        utils/EventEmitter.hpp
        utils/utils.cpp
        utils/utils.h)
target_include_directories(testbeastclion
        PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
        PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/utils>)
target_link_directories(testbeastclion
        PUBLIC ${Boost_SYSTEM_LIBRARY})
