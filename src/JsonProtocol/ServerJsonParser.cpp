#include "JsonProtocol/ServerJsonParser.hpp"

#include <iostream>

#include <cstring>

ServerJsonParser::ServerJsonParser()
{
    Json::CharReaderBuilder builder;
    builder["collectComments"] = true;
    builder["allowComments"] = true;
    p_reader = builder.newCharReader();
}

ServerJsonParser::~ServerJsonParser()
{
    delete p_reader;
}

bool ServerJsonParser::parseJsonString(const std::string& json)
{
    const char* cString = json.c_str();
    return p_reader->parse(cString,
                           &(cString[strlen(cString)]),&m_root,
                           &m_errors);
}

Chat_Action_Type ServerJsonParser::getActionType()
{
    return static_cast<Chat_Action_Type>(m_root[ACTION].asInt());
}

UserCredentials ServerJsonParser::getUserCredentials()
{
    std::string username = m_root[USER_CREDENTIALS][USER_USERNAME].asString();
    std::string password = m_root[USER_CREDENTIALS][USER_PASSWORD].asString();

    return UserCredentials(username,password);
}
