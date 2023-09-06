/*
  ==============================================================================

    TrackLibrary.h
    Created: 2 Aug 2023 11:30:54am
    Author:  Frédéric FELTEN

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "Musician.h"

using namespace juce;

class TrackLibrary 
{
    public:
    
        TrackLibrary();
        ~TrackLibrary();
    
        class Track
        {
            public:
            Track(){};
            Track(String name,String path){};
            Track(File _jsonFile)
            {
                juce::var parsedJson = juce::JSON::parse(_jsonFile);
                
            }
            ~Track(){};
            
            void setTitle(String _title){
                title = _title;
            }
            
            void setPath(String _path){
                path = _path;
            }
            
            void setUrl(String _url){
                url = _url;
            }
            
            void setAuthor(String _author){
                author = _author;
            }
            
            String getTitle(){
                return title;
            }
            
            String getPath(){
                return path;
            }
            
            String getUrl(){
                return url;
            }
            
            String getAuthor(){
                return author;
            }
            
            
            void addMusician(Musician *musician)
            {
                musicians.add(musician);
            }
            
            Musician* getMusician(int i)
            {
                return musicians[i];
            }
            
            void setMusicianState(int id, bool _state)
            {
                musicians[id]->activate(_state);
            }
            
            int getNbMusicians()
            {
                return musicians.size();
            }
                
            private:
                String title;
                String path;
                String url;
                String author;
                OwnedArray<Musician> musicians;
            
            JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Track)
        };
    
        void LoadLibrary(File *file);
        void LoadLibrary(String *json);
        void DownloadTrack(juce::URL _url);
        void AddTrack(Track _track);
        Track* getTrack(int id);
        void RemoveTrack(int id);
        void SaveLibray();
        void selectTrack(int id);
        int getSize();
        String Serialize();
        TrackLibrary::Track* getSelectedTrack();
        
    
    private:
        void LibraryFromJSON(var json);
        const String libraryFile = "./tracks.cfg";
        OwnedArray<Track> tracks;
        int selectedTrack = -1;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TrackLibrary)
};
