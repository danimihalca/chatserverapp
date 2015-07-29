#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <string>

#include <json/json.h>

#include "IJsonParser.h"

class JsonParser: public IJsonParser
{
public:
    JsonParser();
    ~JsonParser();

    //Implements IJsonParser
public:
    void parseJsonString(const std::string& json);
    ChatClient_Action_Type getActionType();
    UserPOD getUser();

private:
    Json::CharReader* p_reader;
    Json::Value m_root;
    std::string m_errors;

};

#endif // JSONPARSER_H

