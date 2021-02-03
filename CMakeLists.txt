cmake_minimum_required(VERSION 3.17)
project(testbeastclion)

set(CMAKE_CXX_STANDARD 14)
set(Boost_SYSTEM_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/lib)

add_executable(testbeastclion main.cpp http_session.cpp http_session.h websocket_session.cpp websocket_session.h
        shared_state.cpp shared_state.h listener.cpp listener.h net.h beast.h EnhancedEventEmitter.hpp WebSocketServer.cpp WebSocketServer.h)
target_include_directories(testbeastclion
        PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/utils>)
target_link_directories(testbeastclion
        PUBLIC ${Boost_SYSTEM_LIBRARY})
