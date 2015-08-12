#include "JsonProtocol/ServerJsonParser.hpp"

#include <iostream>

#include <cstring>

#include <debug_utils/log_debug.hpp>

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

bool ServerJsonParser::trySetJsonString(const std::string& json)
{
    const char* cString = json.c_str();
    return p_reader->parse(cString,
                           &(cString[strlen(cString)]),&m_root,
                           &m_errors);
}

REQUEST_ACTION_TYPE ServerJsonParser::getActionType()
{
	return static_cast<REQUEST_ACTION_TYPE>(m_root[REQUEST_ACTION].asInt());
}

LoginRequestJson ServerJsonParser::tryGetLoginRequestJson()
{
	Json::Value content = m_root[CONTENT];

	std::string userName = content[USER_CREDENTIALS][USERNAME].asString();
	std::string password = content[USER_CREDENTIALS][PASSWORD].asString();

	UserCredentials userCredentials(userName,password);

	LoginRequestJson requestJson;
	requestJson.setUserCredentials(userCredentials);

	return requestJson;
}

//RequestContactsJson ServerJsonParser::tryGetRequestContactsJson()
//{
//
//}

SendMessageJson ServerJsonParser::tryGetSendMessageJson()
{
	Json::Value content = m_root[CONTENT];

	int receiverId = content[MESSAGE][RECEIVER_ID].asInt();
	std::string messageText = content[MESSAGE][MESSAGE_TEXT].asString();
	
	Message message(messageText, receiverId);

	SendMessageJson requestJson;
	requestJson.setMessage(message);

	return requestJson;
}


AddContactJson  ServerJsonParser::tryGetAddContactJson()
{
	Json::Value content = m_root[CONTENT];

	std::string userName = content[USERNAME].asString();

	AddContactJson requestJson(userName);

	return requestJson;
}
AddContactResolutionJson  ServerJsonParser::tryGetAddContactResolutionJson()
{
	Json::Value content = m_root[CONTENT];

	std::string userName = content[USERNAME].asString();
	bool accepted = content["accepted"].asBool();
	AddContactResolutionJson requestJson(userName, accepted);

	return requestJson;
}
RemoveContactJson  ServerJsonParser::tryGetRemoveContactJson()
{
	Json::Value content = m_root[CONTENT];

	int contactId = content[ID].asInt();

	RemoveContactJson requestJson(contactId);

	return requestJson;
}

//UserCredentials ServerJsonParser::getUserCredentials()
//{
//    std::string username = m_root[USER_CREDENTIALS][USER_USERNAME].asString();
//    std::string password = m_root[USER_CREDENTIALS][USER_PASSWORD].asString();
//
//    return UserCredentials(username,password);
//}
//
//Message ServerJsonParser::getMessage()
//{
//    LOG_DEBUG_METHOD;
//    int receiverID = m_root[MESSAGE][MESSAGE_RECEIVER_ID].asInt();
//    std::string messageText = m_root[MESSAGE][MESSAGE_TEXT].asString();
//
//    return Message(messageText,receiverID);
//}
