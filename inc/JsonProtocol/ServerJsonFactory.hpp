#ifndef SERVERJSONFACTORY_HPP
#define SERVERJSONFACTORY_HPP

#include <json/json.h>
#include <sstream>

#include "IServerJsonFactory.hpp"

#include <JsonChatProtocol/common_json_protocol.hpp>

class ServerJsonFactory : public IServerJsonFactory
{
public:
    ServerJsonFactory();
    ~ServerJsonFactory();

    // Implements IServerJsonFactory interface
public:
    std::string createLoginSuccessfulJsonString(const UserDetails& userDetails);
	std::string createLoginFailedJsonString(AUTH_STATUS status);
	std::string createGetContactsResponseJsonString(const std::vector<Contact>& contacts);
    std::string createReceiveMessageJsonString(const Message& message);
	std::string createContactStateChangedJsonString(int userId, CONTACT_STATE state);


private:
    Json::StreamWriter* p_writer;
    std::stringstream m_outputStream;

};

#endif // SERVERJSONFACTORY_HPP

