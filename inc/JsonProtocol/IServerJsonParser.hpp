#ifndef ISERVERJSONPARSER_HPP
#define ISERVERJSONPARSER_HPP

#include <string>

#include <JsonChatProtocol/common_json_protocol.hpp>
#include <Model/User.hpp>
#include <Model/Message.hpp>

#include <JsonChatProtocol/json_request/LoginRequestJson.hpp>
#include <JsonChatProtocol/json_request/RequestContactsJson.hpp>
#include <JsonChatProtocol/json_request/SendMessageJson.hpp>

class IServerJsonParser
{
public:
	virtual bool trySetJsonString(const std::string& json) = 0;
    virtual REQUEST_ACTION_TYPE getActionType() = 0;
	
	virtual LoginRequestJson tryGetLoginRequestJson() = 0;
	//virtual RequestContactsJson tryGetRequestContactsJson() = 0;
	virtual SendMessageJson tryGetSendMessageJson() = 0;
    virtual ~IServerJsonParser()
    {
    }
};

#endif // ISERVERJSONPARSER_HPP

