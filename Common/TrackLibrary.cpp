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
        
    if (auto tracksArray = parsedJson.getProperty("tracks", var()).getArray())
    {
        for (auto& track : *tracksArray)
        {
            Track* _track = new Track();;
            _track->setTitle(track["title"]);
            _track->setUrl(track["url"]);
            _track->setPath(track["path"]);
            _track->setAuthor(track["author"]);
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
    bool first = true;
    
    for(auto& track : tracks)
    {
        if (!first)
            response += ",";
        response += "{'title':'"+track->getTitle()+"',"+
        "'path':'"+track->getPath()+"',"+
        "'url':'"+track->getUrl()+
        "','author':'"+track->getAuthor()+"'}";
        
        first = false;
    }
    
    response = "{ 'tracks' : [" + response + "]}";
    
    return response;
}
