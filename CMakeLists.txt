project(chatServerApp)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


INCLUDE_DIRECTORIES(${HEADERS_INSTALL_PATH})
LINK_DIRECTORIES(${LIBRARY_INSTALL_PATH})

SET(SOURCE_FILES src/main.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} liblibwebsockets)
