//
// Created by pan chenguang on 2021/2/4.
//

#ifndef TESTBEASTCLION_ROOM_H
#define TESTBEASTCLION_ROOM_H
#include "EnhancedEventEmitter.hpp"
#include <string>
#include <map>
#include <iostream>
#include "Peer.h"

using namespace std;

// Forward declaration
class websocket_session;

namespace protoo {
    class Room : public mediasoup::EnhancedEventEmitter {
    public:
        Room();
        ~Room();
    public:
        bool closed();
        void close();
        std::map<string,Peer>& getPeers();
        std::shared_ptr<Peer> createPeer(string peerId, websocket_session* transport);
        bool hasPeer(string peerId);
        Peer* getPeer(string peerId);
    private:
        // Map of Peers indexed by id
        std::map<string,Peer> m_peers;
        //whether room was closed
        bool m_closed;
    };
}


#endif //TESTBEASTCLION_ROOM_H
