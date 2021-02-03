//
// Created by 潘晨光 on 2021/2/3.
//

#ifndef TESTBEASTCLION_WEBSOCKETSERVER_H
#define TESTBEASTCLION_WEBSOCKETSERVER_H

#include "listener.h"
#include "shared_state.h"

#include <boost/asio/signal_set.hpp>
#include <boost/smart_ptr.hpp>
#include <iostream>
#include <vector>
#include "EnhancedEventEmitter.hpp"


class WebSocketServer: public mediasoup::EnhancedEventEmitter {
public:
    WebSocketServer(const char* address, int port, const char* doc_root);
    ~WebSocketServer();
private:
    // The io_context is required for all I/O
    net::io_context m_ioc;
    std::vector<std::thread> m_vecThread;
    boost::shared_ptr<shared_state> m_state;
};


#endif //TESTBEASTCLION_WEBSOCKETSERVER_H
