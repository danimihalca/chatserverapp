#include <iostream>

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

typedef websocketpp::server<websocketpp::config::asio> server;
server print_server;

void on_message(websocketpp::connection_hdl hdl, server::message_ptr msg)
{
    std::cout <<"MESSAGE RECEIVED: " + msg->get_payload() << std::endl;

    server::connection_ptr con = print_server.get_con_from_hdl(hdl);
    con->send(std::string{"This is the reply for: \"" +  msg->get_payload() +'\"'});
}

void on_close(websocketpp::connection_hdl hdl)
{
    server::connection_ptr con = print_server.get_con_from_hdl(hdl);
    std::cout <<"Disconnected: " + con->get_remote_endpoint() <<std::endl;
}

void on_open(websocketpp::connection_hdl hdl)
{
    server::connection_ptr con = print_server.get_con_from_hdl(hdl);
    std::cout <<"Connected: " + con->get_remote_endpoint() <<std::endl;
}

int main()
{

    print_server.set_message_handler(&on_message);
    print_server.set_close_handler(&on_close);
    print_server.set_open_handler(&on_open);
    print_server.init_asio();
    print_server.clear_access_channels(websocketpp::log::alevel::all);
//    print_server.set_access_channels(websocketpp::log::alevel::connect);
//    print_server.set_access_channels(websocketpp::log::alevel::disconnect);
//    print_server.set_access_channels(websocketpp::log::alevel::app);

    print_server.listen(9002);
    print_server.start_accept();

    print_server.run();
}
