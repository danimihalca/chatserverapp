#ifndef WEBSOCKET_CALLBACK_H
#define WEBSOCKET_CALLBACK_H

#include <libwebsockets.h>


const int MAX_PAYLOAD = 1400;

struct session_data
{
    unsigned char buf[LWS_SEND_BUFFER_PRE_PADDING + MAX_PAYLOAD +
                      LWS_SEND_BUFFER_POST_PADDING];
    unsigned int len;

    char ip[25];
};

int websocket_server_callback(struct libwebsocket_context*       ctx,
                              struct libwebsocket*               wsi,
                              enum libwebsocket_callback_reasons reason,
                              void*                              user,
                              void*                              in,
                              size_t                             len);


const struct libwebsocket_protocols default_protocol =
{
    "default",
    websocket_server_callback,
    sizeof(session_data),
    MAX_PAYLOAD
};

const struct libwebsocket_protocols null_protocol =
{
    NULL,
    NULL,
    0
};

#endif // WEBSOCKET_CALLBACK_H

