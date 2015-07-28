project(chatServerApp)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


INCLUDE_DIRECTORIES(${HEADERS_INSTALL_PATH})
INCLUDE_DIRECTORIES(inc)


SET(HEADER_FILES inc/ChatServer/IChatServer.h
                 inc/ChatServer/ChatServer.h
                 inc/WebsocketServer/IWebsocketServer.h
                 inc/WebsocketServer/WebsocketServer.h
                 inc/websocket_callback/websocket_callback.h
                 inc/websocket_callback/IWebsocketCallbackListener.h)

SET(SOURCE_FILES src/main.cpp
                 src/ChatServer/ChatServer.cpp
                 src/WebsocketServer/WebsocketServer.cpp
                 src/websocket_callback/websocket_callback.cpp)


LINK_DIRECTORIES(${LIBRARY_INSTALL_PATH})


add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} websockets)
