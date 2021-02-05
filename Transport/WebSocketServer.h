//
// Created by 潘晨光 on 2021/2/3.
//

#ifndef TESTBEASTCLION_WEBSOCKETSERVER_H
#define TESTBEASTCLION_WEBSOCKETSERVER_H

#include "HttpTransport.h"
#include "WebSocketTransport.h"
#include <boost/asio/signal_set.hpp>
#include <boost/smart_ptr.hpp>
#include <iostream>
#include <vector>
#include "EventEmitter.hpp"


class WebSocketServer: public EventEmitter,public boost::enable_shared_from_this<WebSocketServer> {
public:
    WebSocketServer(const char* address, int port, const char* doc_root);
    ~WebSocketServer();
private:
    void fail(beast::error_code ec, char const* what);
    void on_accept(beast::error_code ec, tcp::socket socket);
    void initListener(tcp::endpoint endpoint,const char* doc_root);
private:
    // The io_context is required for all I/O
    net::io_context m_ioc;
    std::vector<std::thread> m_vecThread;
    tcp::acceptor acceptor_;
    const char* doc_root;

};


#endif //TESTBEASTCLION_WEBSOCKETSERVER_H
