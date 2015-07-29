project(chatServerApp)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


INCLUDE_DIRECTORIES(inc)
INCLUDE_DIRECTORIES(${HEADERS_INSTALL_PATH})
INCLUDE_DIRECTORIES(${COMMON_JSON_CHAT_PROTOCOl_PATH})

SET(HEADER_FILES inc/ChatServer/IChatServer.h
                 inc/ChatServer/ChatServer.h
                 inc/WebsocketServer/IWebsocketServer.h
                 inc/WebsocketServer/IWebsocketServerListener.h
                 inc/WebsocketServer/WebsocketServer.h
                 inc/websocket_callback/websocket_callback.h
                 inc/websocket_callback/IWebsocketCallbackListener.h
                 inc/JsonProtocol/IJsonParser.h
                 inc/JsonProtocol/JsonParser.h
                 inc/JsonProtocol/JsonFactory.h
                 inc/JsonProtocol/pod.h
                 inc/DAO/IUserDAO.h
                 inc/DAO/UserDAO.h)

SET(SOURCE_FILES src/main.cpp
                 src/ChatServer/ChatServer.cpp
                 src/WebsocketServer/WebsocketServer.cpp
                 src/websocket_callback/websocket_callback.cpp
                 src/JsonProtocol/JsonParser.cpp
                 src/JsonProtocol/JsonFactory.cpp
                 src/DAO/UserDAO.cpp)


LINK_DIRECTORIES(${LIBRARY_INSTALL_PATH})


add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} websockets jsoncpp mysqlcppconn)
