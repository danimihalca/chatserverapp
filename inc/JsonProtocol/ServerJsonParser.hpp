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
	bool trySetJsonString(const std::string& json);
    REQUEST_ACTION_TYPE getActionType();
	
	LoginRequestJson tryGetLoginRequestJson();
	//RequestContactsJson tryGetRequestContactsJson();
	SendMessageJson tryGetSendMessageJson();

private:
    Json::CharReader* p_reader;
    Json::Value m_root;
    std::string m_errors;

};

#endif // SERVERJSONPARSER_HPP

