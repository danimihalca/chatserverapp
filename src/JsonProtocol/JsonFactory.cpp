#include "JsonProtocol/JsonFactory.h"

#include <common_protocol.h>

JsonFactory::JsonFactory()
{
    Json::StreamWriterBuilder builder;
    builder["commentStyle"] = "None";
    builder["indentation"] = "";
    builder["enableYAMLCompatibility"] = false;
    builder["dropNullPlaceholders"] = true;

    p_writer = builder.newStreamWriter();
}

JsonFactory::~JsonFactory()
{
    delete p_writer;
}

std::string JsonFactory::createLoginSuccessfulJSON(UserPOD user)
{
    m_outputStream.str("");
    Json::Value root;
    root[ACTION] =CS_LOGIN_RESPONSE;
    root[AUTHENTIFICATION_STATUS] = AUTH_OK;
    root[FULLNAME] = user.fullname;
    p_writer->write(root,&m_outputStream);

    return m_outputStream.str();
}

std::string JsonFactory::createLoginFailedJSON(Authentification_Status status)
{
    m_outputStream.str("");
    Json::Value root;
    root[ACTION] =CS_LOGIN_RESPONSE;
    root[AUTHENTIFICATION_STATUS] = status;
    p_writer->write(root,&m_outputStream);

    return m_outputStream.str();
}

