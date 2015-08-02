#ifndef ISERVERJSONFACTORY_HPP
#define ISERVERJSONFACTORY_HPP

#include <string>

#include <JsonChatProtocol/common_json_protocol.hpp>

class UserDetails;

class IServerJsonFactory
{
public:
    virtual std::string createLoginSuccessfulJsonString(
        const UserDetails& userDetails) = 0;
    virtual std::string createLoginFailedJsonString(Authentication_Status status)
        = 0;

    virtual ~IServerJsonFactory()
    {
    }
};

#endif // ISERVERJSONFACTORY_HPP

