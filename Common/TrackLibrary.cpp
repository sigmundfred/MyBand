/*
  ==============================================================================

    TrackLibrary.cpp
    Created: 2 Aug 2023 11:30:54am
    Author:  Frédéric FELTEN

  ==============================================================================
*/

#include "TrackLibrary.h"


TrackLibrary::TrackLibrary()
{
    File* _file = new File(File::getCurrentWorkingDirectory().getChildFile(libraryFile));
    
    if(_file->exists())
    {
        juce::Logger::writeToLog("Library loading");
        LoadLibrary(_file);
    }
    else
    {
        juce::Logger::writeToLog("Library creating:"+_file->getFullPathName());
        _file->create();
    }
    
}

TrackLibrary::~TrackLibrary()
{
    
}

void TrackLibrary::LoadLibrary(File *file)
{
    juce::var parsedJson = juce::JSON::parse(*file);
    tracks.clear();
        
    LibraryFromJSON(parsedJson);
    
}

void TrackLibrary::LoadLibrary(String* json)
{
    tracks.clear();
    juce::Logger::writeToLog("Library loading");
    juce::var parsedJson;
    juce::Logger::writeToLog(juce::JSON::parse(*json,parsedJson).getErrorMessage());
    
    LibraryFromJSON(parsedJson);
}

void TrackLibrary::LibraryFromJSON(juce::var json)
{
    if (auto tracksArray = json.getProperty("tracks", var()).getArray())
    {
        juce::Logger::writeToLog("Nb de morceaux:"+String(tracksArray->size()));
        for (auto& track : *tracksArray)
        {
            Track* _track = new Track();;
            _track->setTitle(track["title"]);
            _track->setUrl(track["url"]);
            _track->setPath(track["path"]);
            _track->setAuthor(track["author"]);
            // recuperation des instruments
            if (auto musiciansArray = track["musicians"].getArray())
            {
                juce::String log = "Nb musiciens:"+juce::String(musiciansArray->size());
                juce::Logger::writeToLog(log);
                for (auto& musician : *musiciansArray)
                {
                    Musician* _musician = new Musician(musician["type"]);
                    _musician->setFile(musician["file"]);
                    _musician->setIconName(musician["icon"]);
                    _track->addMusician(_musician);
                }
            }
            tracks.add(_track);
        }
    }
}

void TrackLibrary::SaveLibray()
{
    var _json;
    for(auto& track : tracks)
    {
        DynamicObject* obj = new DynamicObject();
        obj->setProperty("title", track->getTitle());
        obj->setProperty("path", track->getPath());
        _json.append(obj);
    }
    
    DynamicObject* master = new DynamicObject();
    master->setProperty("tasks", _json);
    var _finalJson(master);
    
    juce::Logger::writeToLog(JSON::toString(_finalJson));
}


void TrackLibrary::AddTrack(Track _track)
{
    tracks.add(&_track);
}


String TrackLibrary::Serialize(){
    String response = "";
    String subtracks = "";
    bool first = true;
    
    for(auto& track : tracks)
    {
        for (int i=0; i<track->getNbMusicians();i++)
        {
            if (i!=0)
                subtracks += ",";
            subtracks += track->getMusician(i)->serialize();
        }
            
        if (!first)
            response += ",";
        response += "{\"title\":\""+track->getTitle()+"\","+
        "\"path\":\""+track->getPath()+"\","+
        "\"url\":\""+track->getUrl()+
        "\",\"author\":\""+track->getAuthor()+
        "\",\"musicians\":["+subtracks+
        "]}";
        first = false;
    }
    
    response = "{\"tracks\":[" + response + "]}";
    
    return response;
    //return "{\"version\":\"test\",\"tracks\":[{\"title\":\"test\",\"url\":\"test\",\"path\":\"library/1\",\"author\":\"test\"}]}";
}

int TrackLibrary::getSize(){
    if(tracks.isEmpty())
        return 0;
    
    return tracks.size();
}

TrackLibrary::Track* TrackLibrary::getTrack(int id){
    return tracks[id];
}

