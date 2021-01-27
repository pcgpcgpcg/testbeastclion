cmake_minimum_required(VERSION 3.17)
project(testbeastclion)

set(CMAKE_CXX_STANDARD 14)
set(Boost_SYSTEM_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/lib)

add_executable(testbeastclion main.cpp)
target_include_directories(testbeastclion
        PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>)
target_link_directories(testbeastclion
        PUBLIC ${Boost_SYSTEM_LIBRARY})
