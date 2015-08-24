#include "JsonProtocol/ServerJsonFactory.hpp"

#include <Model/Message.hpp>

#include <debug_utils/log_debug.hpp>

ServerJsonFactory::ServerJsonFactory()
{
    Json::StreamWriterBuilder builder;
    builder["commentStyle"] = "None";
    builder["indentation"] = "";
    builder["enableYAMLCompatibility"] = false;
    builder["dropNullPlaceholders"] = true;

    p_writer = builder.newStreamWriter();
}

ServerJsonFactory::~ServerJsonFactory()
{
    delete p_writer;
}

std::string ServerJsonFactory::createLoginSuccessfulJsonString(
    const UserDetails& userDetails)
{
	LOG_DEBUG_METHOD;
	m_outputStream.str("");
    Json::Value root;
    root[NOTIFICATION] = NOTIFICATION_LOGIN;

	Json::Value content;

	content[AUTHENTICATION_STATUS] = AUTH_SUCCESSFUL;

    Json::Value userDetailsJson;
    userDetailsJson[ID] = userDetails.getId();
    userDetailsJson[FIRSTNAME] = userDetails.getFirstName();
	userDetailsJson[LASTNAME] = userDetails.getLastName();

	content[USER_DETAILS] = userDetailsJson;

	root[CONTENT] = content;

    p_writer->write(root,&m_outputStream);

    return m_outputStream.str();
}

std::string ServerJsonFactory::createLoginFailedJsonString(
    AUTH_STATUS status)
{
    m_outputStream.str("");
    Json::Value root;
	root[NOTIFICATION] = NOTIFICATION_LOGIN;
	Json::Value content;
	
	content[AUTHENTICATION_STATUS] = status;
	root[CONTENT] = content;
	
	p_writer->write(root, &m_outputStream);

    return m_outputStream.str();
}

std::string ServerJsonFactory::createGetContactsResponseJsonString(
	const std::vector<Contact>& contacts)
{
    m_outputStream.str("");
    Json::Value root;
	root[NOTIFICATION] = NOTIFICATION_GET_CONTACTS;

	Json::Value content;
    Json::Value contactsJson;
    Json::Value contactJson;

    for (Contact contact: contacts)
    {
        contactJson.clear();

        contactJson[ID] = contact.getId();
        contactJson[USERNAME] = contact.getUserName();
        contactJson[FIRSTNAME] = contact.getFirstName();
		contactJson[LASTNAME] = contact.getLastName();
		contactJson[STATE] = contact.getState();

        contactsJson.append(contactJson);
    }
	content[CONTACTS] = contactsJson;
	root[CONTENT] = content;
	p_writer->write(root, &m_outputStream);

    return m_outputStream.str();
}

std::string ServerJsonFactory::createReceiveMessageJsonString(const Message& message)
{
    LOG_DEBUG_METHOD;
    m_outputStream.str("");
    Json::Value root;
	Json::Value content;
	root[NOTIFICATION] = NOTIFICATION_SEND_MESSAGE;

    Json::Value messageJson;

    messageJson[SENDER_ID] = message.getSenderId();
    messageJson[MESSAGE_TEXT] = message.getMessageText();

	content[MESSAGE] = messageJson;
	root[CONTENT] = content;
	p_writer->write(root, &m_outputStream);

    return m_outputStream.str();
}


std::string ServerJsonFactory::createContactStateChangedJsonString(int userId, USER_STATE state)
{
    LOG_DEBUG_METHOD;
    m_outputStream.str("");

    Json::Value root;
	Json::Value content;
	Json::Value contact;
	root[NOTIFICATION] = NOTIFICATION_CONTACT_STATE_CHANGED;

	contact[ID] = userId;
	contact[STATE] = state;

	content[CONTACT] = contact;
	root[CONTENT] = content;

    p_writer->write(root,&m_outputStream);
    return m_outputStream.str();
}


std::string ServerJsonFactory::createRemovedByContactJsonString(int contactId)
{
	m_outputStream.str("");
	Json::Value root;
	root[NOTIFICATION] = NOTIFICATION_REMOVE_CONTACT;
	Json::Value content;

	content[ID] = contactId;
	root[CONTENT] = content;
	p_writer->write(root, &m_outputStream);

	return m_outputStream.str();
}

std::string ServerJsonFactory::createAddingByContactJsonString(const std::string& userName)
{
	m_outputStream.str("");
	Json::Value root;
	root[NOTIFICATION] = NOTIFICATION_ADD_REQUEST;
	Json::Value content;

	content[USERNAME] = userName;
	root[CONTENT] = content;
	p_writer->write(root, &m_outputStream);

	return m_outputStream.str();
}

std::string ServerJsonFactory::createAddContactResponseJsonString(const std::string& userName, ADD_STATUS status)
{
	m_outputStream.str("");
	Json::Value root;
	root[NOTIFICATION] = NOTIFICATION_ADD_CONTACT_RESPONSE;
	Json::Value content;

	content[USERNAME] = userName;
	content[ADD_REQUEST_STATUS] = status;
	root[CONTENT] = content;
	p_writer->write(root, &m_outputStream);

	return m_outputStream.str();
}

std::string ServerJsonFactory::createUpdateRegisterResponseJsonString(REGISTER_UPDATE_USER_STATUS status)
{
	m_outputStream.str("");
	Json::Value root;
	root[NOTIFICATION] = NOTIFICATION_REGISTER_UPDATE_USER;
	Json::Value content;

	content[REGISTER_UPDATE_STATUS] = status;
	root[CONTENT] = content;
	p_writer->write(root, &m_outputStream);

	return m_outputStream.str();
}
