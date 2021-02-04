//
// Created by pan chenguang on 2021/2/4.
//

#include "Room.h"

namespace protoo {
    Room::Room() {
      this->m_closed = false;
    }

    Room::~Room() {

    }

    //whether the Room is closed.
    bool Room::closed()
    {
        return this->m_closed;
    }

    std::map<string,Peer>& Room::getPeers()
    {
        return m_peers;
    }

    //close the room
    void Room::close()
    {
        if(this->closed())
        {
            return;
        }
        std::cout<<"Room close()"<<std::endl;
        for(auto it : this->m_peers)
        {
            it.second.close();
        }
    }

    std::shared_ptr<Peer> Room::createPeer(string peerId, websocket_session* transport)
    {
       if(transport == nullptr)
       {
           std::cout<<"[Room] no transport given"<<std::endl;
           return nullptr;
       }
       if(m_peers.find(peerId)==m_peers.end())
       {
           cout<<"there is already a Peer with same peerId:"<<peerId<<endl;
           return nullptr;
       }
       //Create the Peer instance
       shared_ptr<Peer> peer(new Peer(peerId, transport));
       //Peer peer(peerId, transport);
        // Store it in the map.
        this->m_peers.emplace(peer->id,peer);
        // TODO
        peer->on('close', [&] => {
            this->m_peers.erase(peer->id);
        });
        return peer;
    }

    bool Room::hasPeer(string peerId)
    {
        return this->m_peers.find(peerId)!=m_peers.end();
    }

    Peer* Room::getPeer(string peerId)
    {
        return &(this->m_peers[peerId]);
    }


}