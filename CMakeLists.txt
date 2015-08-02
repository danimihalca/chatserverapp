project(chatServerApp)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if(${CMAKE_BUILD_TYPE} MATCHES Debug)
    add_definitions(-DENABLE_LOG_DEBUG)
endif()

INCLUDE_DIRECTORIES(inc)
INCLUDE_DIRECTORIES(${HEADERS_INSTALL_PATH})
INCLUDE_DIRECTORIES(${CHAT_UTILS_PATH})

IF (WIN32)
        INCLUDE_DIRECTORIES("C:/Program Files (x86)/MySQL/Connector.C++ 1.1/include")
        INCLUDE_DIRECTORIES("D:/thesis/libraries/boost_1_58_0/")
        SET(MYSQL_CPP_CONNECTOR_PATH "C:/Program Files (x86)/MySQL/Connector.C++ 1.1/lib/debug")
        LINK_DIRECTORIES(${MYSQL_CPP_CONNECTOR_PATH})
        LINK_DIRECTORIES("D:/thesis/libraries/boost_1_58_0/lib32-msvc-12.0")
        add_definitions(-D_WEBSOCKETPP_CPP11_CHRONO_)
ELSE()
        SET(LIBRARIES boost_system)
ENDIF()

INCLUDE_DIRECTORIES("${WORKSPACE}/libraries/websocketpp")



SET(HEADER_FILES inc/ChatServer/IChatServer.hpp
                 inc/ChatServer/ChatServer.hpp
                 inc/WebsocketServer/IWebsocketServer.hpp
                 inc/WebsocketServer/IWebsocketServerListener.hpp
                 inc/WebsocketServer/WebsocketServer.hpp
                 inc/JsonProtocol/IServerJsonParser.hpp
                 inc/JsonProtocol/IServerJsonFactory.hpp
                 inc/JsonProtocol/ServerJsonParser.hpp
                 inc/JsonProtocol/ServerJsonFactory.hpp
                 inc/DAO/IUserDAO.hpp
                 inc/DAO/UserDAO.hpp)

SET(SOURCE_FILES src/main.cpp
                 src/ChatServer/ChatServer.cpp
                 src/WebsocketServer/WebsocketServer.cpp
                 src/JsonProtocol/ServerJsonParser.cpp
                 src/JsonProtocol/ServerJsonFactory.cpp
                 src/DAO/UserDAO.cpp)


LINK_DIRECTORIES(${LIBRARY_INSTALL_PATH})

SET (LIBRARIES ${LIBRARIES} jsoncpp mysqlcppconn)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME}  ${LIBRARIES})

IF(WIN32)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different "${LIBRARY_INSTALL_PATH}/jsoncpp.dll" "${CMAKE_BINARY_DIR}/$<CONFIGURATION>"
                COMMAND ${CMAKE_COMMAND} -E copy_if_different "${MYSQL_CPP_CONNECTOR_PATH}/mysqlcppconn.dll" "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
ENDIF()
