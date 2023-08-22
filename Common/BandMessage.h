/*
  ==============================================================================

    Message.h
    Created: 4 Aug 2023 11:37:03am
    Author:  Frédéric FELTEN

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
using namespace juce;

#define REQUEST 0
#define NOTIFICATION 1

#define GENERAL 0
#define GUI 1
#define LIBRARY 2
#define PLAYER 3

class BandMessage {
public:
    BandMessage();
    BandMessage(int type,int section,int msgNumber);
    BandMessage(MemoryBlock* memory);
    ~BandMessage();
    
    int getType();
    int getSection();
    int getMessageNumber();
    MemoryBlock* getRawMsg();
    String getContent();
    void setContent(String _content);
    void addContent(String _content);
    
private:
    int type;
    int section;
    int msgNumber;
    String content;
};
