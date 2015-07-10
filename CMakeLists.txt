project(chatServerApp)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DASIO_STANDALONE")

MESSAGE(STATUS "TARGET SYSTEM:" ${CMAKE_SYSTEM_NAME})
MESSAGE(STATUS "BUILD TYPE:" ${CMAKE_BUILD_TYPE})

INCLUDE_DIRECTORIES($ENV{WEBSOCKETPP_LIBRARY_PATH})
INCLUDE_DIRECTORIES($ENV{ASIO_LIBRARY_PATH})

SET(SOURCE_FILES src/main.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
