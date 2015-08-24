#include "ChatServer/ChatServer.hpp"

#include "WebsocketServer/WebsocketServer.hpp"

#include "JsonProtocol/ServerJsonParser.hpp"
#include "JsonProtocol/ServerJsonFactory.hpp"

#include "DAO/UserDAO.hpp"

#include <debug_utils/log_debug.hpp>

ChatServer::ChatServer(int port) :
    p_websocketServer(new WebsocketServer(port)),
    p_jsonParser(new ServerJsonParser()),
    p_jsonFactory(new ServerJsonFactory()),
    p_userDAO(new UserDAO())
{
    p_websocketServer->addListener(this);
}

void ChatServer::run()
{
    p_websocketServer->run();
}

void ChatServer::close()
{
    p_websocketServer->close();
}

void ChatServer::onMessageReceived(connection_hdl     hdl,
                                   const std::string& message)
{

	LOG_DEBUG_METHOD;
	if (!p_jsonParser->trySetJsonString(message))
    {
        LOG_DEBUG("Received message: %s is invalid\n", message.c_str());
        return;
    }

    ACTION_TYPE actionType = p_jsonParser->getActionType();
    switch(actionType)
    {
        case ACTION_LOGIN:
        {
			LoginRequestJson requestJson = p_jsonParser->tryGetLoginRequestJson();
			tryLogInUser(requestJson, hdl);
            break;
        }

        case ACTION_GET_CONTACTS:
        {
            handleGetContactsRequest(hdl);
            break;
        }

        case ACTION_SEND_MESSAGE:
        {
			SendMessageJson requestJson = p_jsonParser->tryGetSendMessageJson();
			handleSendMessage(requestJson, hdl);
            break;
        }

		case ACTION_ADD_CONTACT:
		{
			handleAddContact(p_jsonParser->tryGetAddContactJson(), hdl);
			break;
		}

		case ACTION_ADD_CONTACT_RESOLUTION:
		{
			handleAddContactResolution(p_jsonParser->tryGetAddContactResolutionJson(), hdl);
			break;
		}

		case ACTION_REMOVE_CONTACT:
		{
			handleRemoveContact(p_jsonParser->tryGetRemoveContactJson(), hdl);
			break;
		}

		case ACTION_CHANGE_STATE:
		{
			handleChangeState(p_jsonParser->tryGetChangeStateJson(), hdl);
			break;
		}
		case ACTION_REGISTER_USER:
		{
			handleRegister(p_jsonParser->tryGetRegisterUpdateUserJson(), hdl);
			break;
		}
		case ACTION_UPDATE_USER:
		{
			handleUpdate(p_jsonParser->tryGetRegisterUpdateUserJson(), hdl);
			break;
		}

        default:
        {
            break;
        }
    }
}

void ChatServer::handleAddContact(const AddContactJson& requestJson, connection_hdl hdl)
{
	const std::string& acceptorUserName = requestJson.getUserName();
	int initiatorId = getUserId(hdl);
	if (initiatorId != -1)
	{
		BaseUser initiator = p_userDAO->getBaseUser(initiatorId);
		BaseUser acceptor = p_userDAO->getBaseUser(acceptorUserName);
		if (acceptor.getId() == -1)
		{
			std::string responseJson = p_jsonFactory->createAddContactResponseJsonString(acceptorUserName, ADD_INEXISTENT);
			p_websocketServer->sendMessage(hdl, responseJson);
			return;
		}
		if (acceptor.getUserName() == initiator.getUserName())
		{
			std::string responseJson = p_jsonFactory->createAddContactResponseJsonString(acceptorUserName, ADD_YOURSELF);
			p_websocketServer->sendMessage(hdl, responseJson);
			return;
		}
		if(!isUserLoggedIn(acceptor.getId()))
		{
			std::string responseJson = p_jsonFactory->createAddContactResponseJsonString(acceptorUserName, ADD_OFFLINE);
			p_websocketServer->sendMessage(hdl, responseJson);
		}
		else
		{
			std::string responseJson = p_jsonFactory->createAddingByContactJsonString(initiator.getUserName());
			p_websocketServer->sendMessage(getConnection(acceptor.getId()), responseJson);
		}
	}
}

void ChatServer::handleAddContactResolution(const AddContactResolutionJson& requestJson, connection_hdl hdl)
{
	const std::string& initiatorUserName = requestJson.getUserName();
	BaseUser initiator = p_userDAO->getBaseUser(initiatorUserName);
	BaseUser acceptor = p_userDAO->getBaseUser(getUserId(hdl));

	bool accepted = requestJson.hasAccepted();
	if (accepted)
	{
		p_userDAO->addContactRelation(initiator.getId(), acceptor.getId());
		p_userDAO->addContactRelation(acceptor.getId(), initiator.getId());


		std::string responseJson = p_jsonFactory->createAddContactResponseJsonString(acceptor.getUserName(), ADD_ACCEPTED);
		p_websocketServer->sendMessage(getConnection(initiator.getId()), responseJson);

		USER_STATE initiatorState = m_userState[initiator.getId()];
		if (initiatorState == INVISIBLE)
		{
			initiatorState = OFFLINE;
		}
		Contact initiatorContact(initiator, initiatorState);

		USER_STATE acceptorState = m_userState[acceptor.getId()];
		if (acceptorState == INVISIBLE)
		{
			acceptorState = OFFLINE;
		}
		Contact acceptorContact(acceptor, acceptorState);

		std::string initiatorJson = p_jsonFactory->createGetContactsResponseJsonString(std::vector<Contact>{acceptorContact});
		p_websocketServer->sendMessage(getConnection(initiator.getId()), initiatorJson);

		std::string acceptorJson = p_jsonFactory->createGetContactsResponseJsonString(std::vector<Contact>{initiatorContact});
		p_websocketServer->sendMessage(hdl, acceptorJson);

		//add in db
		//send confirmation to requester
		//send contacts list to requester and accepter
	}
	else
	{
		std::string responseJson = p_jsonFactory->createAddContactResponseJsonString(acceptor.getUserName(), ADD_DECLINED);
		p_websocketServer->sendMessage(getConnection(initiator.getId()), responseJson);
	}

}

void ChatServer::handleRemoveContact(const RemoveContactJson& requestJson, connection_hdl hdl)
{
	int contactId = requestJson.getContactId();
	int requesterId = getUserId(hdl);
	if (p_userDAO->isContactRelation(contactId, requesterId))
	{
		p_userDAO->removeContactRelation(contactId, requesterId);
		p_userDAO->removeContactRelation(requesterId, contactId);

		if (isUserLoggedIn(contactId))
		{
			std::string responseJson = p_jsonFactory->createRemovedByContactJsonString(requesterId);
			p_websocketServer->sendMessage(getConnection(contactId), responseJson);
		}
	}


	//get remover's id from hdl
	//remove relation from dao
	//send notification to removed
}

void ChatServer::handleChangeState(const ChangeStateJson& requestJson, connection_hdl hdl)
{
	int userId = getUserId(hdl);
	if (userId != -1)
	{
		USER_STATE newState = requestJson.getNewState();
		USER_STATE oldState = m_userState[userId];
		if (newState != oldState)
		{
			m_userState[userId] = newState;

			if (newState == INVISIBLE)
			{
				newState = OFFLINE;
			}

			notifyContactsStateChanged(userId, newState);
		}
	}
}

void ChatServer::handleRegister(const RegisterUpdateUserJson& requestJson, connection_hdl hdl)
{
	int userId = getUserId(hdl);
	if (userId == -1)
	{
		std::string responseJson;
		User user = requestJson.getUser();
		if (user.getUserName() == "" || user.getPassword() == "" || user.getFirstName() == "" || user.getLastName() == "")
		{
			responseJson = p_jsonFactory->createUpdateRegisterResponseJsonString(USER_INVALID_INPUT);
			p_websocketServer->sendMessage(hdl, responseJson);
			return;
		}
		BaseUser existing=p_userDAO->getBaseUser(user.getUserName());
		if (existing.getId() != -1)
		{
			responseJson = p_jsonFactory->createUpdateRegisterResponseJsonString(USER_EXISTING_USERNAME);
			p_websocketServer->sendMessage(hdl, responseJson);
			return;
		}
		p_userDAO->registerUser(user);
		responseJson = p_jsonFactory->createUpdateRegisterResponseJsonString(USER_OK);
		p_websocketServer->sendMessage(hdl, responseJson);
	}
}

void ChatServer::handleUpdate(const RegisterUpdateUserJson& requestJson, connection_hdl hdl)
{
	int userId = getUserId(hdl);
	if (userId != -1)
	{
		std::string responseJson;
		User updatedUser = requestJson.getUser();
		if (updatedUser.getUserName() == "" || updatedUser.getPassword() == "" || updatedUser.getFirstName() == "" || updatedUser.getLastName() == "")
		{
			responseJson = p_jsonFactory->createUpdateRegisterResponseJsonString(USER_INVALID_INPUT);
			p_websocketServer->sendMessage(hdl, responseJson);
			return;
		}

		BaseUser existing = p_userDAO->getBaseUser(updatedUser.getUserName());
		if (existing.getId() != userId && existing.getId()!=-1)
		{
			responseJson = p_jsonFactory->createUpdateRegisterResponseJsonString(USER_EXISTING_USERNAME);
			p_websocketServer->sendMessage(hdl, responseJson);
			return;
		}
		updatedUser.setId(userId);
		p_userDAO->updateUser(updatedUser);
		responseJson = p_jsonFactory->createUpdateRegisterResponseJsonString(USER_OK);
		p_websocketServer->sendMessage(hdl, responseJson);

		std::string notificationMessage;
		USER_STATE state = m_userState[userId];
		if (state == INVISIBLE)
		{
			state = OFFLINE;
		}
		Contact c(updatedUser,state);
		notificationMessage = p_jsonFactory->createGetContactsResponseJsonString({ c });

		const std::vector<Contact>& contacts = p_userDAO->getContacts(userId);
		for (auto contact : contacts)
		{
			if (isUserLoggedIn(contact.getId()))
			{
				LOG_DEBUG("Notify:%d\n", contact.getId());
				p_websocketServer->sendMessage(getConnection(contact.getId()), notificationMessage);
			}
		}

	}
}

void ChatServer::onDisconnected(connection_hdl hdl)
{
	int userId = getUserId(hdl);
	if (userId != -1)
	{
		if (m_userState[userId] != INVISIBLE)
		{
			notifyContactsStateChanged(getUserId(hdl), OFFLINE);
		}

		auto user = m_loggedClients.right.find(hdl);
		if (user != m_loggedClients.right.end())
		{
			LOG_DEBUG("Disconnected user with id: %d\n", user->second);
			m_loggedClients.right.erase(user);
		}
	}

}

bool ChatServer::isUserLoggedIn(int userId)
{
    return (m_loggedClients.left.find(userId) != m_loggedClients.left.end());
}

void ChatServer::tryLogInUser(const LoginRequestJson& requestJson,
                              connection_hdl          hdl)
{
	LOG_DEBUG_METHOD;
	const UserCredentials& userCredentials = requestJson.getUserCredentials();
	USER_STATE state = requestJson.getState();
	std::string jsonResponse;

	if (state == OFFLINE)
	{
		jsonResponse = p_jsonFactory->createLoginFailedJsonString(
			AUTH_INVALID_STATE);
		p_websocketServer->sendMessage(hdl, jsonResponse);
		return;
	}

    if (!p_userDAO->isValidUser(userCredentials))
    {
        jsonResponse = p_jsonFactory->createLoginFailedJsonString(
            AUTH_INVALID_CREDENTIALS);
        p_websocketServer->sendMessage(hdl,jsonResponse);
        return;
    }

    UserDetails userDetails = p_userDAO->getUserDetails(
        userCredentials.getUserName());


    if (isUserLoggedIn(userDetails.getId()))
    {
        jsonResponse = p_jsonFactory->createLoginFailedJsonString(
            AUTH_ALREADY_LOGGED_IN);
        p_websocketServer->sendMessage(hdl,jsonResponse);
        return;
    }

    logInUser(userDetails,hdl);
	m_userState[userDetails.getId()] = state;
    jsonResponse = p_jsonFactory->createLoginSuccessfulJsonString(userDetails);
    p_websocketServer->sendMessage(hdl,jsonResponse);

	if (state != INVISIBLE)
	{
		notifyContactsStateChanged(userDetails.getId(), state);
	}

}

void ChatServer::logInUser(const UserDetails& userDetails, connection_hdl hdl)
{
    int userId = userDetails.getId();
    m_loggedClients.insert(userConnectionMap::value_type(userId, hdl));
    LOG_DEBUG("Logged in user with id: %d\n", userId);
}

int ChatServer::getUserId(connection_hdl hdl)
{
    auto user = m_loggedClients.right.find(hdl);
    if (user != m_loggedClients.right.end())
    {
        return user->second;
    }
    return -1;
}

websocketpp::connection_hdl ChatServer::getConnection(int userId)
{
    auto con = m_loggedClients.left.find(userId);
    if (con == m_loggedClients.left.end())
    {
        throw std::exception();
    }
    return con->second;
}

void ChatServer::handleGetContactsRequest(connection_hdl hdl)
{
    LOG_DEBUG_METHOD;
    int userId = getUserId(hdl);
	std::vector<Contact>& contacts = p_userDAO->getContacts(userId);

    setContactsOnlineStatus(contacts);
    std::string contactsJsonResponse =
        p_jsonFactory->createGetContactsResponseJsonString(contacts);

    p_websocketServer->sendMessage(hdl, contactsJsonResponse);
}

void ChatServer::handleSendMessage(const SendMessageJson& requestJson, websocketpp::connection_hdl hdl)
{
	Message message = requestJson.getMessage();
    int senderId = getUserId(hdl);
    if (senderId != -1)
    {
        message.setSenderId(senderId);
        std::string receiveMessageJson = p_jsonFactory->createReceiveMessageJsonString(message);
        LOG_DEBUG("R:%s", receiveMessageJson.c_str());
        try
        {
            connection_hdl peerHdl = getConnection(message.getReceiverId());

            p_websocketServer->sendMessage(peerHdl, receiveMessageJson);
        }
        catch(const std::exception&)
        {
            LOG_DEBUG("RECEIVER NOT FOUND\n");
        }
    }
}

void ChatServer::setContactsOnlineStatus(std::vector<Contact>& contacts)
{
    for(Contact& contact: contacts)
    {
        bool isLoggedIn = isUserLoggedIn(contact.getId());
        //LOG_DEBUG("U:%d L:%d\n",contact.getDetails().getId(), isLoggedIn);
		if (isLoggedIn)
		{
			USER_STATE state = m_userState[contact.getId()];
			if (state == INVISIBLE)
			{
				state = OFFLINE;
			}
			contact.setState(state);
		}
    }
}

void ChatServer::notifyContactsStateChanged(int userId, USER_STATE state)
{
	if (userId != -1)
	{
		LOG_DEBUG("U:%d O:%d\n", userId, state);

		std::string notificationMessage;

		notificationMessage = p_jsonFactory->createContactStateChangedJsonString(userId, state);

		const std::vector<Contact>& contacts = p_userDAO->getContacts(userId);
		for (auto contact : contacts)
		{
			if (isUserLoggedIn(contact.getId()))
			{
				LOG_DEBUG("Notify:%d\n", contact.getId());
				p_websocketServer->sendMessage(getConnection(contact.getId()), notificationMessage);
			}
		}
	}

}
