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

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include <JuceHeader.h>
//[/Headers]




//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class Musician
{
public:
    Musician ();
    Musician (juce::String type, bool activated = true);
    ~Musician();


    void activate(bool activated);
    bool isActivated();
    void setFile(juce::String _fileName);
    void setIconName(juce::String _icon);
    void setType(juce::String _type);
    juce::String getFileName();
    juce::String getIconName();
    juce::String getType();
    juce::String serialize();


private:

    juce::Image icon = juce::ImageFileFormat::loadFrom(BinaryData::test_gif, BinaryData::test_gifSize);
    bool activated;
    juce::String type;
    juce::String fileName;
    juce::String iconName;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Musician)
};


