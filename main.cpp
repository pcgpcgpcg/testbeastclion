#include <iostream>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <iostream>
#include <string>
#include <thread>

using tcp=boost::asio::ip::tcp;
namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace http = beast::http;

void do_session(tcp::socket socket){

    try {
        //construct the stream by moving in the socket
        websocket::stream<tcp::socket> ws{std::move(socket)};
        //Set a decorator to change the server of the handshake
        ws.set_option(websocket::stream_base::decorator(
                [](websocket::response_type& res){
                    res.set(http::field::server,std::string(BOOST_BEAST_VERSION_STRING)+" websocket-server-sync");
                }
                ));
        ws.accept();
        for(;;){
            beast::flat_buffer buffer;
            //read a message
            ws.read(buffer);
            //just print the message we receiced
            std::cout<<"we got buffer:"<< boost::beast::buffers_to_string(buffer.data())<<std::endl;
        }
    }
    catch(beast::system_error const& se){
        //this indicates that the session was closed
        if(se.code() != websocket::error::closed){
            std::cerr <<"Error: "<<se.code().message()<<std::endl;
        }
    }
    catch(std::exception const& e){
        std::cerr<<"Error: "<<e.what() <<std::endl;
    }
}

int main() {
    std::cout << "begin beast test!" << std::endl;
    auto const address = boost::asio::ip::make_address("0.0.0.0");
    auto const port = static_cast<unsigned short>(8001);
    //the io_context is required for all I/O
    boost::asio::io_context ioc{1};
    //the acceptor receives incoming connections
    tcp::acceptor acceptor{ioc,{address,port}};
    for(;;){
        //this will receive the new connection
        tcp::socket socket(ioc);
        //Block until we get a connection
        acceptor.accept(socket);
        //Launch the session, transferring ownership of the socket
        std::thread(&do_session,std::move(socket)).detach();
    };
    return 0;
}
