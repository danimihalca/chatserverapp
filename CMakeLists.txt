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
        SET(MYSQL_CPP_CONNECTOR_PATH "C:/Program Files (x86)/MySQL/Connector.C++ 1.1/lib/debug")
        LINK_DIRECTORIES(${MYSQL_CPP_CONNECTOR_PATH})
        add_definitions(-DASIO_STANDALONE)
        add_definitions(-D_WEBSOCKETPP_CPP11_THREAD_)
        add_definitions(-D_WEBSOCKETPP_CPP11_FUNCTIONAL_)
        add_definitions(-D_WEBSOCKETPP_CPP11_SYSTEM_ERROR_)
        add_definitions(-D_WEBSOCKETPP_CPP11_RANDOM_DEVICE_)
        add_definitions(-D_WEBSOCKETPP_CPP11_MEMORY_)
        add_definitions(-D_WEBSOCKETPP_CPP11_CHRONO_)
        #add_definitions(-D_WEBSOCKETPP_CPP11_STL_)
        add_definitions(-D_WEBSOCKETPP_CPP11_TYPE_TRAITS_)
        add_definitions(-DASIO_HAS_CSTDINT)
        add_definitions(-DASIO_HAS_STD_TYPE_TRAITS)
        add_definitions(-DASIO_HAS_STD_SHARED_PTR)
        add_definitions(-DASIO_HAS_STD_ADDRESSOF)
        add_definitions(-DASIO_HAS_STD_SHARED_PTR)
        add_definitions(-DASIO_HAS_STD_ARRAY)
        add_definitions(-DASIO_HAS_STD_ATOMIC)
        add_definitions(-DASIO_HAS_THREADS)
ELSE()
        SET(LIBWEBSOCKETS_LIBRARY websockets)
ENDIF()

INCLUDE_DIRECTORIES("D:/thesis/libraries/asio/asio/include")
INCLUDE_DIRECTORIES("D:/thesis/libraries/websocketpp/include")

SET(HEADER_FILES inc/ChatServer/IChatServer.h
                 inc/ChatServer/ChatServer.h
                 inc/WebsocketServer/IWebsocketServer.h
                 inc/WebsocketServer/IWebsocketServerListener.h
                 inc/WebsocketServer/WebsocketServer.h
                 inc/JsonProtocol/IJsonParser.h
                 inc/JsonProtocol/JsonParser.h
                 inc/JsonProtocol/JsonFactory.h
                 inc/JsonProtocol/pod.h
                 inc/DAO/IUserDAO.h
                 inc/DAO/UserDAO.h)

SET(SOURCE_FILES src/main.cpp
                 src/ChatServer/ChatServer.cpp
                 src/WebsocketServer/WebsocketServer.cpp
                 src/JsonProtocol/JsonParser.cpp
                 src/JsonProtocol/JsonFactory.cpp
                 src/DAO/UserDAO.cpp)


LINK_DIRECTORIES(${LIBRARY_INSTALL_PATH})


add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${LIBWEBSOCKETS_LIBRARY} jsoncpp mysqlcppconn)

IF(WIN32)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different "${LIBRARY_INSTALL_PATH}/jsoncpp.dll" "${CMAKE_BINARY_DIR}/$<CONFIGURATION>"
                COMMAND ${CMAKE_COMMAND} -E copy_if_different "${MYSQL_CPP_CONNECTOR_PATH}/mysqlcppconn.dll" "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
ENDIF()