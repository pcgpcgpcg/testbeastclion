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
#include "WebSocketServer.h"

int main(int argc, char* argv[])
{
    // Check command line arguments.
    std::cout << "websocket server started..."<<std::endl;
    WebSocketServer* pServer = new WebSocketServer("0.0.0.0",8001,"");
    getchar();
    delete pServer;

    return EXIT_SUCCESS;
}