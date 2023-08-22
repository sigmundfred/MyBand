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
#include "../../Common/Musician.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class MusicianComponent  : public Musician,
                  public juce::Component,
                  public juce::Button::Listener
{
public:
    //==============================================================================
    MusicianComponent (int _id);
    MusicianComponent (Musician* _musician,int _id);
    ~MusicianComponent() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    MusicianComponent (juce::String _type,bool _activated);
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;



private:
    void init();
    //[UserVariables]   -- You can add your own custom variables in this section.
    juce::Image icon = juce::ImageFileFormat::loadFrom(BinaryData::test_gif, BinaryData::test_gifSize);
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::Label> name;
    std::unique_ptr<juce::ToggleButton> activation;
    int id;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MusicianComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

