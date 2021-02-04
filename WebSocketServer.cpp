//
// Created by 潘晨光 on 2021/2/3.
//

#include "WebSocketServer.h"
WebSocketServer::WebSocketServer(const char* address, int port, const char* doc_root)
{
    auto ip_address = net::ip::make_address(address);
    auto ip_port = static_cast<unsigned short>(port);
    auto ip_doc_root = doc_root;
    auto const threads = 5;
    m_state = boost::make_shared<shared_state>(doc_root);
    // TODO how to set options to server (keepalive:true keepaliveInterval:60000)
    // Create and launch a listening port
    boost::make_shared<listener>(m_ioc,tcp::endpoint{ip_address, ip_port}, doc_root)->run();
    // Capture SIGINT and SIGTERM to perform a clean shutdown
    net::signal_set signals(m_ioc, SIGINT, SIGTERM);
    signals.async_wait(
            [&](boost::system::error_code const&, int)
            {
                // Stop the io_context. This will cause run()
                // to return immediately, eventually destroying the
                // io_context and any remaining handlers in it.
                m_ioc.stop();
            });

    // Run the I/O service on the requested number of threads
    m_vecThread.reserve(threads - 1);
    for(auto i = threads - 1; i > 0; --i)
        m_vecThread.emplace_back(
                [&]
                {
                    m_ioc.run();
                });
    m_ioc.run();

    // (If we get here, it means we got a SIGINT or SIGTERM)
    // Block until all the threads exit
    for(auto& t : m_vecThread)
        t.join();
}

WebSocketServer::~WebSocketServer()
{
    m_ioc.stop();
}