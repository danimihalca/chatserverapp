#ifndef ISERVERJSONFACTORY_HPP
#define ISERVERJSONFACTORY_HPP

#include <string>
#include <vector>

#include <JsonChatProtocol/common_json_protocol.hpp>

#include <Model/User.hpp>

class Message;

class IServerJsonFactory
{
public:
    virtual std::string createLoginSuccessfulJsonString(const UserDetails& userDetails) = 0;
	virtual std::string createLoginFailedJsonString(AUTH_STATUS status) = 0;
	virtual std::string createGetContactsResponseJsonString(const std::vector<Contact>& contacts) = 0;
    virtual std::string createReceiveMessageJsonString(const Message& message) = 0;
    virtual std::string createContactStateChangedJsonString(int userId, CONTACT_STATE state) = 0;

    virtual ~IServerJsonFactory()
    {
    }
};

#endif // ISERVERJSONFACTORY_HPP

