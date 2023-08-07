/*
  ==============================================================================

    TrackLibrary.h
    Created: 2 Aug 2023 11:30:54am
    Author:  Frédéric FELTEN

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

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
                
            private:
                String title;
                String path;
                String url;
                String author;
        };
    
        void LoadLibrary(File *file);
        void AddTrack(Track _track);
        void RemoveTrack(int id);
        void SaveLibray();
        String Serialize();
    
    private:
        const String libraryFile = "./tracks.cfg";
        OwnedArray<Track> tracks;
    
};
