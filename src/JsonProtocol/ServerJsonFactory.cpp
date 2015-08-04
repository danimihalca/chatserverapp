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
    m_outputStream.str("");
    Json::Value root;
    root[ACTION] = LOGIN_RESPONSE;
    root[AUTHENTICATION_STATUS] = AUTH_SUCCESSFUL;

    Json::Value userDetailsJson;
    userDetailsJson[USER_ID] = userDetails.getId();
    userDetailsJson[USER_FULLNAME] = userDetails.getFullName();

    root[USER_DETAILS] = userDetailsJson;
    p_writer->write(root,&m_outputStream);

    return m_outputStream.str();
}

std::string ServerJsonFactory::createLoginFailedJsonString(
    Authentication_Status status)
{
    m_outputStream.str("");
    Json::Value root;
    root[ACTION] = LOGIN_RESPONSE;
    root[AUTHENTICATION_STATUS] = status;
    p_writer->write(root,&m_outputStream);

    return m_outputStream.str();
}

std::string ServerJsonFactory::createGetContactsResponseJsonString(
    const Contacts& contacts)
{
    m_outputStream.str("");
    Json::Value root;
    root[ACTION] = GET_CONTACTS_RESPONSE;

    Json::Value contactsJson;
    Json::Value contactJson;

    for (Contact contact: contacts)
    {
        contactJson.clear();

        contactJson[USER_ID] = contact.getDetails().getId();
        contactJson[USER_USERNAME] = contact.getUserName();
        contactJson[USER_FULLNAME] = contact.getDetails().getFullName();
        contactJson[CONTACT_ONLINE] = contact.isOnline();

        contactsJson.append(contactJson);
    }
    root[CONTACTS] = contactsJson;
    p_writer->write(root,&m_outputStream);

    return m_outputStream.str();
}

std::string ServerJsonFactory::createReceiveMessageJsonString(const Message& message)
{
    LOG_DEBUG_METHOD;
    m_outputStream.str("");
    Json::Value root;
    root[ACTION] = RECEIVE_MESSAGE;

    Json::Value messageJson;

    messageJson[MESSAGE_SENDER_ID] = message.getSenderId();
    messageJson[MESSAGE_TEXT] = message.getMessageText();

    root[MESSAGE] = messageJson;
    p_writer->write(root,&m_outputStream);

    return m_outputStream.str();
}
