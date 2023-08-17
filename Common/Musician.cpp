/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 7.0.5

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/


#include "Musician.h"


//==============================================================================
Musician::Musician ()
{
    activated = true;
}

Musician::Musician (juce::String _type,bool _activated)
{
    activate(_activated);
    type = _type;
}

Musician::~Musician()
{

}


void Musician::activate(bool _activated)
{
    activated = _activated;
}


bool Musician::isActivated()
{
    return activated;
}

void Musician::setFile(juce::String _fileName)
{
    fileName = _fileName;
}

void Musician::setIconName(juce::String _icon)
{
    iconName = _icon;
}

void Musician::setType(juce::String _type)
{
    type = _type;
}

juce::String Musician::getFileName()
{
    return fileName;
}

juce::String Musician::getIconName()
{
    return iconName;
}

juce::String Musician::getType()
{
    return type;
}

juce::String Musician::serialize()
{
    juce::String result = "{\"type\":\""+type+"\",\"file\":\""+fileName+"\",\"icon\":\""+iconName+"\"}";
    
    return result;
}





