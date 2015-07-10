#include <iostream>

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <set>

typedef websocketpp::server<websocketpp::config::asio> broadcastServer;
broadcastServer print_server;
std::set<websocketpp::connection_hdl, std::owner_less<websocketpp::connection_hdl>> connections;

void on_message(websocketpp::connection_hdl hdl, broadcastServer::message_ptr msg)
{
    std::cout <<"MESSAGE RECEIVED: " + msg->get_payload() << std::endl;
    for(auto connection: connections)
    {
        print_server.send(connection,msg);
    }
}

void on_close(websocketpp::connection_hdl hdl)
{
    connections.erase(hdl);
    broadcastServer::connection_ptr con = print_server.get_con_from_hdl(hdl);
    std::cout <<"Disconnected: " + con->get_remote_endpoint() <<std::endl;
}

void on_open(websocketpp::connection_hdl hdl)
{
    connections.insert(hdl);
    broadcastServer::connection_ptr con = print_server.get_con_from_hdl(hdl);
    std::cout <<"Connected: " + con->get_remote_endpoint() <<std::endl;
    std::cout <<"Origin:" + con->get_host()<<std::endl;
}


int main()
{
    print_server.set_message_handler(&on_message);
    print_server.set_close_handler(&on_close);
    print_server.set_open_handler(&on_open);
    print_server.init_asio();
    print_server.set_access_channels(websocketpp::log::alevel::all);
    print_server.set_error_channels(websocketpp::log::elevel::all);

    print_server.listen(9002);
    print_server.start_accept();

    print_server.run();
}
