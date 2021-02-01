//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/vinniefalco/CppCon2018
//

//------------------------------------------------------------------------------
/*
    WebSocket chat server, multi-threaded

    This implements a multi-user chat room using WebSocket. The
    `io_context` runs on any number of threads, specified at
    the command line.

*/
//------------------------------------------------------------------------------

#include "listener.h"
#include "shared_state.h"

#include <boost/asio/signal_set.hpp>
#include <boost/smart_ptr.hpp>
#include <iostream>
#include <vector>

int
main(int argc, char* argv[])
{
    // Check command line arguments.
    if (argc != 5)
    {
        std::cerr <<
                  "Usage: websocket-chat-multi <address> <port> <doc_root> <threads>\n" <<
                  "Example:\n" <<
                  "    websocket-chat-server 0.0.0.0 8080 . 5\n";
        return EXIT_FAILURE;
    }
    auto address = net::ip::make_address(argv[1]);
    auto port = static_cast<unsigned short>(std::atoi(argv[2]));
    auto doc_root = argv[3];
    auto const threads = std::max<int>(1, std::atoi(argv[4]));

    // The io_context is required for all I/O
    net::io_context ioc;

    // Create and launch a listening port
    boost::make_shared<listener>(
            ioc,
            tcp::endpoint{address, port},
            boost::make_shared<shared_state>(doc_root))->run();

    // Capture SIGINT and SIGTERM to perform a clean shutdown
    net::signal_set signals(ioc, SIGINT, SIGTERM);
    signals.async_wait(
            [&ioc](boost::system::error_code const&, int)
            {
                // Stop the io_context. This will cause run()
                // to return immediately, eventually destroying the
                // io_context and any remaining handlers in it.
                ioc.stop();
            });

    // Run the I/O service on the requested number of threads
    std::vector<std::thread> v;
    v.reserve(threads - 1);
    for(auto i = threads - 1; i > 0; --i)
        v.emplace_back(
                [&ioc]
                {
                    ioc.run();
                });
    ioc.run();

    // (If we get here, it means we got a SIGINT or SIGTERM)

    // Block until all the threads exit
    for(auto& t : v)
        t.join();

    return EXIT_SUCCESS;
}







//#include <iostream>
//#include <boost/beast/core.hpp>
//#include <boost/beast/websocket.hpp>
//#include <boost/asio/ip/tcp.hpp>
//#include <boost/asio/dispatch.hpp>
//#include <iostream>
//#include <string>
//#include <thread>
//
//using tcp=boost::asio::ip::tcp;
//namespace beast = boost::beast;
//namespace websocket = beast::websocket;
//namespace http = beast::http;
//namespace net = boost::asio;
//
//void do_session(tcp::socket socket){
//
//    try {
//        //construct the stream by moving in the socket
//        websocket::stream<tcp::socket> ws{std::move(socket)};
//        //Set a decorator to change the server of the handshake
//        ws.set_option(websocket::stream_base::decorator(
//                [](websocket::response_type& res){
//                    res.set(http::field::server,std::string(BOOST_BEAST_VERSION_STRING)+" websocket-server-sync");
//                }
//                ));
//        ws.accept();
//        for(;;){
//            beast::flat_buffer buffer;
//            //read a message
//            ws.read(buffer);
//            //just print the message we receiced
//            std::cout<<"we got buffer:"<< boost::beast::buffers_to_string(buffer.data())<<std::endl;
//        }
//    }
//    catch(beast::system_error const& se){
//        //this indicates that the session was closed
//        if(se.code() != websocket::error::closed){
//            std::cerr <<"Error: "<<se.code().message()<<std::endl;
//        }
//    }
//    catch(std::exception const& e){
//        std::cerr<<"Error: "<<e.what() <<std::endl;
//    }
//}
//
//void main_sync() {
//    std::cout << "begin beast test!" << std::endl;
//    auto const address = boost::asio::ip::make_address("0.0.0.0");
//    auto const port = static_cast<unsigned short>(8001);
//    //the io_context is required for all I/O
//    boost::asio::io_context ioc{1};
//    //the acceptor receives incoming connections
//    tcp::acceptor acceptor{ioc,{address,port}};
//    for(;;){
//        //this will receive the new connection
//        tcp::socket socket(ioc);
//        //Block until we get a connection
//        acceptor.accept(socket);
//        //Launch the session, transferring ownership of the socket
//        std::thread(&do_session,std::move(socket)).detach();
//    };
//}
//const int threads = 5;
//
//// Report a failure
//void
//fail(beast::error_code ec, char const* what)
//{
//    std::cerr << what << ": " << ec.message() << "\n";
//}
//
//// Echoes back all received WebSocket messages
//class session : public std::enable_shared_from_this<session>
//{
//    websocket::stream<beast::tcp_stream> ws_;
//    beast::flat_buffer buffer_;
//
//public:
//    // Take ownership of the socket
//    explicit
//    session(tcp::socket&& socket)
//            : ws_(std::move(socket))
//    {
//    }
//
//    // Get on the correct executor
//    void
//    run()
//    {
//        // We need to be executing within a strand to perform async operations
//        // on the I/O objects in this session. Although not strictly necessary
//        // for single-threaded contexts, this example code is written to be
//        // thread-safe by default.
//        net::dispatch(ws_.get_executor(),
//                      beast::bind_front_handler(
//                              &session::on_run,
//                              shared_from_this()));
//    }
//
//    // Start the asynchronous operation
//    void
//    on_run()
//    {
//        // Set suggested timeout settings for the websocket
//        ws_.set_option(
//                websocket::stream_base::timeout::suggested(
//                        beast::role_type::server));
//
//        // Set a decorator to change the Server of the handshake
//        ws_.set_option(websocket::stream_base::decorator(
//                [](websocket::response_type& res)
//                {
//                    res.set(http::field::server,
//                            std::string(BOOST_BEAST_VERSION_STRING) +
//                            " websocket-server-async");
//                }));
//        // Accept the websocket handshake
//        ws_.async_accept(
//                beast::bind_front_handler(
//                        &session::on_accept,
//                        shared_from_this()));
//    }
//
//    void
//    on_accept(beast::error_code ec)
//    {
//        if(ec)
//            return fail(ec, "accept");
//
//        // Read a message
//        do_read();
//    }
//
//    void
//    do_read()
//    {
//        // Read a message into our buffer
//        ws_.async_read(
//                buffer_,
//                beast::bind_front_handler(
//                        &session::on_read,
//                        shared_from_this()));
//    }
//
//    void
//    on_read(
//            beast::error_code ec,
//            std::size_t bytes_transferred)
//    {
//        boost::ignore_unused(bytes_transferred);
//
//        // This indicates that the session was closed
//        if(ec == websocket::error::closed)
//            return;
//
//        if(ec)
//            fail(ec, "read");
//
//        // Echo the message
//        ws_.text(ws_.got_text());
//        ws_.async_write(
//                buffer_.data(),
//                beast::bind_front_handler(
//                        &session::on_write,
//                        shared_from_this()));
//    }
//
//    void
//    on_write(
//            beast::error_code ec,
//            std::size_t bytes_transferred)
//    {
//        boost::ignore_unused(bytes_transferred);
//
//        if(ec)
//            return fail(ec, "write");
//
//        // Clear the buffer
//        buffer_.consume(buffer_.size());
//
//        // Do another read
//        do_read();
//    }
//};
//
//
////accepts incoming connnections and lauches the sessions
//class listener:public std::enable_shared_from_this<listener>
//{
//    boost::asio::io_context &ioc_;
//    tcp::acceptor acceptor_;
//
//public:
//    listener(boost::asio::io_context& ioc, tcp::endpoint endpoint):ioc_(ioc),acceptor_(ioc) {
//        beast::error_code ec;
//        //open the acceptor
//        acceptor_.open(endpoint.protocol(), ec);
//        if (ec) {
//            fail(ec, "open");
//            return;
//        }
//        //allow address reuse
//        acceptor_.set_option(boost::asio::socket_base::reuse_address(true), ec);
//        if (ec) {
//            fail(ec, "set_option");
//            return;
//        }
//        //Bind to the server address
//        acceptor_.bind(endpoint, ec);
//        if (ec) {
//            fail(ec, "bind");
//            return;
//        }
//        //Start listening for connection
//        acceptor_.listen(boost::asio::socket_base::max_listen_connections, ec);
//        if (ec) {
//            fail(ec, "listen");
//            return;
//        }
//    }
//
//        //Start accepting incoming connections
//        void run()
//        {
//            do_accept();
//        }
//
//    private:
//        void do_accept()
//        {
//
//        //The new connection gets its own strand
//        acceptor_.async_accept(boost::asio::make_strand(ioc_), beast::bind_front_handler(
//                &listener::on_accept,
//                shared_from_this()));
//        }
//
//    void
//    on_accept(beast::error_code ec, tcp::socket socket)
//    {
//        if(ec)
//        {
//            fail(ec, "accept");
//        }
//        else
//        {
//            // Create the session and run it
//            std::make_shared<session>(std::move(socket))->run();
//        }
//        // Accept another connection
//        do_accept();
//    }
//
//};
//int main() {
//    std::cout << "begin beast test!" << std::endl;
//    auto const address = boost::asio::ip::make_address("0.0.0.0");
//    auto const port = static_cast<unsigned short>(8001);
//    //the io_context is required for all I/O
//    boost::asio::io_context ioc{threads};
//
//    //create and launch a listening port
//    std::make_shared<listener>(ioc, tcp::endpoint{address,port})->run();
//    //run the I/O service on the requested number of threads
//    std::vector<std::thread> v;
//    v.reserve(threads -1 );
//    for(auto i = threads -1 ; i>0; --i){
//        v.emplace_back(
//                [&ioc]{
//                    ioc.run();
//                }
//                );
//    }
//    ioc.run();
//
//    return 0;
//}
