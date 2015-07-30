project(chatServerApp)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if(${CMAKE_BUILD_TYPE} MATCHES Debug)
    add_definitions(-DLOG_DEBUG)
endif()

INCLUDE_DIRECTORIES(inc)
INCLUDE_DIRECTORIES(${HEADERS_INSTALL_PATH})
INCLUDE_DIRECTORIES(${CHAT_UTILS_PATH})

IF (WIN32)
        INCLUDE_DIRECTORIES("C:/Program Files (x86)/MySQL/Connector.C++ 1.1/include")
        SET(LIBWEBSOCKETS_LIBRARY websockets_shared)
        SET(MYSQL_CPP_CONNECTOR_PATH "C:/Program Files (x86)/MySQL/Connector.C++ 1.1/lib/debug")
        LINK_DIRECTORIES(${MYSQL_CPP_CONNECTOR_PATH})
        #set_property(DIRECTORY PROPERTY COMPILE_DEFINITIONS LIBRARY_LINK_PATH="${LIBRARY_INSTALL_PATH}")
ELSE()
        SET(LIBWEBSOCKETS_LIBRARY websockets)

ENDIF()

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
target_link_libraries(${PROJECT_NAME} ${LIBWEBSOCKETS_LIBRARY} jsoncpp mysqlcppconn)

IF(WIN32)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different "${LIBRARY_INSTALL_PATH}/${LIBWEBSOCKETS_LIBRARY}.dll" "${CMAKE_BINARY_DIR}/$<CONFIGURATION>"
                COMMAND ${CMAKE_COMMAND} -E copy_if_different "${LIBRARY_INSTALL_PATH}/jsoncpp.dll" "${CMAKE_BINARY_DIR}/$<CONFIGURATION>"
                COMMAND ${CMAKE_COMMAND} -E copy_if_different "${MYSQL_CPP_CONNECTOR_PATH}/mysqlcppconn.dll" "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
ENDIF()