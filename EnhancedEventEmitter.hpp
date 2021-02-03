#pragma once
#include <json.hpp>
#include <string>
using json = nlohmann::json;
//#include "EventEmitter.hpp"
#include "utils/event_emitter.hpp"
//const logger = new Logger('EnhancedEventEmitter');
//https://github.com/HQarroum/event-emitter
namespace mediasoup
{
  
//using ListenerT = std::function< void( const std::string & str) >;
//using EventT = std::string;
class EnhancedEventEmitter //: public EventEmitter<EventT,ListenerT>
{
private:
  event::emitter_t<std::string> str_emitter;
  event::emitter_t<json> json_emitter;
  event::emitter_t<uint64_t> ptr_emitter;
  event::emitter_t<> null_emitter;
  event::emitter_t<std::pair<std::string,json>> pair_emitter;
public:
  EnhancedEventEmitter()
  {
        //this->setMaxListeners(Infinity);
  }
  void emit( const std::string &event)
  {
      null_emitter.emit(event);
  }
  void safeEmit( const std::string &event)
  {
      null_emitter.emit(event);
  }
  void emit( const char *event,const std::string &data)
  {
        str_emitter.emit(event,data);
  }
  void safeEmit( const char *event,const std::string &data)
  {
        str_emitter.emit(event,data);
  }
    
  void emit( const char *targetId,const char * event,const json &data)
  {
     std::pair<std::string,json> pair(event,data);
     pair_emitter.emit(targetId,pair);
  }
  void safeEmit( const char *targetId,const char *event,const json &data)
  {
     std::pair<std::string,json> pair(event,data);
     pair_emitter.emit(targetId,pair);
  }
    
    
  void emit( const char *event,const json &data)
  {
      json_emitter.emit(event,data);
  }
  void safeEmit( const char *event,const json &data)
  {
      json_emitter.emit(event,data);
  }
   void emit( const char *event,const void* data)
  {
      ptr_emitter.emit(event,(uint64_t)data);
  }
  void safeEmit( const char *event,const void *data)
  {
      ptr_emitter.emit(event,(uint64_t)data);
  }

  void on(const char *event, const std::function< void(const event::event_t<json>& e) > & listener )
  {
      //json_emitter.on(event, listener);
      //event::emitter_t<std::shared_ptr<PROTOO_MSG>> m_emitter;
      //[&](const event::event_t<std::shared_ptr<PROTOO_MSG>>& e)
      //json_emitter.on(event, [&] (const event::event_t<json>& e) {
        //e.value();
        // Lambda called for every `event.foo`.
      //});
      json_emitter.on(event, listener);
      
      //this->_channel->on("event",[]( const event::event_t<json>& e )
  }

  void once( const char *event,  const std::function< void(const event::event_t<json>& e) > & listener )
  {
      json_emitter.once(event, listener);
  }
  void on(const char *event, const std::function< void(const event::event_t<uint64_t>& e) > & listener )
  {
      ptr_emitter.on(event, listener);
        
  }

  void once( const char *event,  const std::function< void(const event::event_t<uint64_t>& e) > & listener )
  {
      ptr_emitter.once(event, listener);
  }
    
    void on(const char *event, const std::function< void(const event::event_t<std::string>& e) > & listener )
    {
        str_emitter.on(event, listener);
          
    }

    void once( const char *event,  const std::function< void(const event::event_t<std::string>& e) > & listener )
    {
        str_emitter.once(event, listener);
    }
    void on(const char *event, const std::function< void(const event::event_t<>& e) > & listener )
    {
        null_emitter.on(event, listener);
          
    }

    void once( const char *event,  const std::function< void(const event::event_t<>& e) > & listener )
    {
        null_emitter.once(event, listener);
    }
    
    
    
    void on(json &targetId, const std::function< void(const event::event_t<std::pair<std::string,json>>& e) > & listener )
    {
        pair_emitter.on(targetId.dump().c_str(), listener);
          
    }

    void once( json &targetId,  const std::function< void(const event::event_t<std::pair<std::string,json>>& e) > & listener )
    {
        pair_emitter.once(targetId.dump().c_str(), listener);
    }
};

/**
 router->on("@close", [&]( const event::event_t<uint64_t>& e ){
     auto * router = (Router*)e.value();
     this->m_routers.erase(std::remove( this->m_routers.begin(), this->m_routers.end(), router ),this->m_routers.end() );
 });
 
 */
}
