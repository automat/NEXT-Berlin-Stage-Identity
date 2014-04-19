//
//  Mapping.h
//  SessionView
//
//  Created by Henryk Wollik on 19/04/14.
//
//

#ifndef SessionView_JsonBinding_h
#define SessionView_JsonBinding_h

#include "cinder/Json.h"
#include "cinder/app/App.h"
#include "cinder/Utilities.h"
#include <map>
#include <string>
#include "cinder/gl/Texture.h"
#include "Session.h"
#include "Speaker.h"
#include "Session.h"

#include <exception>
#include <boost/filesystem.hpp>
#include <boost/assign/std/map.hpp>
#include <boost/assign/std/vector.hpp>

namespace next {
    using namespace ci;
    using namespace ci::app;
    using namespace std;
    using namespace boost;
    using namespace boost::assign;


    class Mapping {
    public:
        ////////////////////////////////////////////////////////////////////////////////////////////////
        //
        //  Exceptions
        //
        ////////////////////////////////////////////////////////////////////////////////////////////////

        
        class NoSessionExc : public std::exception {
            std::string mMessage;
        public:
            NoSessionExc(const std::string& msg) : mMessage("Session not available: " + msg){};
            inline virtual const char* what() const throw(){return mMessage.c_str();}
        };
        
        class NoPersonExc : public std::exception {
            std::string mMessage;
        public:
            NoPersonExc(const std::string& msg) : mMessage("Person not available: " + msg){};
            inline virtual const char* what() const throw(){return mMessage.c_str();}
        };
        
        class NoEventExc : public std::exception {
            std::string mMessage;
        public:
            NoEventExc(const std::string& msg) : mMessage("Event not available: " + msg){};
            inline virtual const char* what() const throw(){return mMessage.c_str();}
        };
        
        class NoImageExc : public std::exception {
            std::string mMessage;
        public:
            NoImageExc(const std::string& msg) : mMessage("Image not available: " + msg){};
            inline virtual const char* what() const throw(){return mMessage.c_str();}
        };
        
        class NoChildExc : public std::exception {
            std::string mMessage;
        public:
            NoChildExc(const std::string& msg) : mMessage("Child not available: " + msg){};
            inline virtual const char* what() const throw(){return mMessage.c_str();}
        };
        
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////////
        //
        //  Utilities
        //
        ////////////////////////////////////////////////////////////////////////////////////////////////

        // cant get child with integer as key, must traverse throu
        inline static const JsonTree& getChild(const JsonTree& json, uint32_t _id){
            for(const auto& child : json){
                if(child.getChild("id").getValue<uint32_t>() == _id){
                    return child;
                }
            }
            throw NoChildExc(toString(_id));
        }
        
        
    public:
        ////////////////////////////////////////////////////////////////////////////////////////////////
        //
        //  Get
        //
        ////////////////////////////////////////////////////////////////////////////////////////////////

        inline static void Get(const string& jsonFilepath,
                               const string& imageFilepath,
                               uint32_t session_id,
                               map<uint32_t, gl::Texture>*& images ,
                               map<uint32_t, Speaker>*& speakers ,
                               map<uint32_t, Event>*& events ,
                               Session*& session){
            
            if(images)   delete images;
            if(speakers) delete speakers;
            if(events)   delete events;
            if(session)  delete session;
             
            map<uint32_t, gl::Texture>* _images   = new map<uint32_t, gl::Texture>();
            map<uint32_t, Speaker>*     _speakers = new map<uint32_t, Speaker>();
            map<uint32_t, Event>*       _events   = new map<uint32_t, Event>();
            
            JsonTree data = JsonTree(loadFile(jsonFilepath));
            
            const JsonTree& jsonSessions = data.getChild("sessions");
            const JsonTree& jsonPersons  = data.getChild("persons");
            const JsonTree& jsonEvents   = data.getChild("events");
            
            const JsonTree& jsonSession = getChild(jsonSessions, session_id);
            const JsonTree& jsonEvent_ids = jsonSession.getChild("event_ids");
            
            for(const auto& event_id : jsonEvent_ids){
                uint32_t _event_id = event_id.getValue<uint32_t>();
                
                const JsonTree& jsonEvent = getChild(jsonEvents, _event_id);
                
                Event& event         = (*_events)[_event_id] = Event();
                event.title          = jsonEvent.getChild("title").getValue<string>();
                event.endHourString  = jsonEvent.getChild("endHourString").getValue<string>();
                event.endTimeStamp   = jsonEvent.getChild("endTimeStamp").getValue<time_t>();
                event.startTimeStamp = jsonEvent.getChild("startTimeStamp").getValue<string>();
                event.type           = jsonEvent.getChild("type").getValue<string>();
                
                const JsonTree& jsonPerson_ids = jsonEvent.getChild("person_ids");
                for(const auto& person_id : jsonPerson_ids){
                    uint32_t _person_id = person_id.getValue<uint32_t>();
                    
                    if (!static_cast<bool>(_speakers->count(_person_id))) {
                        const JsonTree& jsonPerson = getChild(jsonPersons, _person_id);
                        
                        string _imageFilepath = imageFilepath + "/" + toString(_person_id) + ".png";
                        const gl::Texture& image = (*_images)[_person_id] = gl::Texture(loadImage(_imageFilepath));
                        
                        Speaker& speaker      = (*_speakers)[_person_id] = Speaker();
                        speaker.imageRef      = image.weakClone();
                        speaker.companyName   = jsonPerson.getChild("companyName").getValue<string>();
                        speaker.companyRole   = jsonPerson.getChild("companyRole").getValue<string>();
                        speaker.name          = jsonPerson.getChild("name").getValue<string>();
                        
                        if(jsonPerson.hasChild("profileTwitterHandle")){
                            speaker.twitterHandle = jsonPerson.getChild("profileTwitterHandle").getValue<string>();
                        }
                    }
                 
                    event.speakers += &(*_speakers)[_person_id];
                }
            }
            
            
            
            images   = _images;
            speakers = _speakers;
            events   = _events;
            
            session = new Session();
            Session& _session = *session;
            _session._id                 = session_id;
            _session.title               = jsonSession.getChild("title").getValue<string>();
            _session.endHourString       = jsonSession.getChild("endHourString").getValue<string>();
            _session.endTimeStamp        = jsonSession.getChild("endTimeStamp").getValue<time_t>();
            _session.startHourClockFile  = jsonSession.getChild("startHourClockFile").getValue<string>();
            _session.startHourString     = jsonSession.getChild("startHourString").getValue<string>();
            _session.startTimeStamp      = jsonSession.getChild("startTimeStamp").getValue<time_t>();
            _session.type                = jsonSession.getChild("type").getValue<string>();
            _session.events              = events;
            
        }
        
    };
    
    
}

#endif
