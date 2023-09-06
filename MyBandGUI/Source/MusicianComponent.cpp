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

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "MusicianComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
MusicianComponent::MusicianComponent (int _id) : Musician(), id(_id)
{
    init();

    //[Constructor] You can add your own custom stuff here..

}

MusicianComponent::MusicianComponent (Musician* _musician,int _id)
{
    setType(_musician->getType());
    setFile(_musician->getFileName());
    setIconName(_musician->getIconName());
    setEnabled(_musician->isActivated());
    id = _id;
    
    isActivated()?juce::Logger::writeToLog("Enabled"):juce::Logger::writeToLog("not Enabled");
    
    init();
}

void MusicianComponent::init()
{
    setOpaque (false);
    setSize (130, 260);
    
    name.reset (new juce::Label ("name",
                                 TRANS("Musician")));
    addAndMakeVisible (name.get());
    name->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    name->setJustificationType (juce::Justification::centredLeft);
    name->setEditable (false, false, false);
    name->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    //name->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    name->setBounds (10, this->getHeight()-70, 80, 20);

    activation.reset (new juce::ToggleButton ("activation"));
    addAndMakeVisible (activation.get());
    //activation->setEnabled(isActivated());
    activation->setToggleState(isActivated(), NotificationType::sendNotification);
    activation->setButtonText (juce::String());
    activation->onClick = [this] {
        isActivated()?activate(false):activate(true);
        int command = id << 8;
        if (isActivated()) command++;
        this->getParentComponent()->postCommandMessage(command);
    };
    //activation->addListener (this);

    activation->setBounds (40, this->getHeight()-30, 20, 20);


    //[UserPreSize]
    //[/UserPreSize]
}

MusicianComponent::MusicianComponent (juce::String _type,bool _activated) : Musician(_type,_activated)
{
    activation->setEnabled(isActivated());
    name->setTitle(_type);
}

    //[/Constructor]


MusicianComponent::~MusicianComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    name = nullptr;
    activation = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void MusicianComponent::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //g.drawImageAt(icon, 10, 10);
    g.drawImage(icon,0,0,130,180,0,0,50,103);
    //[/UserPrePaint]

    //g.fillAll (juce::Colour (0xff323e44));

    {
        int x = 188, y = 148, width = 180, height = 180;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
    }

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void MusicianComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}


void MusicianComponent::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == activation.get())
    {
        //[UserButtonCode_activation] -- add your button handler code here..
        isActivated()?activate(false):activate(true);
        //[/UserButtonCode_activation]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="Musician" componentName=""
                 parentClasses="public juce::Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44">
    <IMAGE pos="188 148 180 180" resource="" opacity="1.0" mode="0"/>
  </BACKGROUND>
  <LABEL name="name" id="4550c7f48b712f2c" memberName="name" virtualName=""
         explicitFocusOrder="0" pos="240 352 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="label text" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <TOGGLEBUTTON name="activation" id="4548c82571fb779e" memberName="activation"
                virtualName="" explicitFocusOrder="0" pos="200 392 150 24" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

