#ifndef IJSONPARSER
#define IJSONPARSER

#include <string>

#include <JsonChatProtocol/common_json_protocol.h>
#include <Model/User.h>

class IJsonParser
{
public:
    virtual ~IJsonParser()
    {
    }

    virtual void parseJsonString(const std::string& json) = 0 ;
    virtual ChatClient_Action_Type getActionType() = 0;
    virtual UserCredentials getUser() = 0;
};

#endif // IJSONPARSER

