#include <stdio.h>
#include <stdlib.h>
#include <libwebsockets.h>
#include <string.h>

static int callback_http(struct libwebsocket_context*       ctx,
                         struct libwebsocket*               wsi,
                         enum libwebsocket_callback_reasons reason,
                         void*                              user,
                         void*                              in,
                         size_t                             len)
{
    return 0;
}


static int callback_dumb_increment(struct libwebsocket_context* ctx,
                                   struct libwebsocket* wsi,
                                   enum libwebsocket_callback_reasons reason,
                                   void* user, void* in, size_t len)
{

    switch (reason)
    {
    case LWS_CALLBACK_ESTABLISHED:     // just log message that someone is connecting
    {
        char name[20];
        char rip[20];
        int fd = libwebsocket_get_socket_fd(wsi);
        libwebsockets_get_peer_addresses(ctx,wsi,fd,name,20,rip,20);
        printf("connection established: %s\n",rip);
        break;
    }

    case LWS_CALLBACK_CLOSED:
    {
            char name[20];
            char rip[20];
            int fd = libwebsocket_get_socket_fd(wsi);
            libwebsockets_get_peer_addresses(ctx,wsi,fd,name,20,rip,20);
            printf("Disconnected: %s\n",rip);
            break;
    }

    case LWS_CALLBACK_RECEIVE:
    {                                // the funny part
        // create a buffer to hold our response
        // it has to have some pre and post padding. You don't need to care
        // what comes there, libwebsockets will do everything for you. For more info see
        // http://git.warmcat.com/cgi-bin/cgit/libwebsockets/tree/lib/libwebsockets.h#n597
        unsigned char* buf = (unsigned char*) malloc(LWS_SEND_BUFFER_PRE_PADDING + len +
                                                     LWS_SEND_BUFFER_POST_PADDING);

        int i;

        // pointer to `void *in` holds the incomming request
        // we're just going to put it in reverse order and put it in `buf` with
        // correct offset. `len` holds length of the request.
        for (i = 0; i < len; i++)
        {
            buf[LWS_SEND_BUFFER_PRE_PADDING + (len - 1) -
                i ] = ((char*) in)[i];
        }

        // log what we recieved and what we're going to send as a response.
        // that disco syntax `%.*s` is used to print just a part of our buffer
        // http://stackoverflow.com/questions/5189071/print-part-of-char-array
        printf("received data: %s, replying: %.*s\n", (char*) in, (int) len,
               buf + LWS_SEND_BUFFER_PRE_PADDING);

        // send response
        // just notice that we have to tell where exactly our response starts. That's
        // why there's `buf[LWS_SEND_BUFFER_PRE_PADDING]` and how long it is.
        // we know that our response has the same length as request because
        // it's the same message in reverse order.
        libwebsocket_write(wsi,
                           &buf[LWS_SEND_BUFFER_PRE_PADDING],
                           len,
                           LWS_WRITE_TEXT);

        // release memory back into the wild
        free(buf);
        break;
    }

    default:
        break;
    }


    return 0;
}


static struct libwebsocket_protocols protocols[] =
{
    {
        "default", // protocol name - very important!
        callback_dumb_increment,   // callback
        0                          // we don't use any per session data

    },
    {
        NULL, NULL, 0   /* End of list */
    }
};

int main(void)
{
    // server url will be http://localhost:9000
    int port = 9003;
    const char* interface = NULL;
    struct libwebsocket_context* context;
    // we're not using ssl
    const char* cert_path = NULL;
    const char* key_path = NULL;
    // no special options
    int opts = 0;

    lws_context_creation_info m_info;
    memset(&m_info, 0, sizeof m_info);

    m_info.port = 9003;
    m_info.iface = NULL;
    m_info.protocols = protocols;
//#ifndef LWS_NO_EXTENSIONS
//    m_info.extensions = libwebsocket_get_internal_extensions();
//#endif
//    if (use_ssl) {
//        //        info.ssl_cert_filepath = LOCAL_RESOURCE_PATH"/libwebsockets-test-server.pem";
//        //        info.ssl_private_key_filepath = LOCAL_RESOURCE_PATH"/libwebsockets-test-server.key.pem";
//    }
    m_info.gid = -1;
    m_info.uid = -1;
    m_info.options = 0;

    // create libwebsocket context representing this server
    context = libwebsocket_create_context(&m_info);

    if (context == NULL)
    {
        fprintf(stderr, "libwebsocket init failed\n");
        return -1;
    }

    printf("starting server...\n");

    // infinite loop, to end this server send SIGTERM. (CTRL+C)
    while (1)
    {
        libwebsocket_service(context, 50);
        // libwebsocket_service will process all waiting events with their
        // callback functions and then wait 50 ms.
        // (this is a single threaded webserver and this will keep our server
        // from generating load while there are not requests to process)
    }

    libwebsocket_context_destroy(context);

    return 0;
}
