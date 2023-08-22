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

BandMessage::BandMessage(int _type, int _section, int _msgNb){
    type = _type;
    section = _section;
    msgNumber = _msgNb;
    
    content = "";
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

void BandMessage::setContent(String _content)
{
    content = _content;
}

void BandMessage::addContent(String _content)
{
    content += _content;
}

MemoryBlock* BandMessage::getRawMsg()
{
    MemoryBlock *message = new MemoryBlock();
    message->append(&type, 1);
    message->append(&section, 1);
    message->append(&msgNumber, 1);
    
    if (content.length() > 0)
        message->append(content.toUTF8(), content.length());
    
    return message;
        
}
