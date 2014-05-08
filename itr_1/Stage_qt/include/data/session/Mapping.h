//
//  Mapping.h
//  SessionView
//
//  Created by Henryk Wollik on 19/04/14.
//
//

#ifndef SessionView_JsonBinding_h
#define SessionView_JsonBinding_h

#include <map>
#include <string>

#include "cinder/Json.h"
#include "cinder/app/App.h"
#include "cinder/Utilities.h"

#include "cinder/gl/Texture.h"
#include "data/session/Session.h"
#include "data/session/Speaker.h"

#include <exception>
#include <boost/filesystem.hpp>
#include <boost/assign/std/map.hpp>
#include <boost/assign/std/vector.hpp>

#include "Config.h"

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

        inline static void Get(const string& dataSession,
                               const string& folderImagesSpeaker,
                               const string& folderImagesClocks,
                               uint32_t session_id,
                               map<uint32_t, gl::Texture>*& imagesClocks,
                               map<uint32_t, gl::Texture>*& imagesSpeaker ,
                               map<uint32_t, Speaker>*& speakers ,
                               map<uint32_t, Event>*& events ,
                               Session*& session){
            
            if(imagesSpeaker)   delete imagesSpeaker;
            if(speakers)        delete speakers;
            if(events)          delete events;
            if(session)         delete session;
            
            
            // keys session
            static const string session_title("title");
            static const string session_endHourString("endHourString");
            static const string session_endTimeStamp("endTimeStamp");
            static const string session_startTimeStamp("startTimeStamp");
            static const string session_startHourTimeStamp("startHourTimeStamp");
            static const string session_startHourClockFile("startHourClockFile");
            static const string session_startHourString("startHourString");
            static const string session_type("type");
            static const string session_event_ids("event_ids");
            
            
            //  keys event
            static const string key_event_type("type");
            static const string key_event_title("title");
            static const string key_event_endHourString("endHourString");
            static const string key_event_endTimeStamp("endTimeStamp");
            static const string key_event_startTimeStamp("startTimeStamp");
            static const string key_event_person_ids("person_ids");
            
            
            //  keys speaker
            static const string key_speaker_companyName("companyName");
            static const string key_speaker_companyRole("companyRole");
            static const string key_speaker_name("name");
            static const string key_speaker_profileTwitterHandle("profileTwitterHandle");
            
            static const string stringEmpty("");
            
            
            map<uint32_t, gl::Texture>* _clocks   = new map<uint32_t, gl::Texture>();
            map<uint32_t, gl::Texture>* _images   = new map<uint32_t, gl::Texture>();
            map<uint32_t, Speaker>*     _speakers = new map<uint32_t, Speaker>();
            map<uint32_t, Event>*       _events   = new map<uint32_t, Event>();
            
            JsonTree data = JsonTree(loadFile(dataSession));
            
            const JsonTree& jsonSessions = data.getChild("sessions");
            const JsonTree& jsonPersons  = data.getChild("persons");
            const JsonTree& jsonEvents   = data.getChild("events");
            
            const JsonTree& jsonSession = getChild(jsonSessions, session_id);
            const JsonTree& jsonEvent_ids = jsonSession.getChild(session_event_ids);
            
            for(const auto& event_id : jsonEvent_ids){
                uint32_t _event_id = event_id.getValue<uint32_t>();
                
                const JsonTree& jsonEvent = getChild(jsonEvents, _event_id);
                
                Event event;
                event.title          = jsonEvent.getChild(key_event_title).getValue<string>();
                event.endHourString  = jsonEvent.getChild(key_event_endHourString).getValue<string>();
                event.endTimeStamp   = jsonEvent.getChild(key_event_endTimeStamp).getValue<time_t>();
                event.startTimeStamp = jsonEvent.getChild(key_event_startTimeStamp).getValue<string>();
                
                if(jsonEvent.hasChild(key_event_type)){
                    event.type = jsonEvent.getChild(key_event_type).getValue<string>();
                }
                
                const JsonTree& jsonPerson_ids = jsonEvent.getChild(key_event_person_ids);
                for(const auto& person_id : jsonPerson_ids){
                    uint32_t _person_id = person_id.getValue<uint32_t>();
                    
                    if (!static_cast<bool>(_speakers->count(_person_id))) {
                        const JsonTree& jsonPerson = getChild(jsonPersons, _person_id);
                        
                        string _imageFilepath = folderImagesSpeaker + "/" + toString(_person_id) + ".png";
                        const gl::Texture& image = (*_images)[_person_id] = gl::Texture(loadImage(_imageFilepath));
                        
                        Speaker& speaker      = (*_speakers)[_person_id] = Speaker();
                        
                        speaker.imageRef      = image.weakClone();
                        speaker.companyName   = jsonPerson.hasChild(key_speaker_companyName)          ? jsonPerson.getChild(key_speaker_companyName).getValue<string>() : stringEmpty;
                        speaker.companyRole   = jsonPerson.hasChild(key_speaker_companyRole)          ? jsonPerson.getChild(key_speaker_companyRole).getValue<string>() : stringEmpty;
                        speaker.name          = jsonPerson.hasChild(key_speaker_name)                 ? jsonPerson.getChild(key_speaker_name).getValue<string>() : stringEmpty;
                        speaker.twitterHandle = jsonPerson.hasChild(key_speaker_profileTwitterHandle) ? jsonPerson.getChild(key_speaker_profileTwitterHandle).getValue<string>() : stringEmpty;
                    }
                 
                    event.speakers += &(*_speakers)[_person_id];
                }
                
                (*_events)[_event_id] = event;
            }
            
            imagesSpeaker = _images;
            speakers      = _speakers;
            events        = _events;
            
            session = new Session();
            Session& _session = *session;
            _session._id                 = session_id;
            _session.title               = jsonSession.getChild(session_title).getValue<string>();
            _session.endHourString       = jsonSession.getChild(session_endHourString).getValue<string>();
            _session.endTimeStamp        = jsonSession.getChild(session_endTimeStamp).getValue<time_t>();
            _session.startHourClockFile  = jsonSession.getChild(session_startHourClockFile).getValue<string>();
            _session.startHourString     = jsonSession.getChild(session_startHourString).getValue<string>();
            _session.startTimeStamp      = jsonSession.getChild(session_startTimeStamp).getValue<time_t>();
            _session.type                = jsonSession.getChild(session_type).getValue<string>();
            _session.events              = events;
            
            string clockFilepath   = folderImagesClocks + "/" + _session.startHourClockFile;
            (*_clocks)[session_id] = gl::Texture(loadImage(clockFilepath));
            
            _session.clockImageRef = (*_clocks)[session_id].weakClone();
            
            imagesClocks = _clocks;
        }
        
    };
    
    
}

#endif
