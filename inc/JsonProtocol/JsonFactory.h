#ifndef JSONFACTORY_H
#define JSONFACTORY_H

#include <json/json.h>
#include <sstream>

#include "pod.h"
#include <JsonChatProtocol/common_json_protocol.h>

class JsonFactory
{
public:
    JsonFactory();
    ~JsonFactory();

    std::string createLoginSuccessfulJSON(UserPOD user);
    std::string createLoginFailedJSON(Authentification_Status status);

private:
    Json::StreamWriter* p_writer;
    std::stringstream m_outputStream;
};

#endif // JSONFACTORY_H

