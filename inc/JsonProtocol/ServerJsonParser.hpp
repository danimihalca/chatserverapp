#ifndef SERVERJSONPARSER_HPP
#define SERVERJSONPARSER_HPP

#include <string>

#include <json/json.h>

#include "IServerJsonParser.hpp"

class ServerJsonParser : public IServerJsonParser
{
public:
    ServerJsonParser();
    ~ServerJsonParser();

    // Implements IServerJsonParser interface
public:
    bool parseJsonString(const std::string& json);
    Chat_Action_Type getActionType();
    UserCredentials getUserCredentials();

private:
    Json::CharReader* p_reader;
    Json::Value m_root;
    std::string m_errors;

};

#endif // SERVERJSONPARSER_HPP

