#include "websocket_callback/websocket_callback.h"
#include "websocket_callback/IWebsocketCallbackListener.h"

#include <iostream>

#include <cstdlib>
#include <cstring>

#include <debug_utils\log_debug.h>

int websocket_server_callback(struct libwebsocket_context*       ctx,
                              struct libwebsocket*               wsi,
                              enum libwebsocket_callback_reasons reason,
                              void*                              user,
                              void*                              in,
                              size_t                             len)
{
    IWebsocketCallbackListener* listener =
        static_cast<IWebsocketCallbackListener*>(libwebsocket_context_user(ctx));
    struct session_data* data = static_cast<struct session_data*>(user);

    if (reason != LWS_CALLBACK_GET_THREAD_ID)
    {
        std::cout << reason << std::endl;
    }
    switch (reason)
    {
        case LWS_CALLBACK_ESTABLISHED:
        {
            char name[128];
            int fd = libwebsocket_get_socket_fd(wsi);

            libwebsockets_get_peer_addresses(ctx,
                                             wsi,
                                             fd,
                                             name,
                                             sizeof(name),
                                             data->ip,
                                             sizeof(data->ip));

            listener->onConnected(wsi, data);
            break;
        }

        case LWS_CALLBACK_CLOSED:
        {
            listener->onDisconnected(wsi);
            break;
        }

        case LWS_CALLBACK_RECEIVE:
        {
            listener->onMessageReceived(wsi,(const char*)in);
            break;
        }

        case LWS_CALLBACK_SERVER_WRITEABLE:
        {
            libwebsocket_write(wsi,
                               &(data->buf[LWS_SEND_BUFFER_PRE_PADDING]),
                               data->len,
                               LWS_WRITE_TEXT);
            break;
        }

        default:
        {
            break;
        }
    }


    return 0;
}
