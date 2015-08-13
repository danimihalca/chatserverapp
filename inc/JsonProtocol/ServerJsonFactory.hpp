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
	std::string createContactStateChangedJsonString(int userId, USER_STATE state);

	std::string createRemovedByContactJsonString(int contactId);
	std::string createAddingByContactJsonString(const std::string& userName);
	std::string createAddContactResponseJsonString(const std::string& userName, ADD_STATUS status);

	std::string createUpdateRegisterResponseJsonString(REGISTER_UPDATE_USER_STATUS status);

private:
    Json::StreamWriter* p_writer;
    std::stringstream m_outputStream;

};

#endif // SERVERJSONFACTORY_HPP

