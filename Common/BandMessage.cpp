/*
  ==============================================================================

    Message.cpp
    Created: 4 Aug 2023 11:37:03am
    Author:  Frédéric FELTEN

  ==============================================================================
*/

#include "BandMessage.h"

BandMessage::BandMessage(){
    ;
}

BandMessage::BandMessage(MemoryBlock* memory){
    char* header = memory->begin();
    
    type = header[0];
    section = header[1];
    msgNumber = header[2];
    
    if (memory->getSize() > 3)
    {
        memory->removeSection(0, 3);
        content = memory->toString();
    }
    
}

BandMessage::~BandMessage(){
    ;
}

int BandMessage::getType()
{
    return type;
}

int BandMessage::getSection()
{
    return section;
}

int BandMessage::getMessageNumber()
{
    return msgNumber;
}

String BandMessage::getContent()
{
    return content;
}
