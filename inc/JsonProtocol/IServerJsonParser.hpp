#ifndef ISERVERJSONPARSER_HPP
#define ISERVERJSONPARSER_HPP

#include <string>

#include <JsonChatProtocol/common_json_protocol.hpp>
#include <Model/User.hpp>

class IServerJsonParser
{
public:
    virtual bool parseJsonString(const std::string& json) = 0;
    virtual Chat_Action_Type getActionType() = 0;
    virtual UserCredentials getUserCredentials() = 0;

    virtual ~IServerJsonParser()
    {
    }
};

#endif // ISERVERJSONPARSER_HPP

