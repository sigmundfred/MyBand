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

#include "Stage.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
Stage::Stage ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    internalPath1.startNewSubPath (4.0f, 376.0f);
    internalPath1.lineTo (4.0f, 4.0f);
    internalPath1.lineTo (648.0f, 4.0f);
    internalPath1.lineTo (648.0f, 376.0f);
    internalPath1.closeSubPath();


    //[UserPreSize]
    //[/UserPreSize]

    //setSize (600, 400);
    musicians.add(new MusicianComponent());
    //musicians.getFirst()->setBounds(10, 10, 50, 100);
    addAndMakeVisible(musicians.getFirst());


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

Stage::~Stage()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]



    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void Stage::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xffedeeee));

    {
        float x = 0, y = 0;
        juce::Colour fillColour1 = juce::Colours::beige, fillColour2 = juce::Colour (0xff858585);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setGradientFill (juce::ColourGradient (fillColour1,
                                             static_cast<float> (proportionOfWidth (0.5309f)) - 0.0f + x,
                                             static_cast<float> (proportionOfHeight (0.4720f)) - 0.0f + y,
                                             fillColour2,
                                             static_cast<float> (proportionOfWidth (0.0988f)) - 0.0f + x,
                                             static_cast<float> (proportionOfHeight (0.1118f)) - 0.0f + y,
                                             true));
        g.fillPath (internalPath1, juce::AffineTransform::translation(x, y));
    }

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void Stage::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void Stage::refreshTrack(TrackLibrary::Track* _track)
{
    musicians.clear();
    for (int i=0;i<_track->getNbMusicians();i++)
    {
        musicians.add(new MusicianComponent(_track->getMusician(i)));
        musicians.getLast()->setTopLeftPosition(musicians.getLast()->getWidth()*i+10, 10);
        //musicians.getLast()->setBounds(100*i+10, 10, 100, 300); // a remplacer apr des valeurs gloables
        addAndMakeVisible(musicians.getLast());
    }
    
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="Stage" componentName="" parentClasses="public juce::Component"
                 constructorParams="" variableInitialisers="" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330" fixedSize="0" initialWidth="600"
                 initialHeight="400">
  <BACKGROUND backgroundColour="ffedeeee">
    <PATH pos="0 0 100 100" fill=" radial: 53.086% 47.205%, 9.877% 11.18%, 0=fff5f5dc, 1=ff858585"
          hasStroke="0" nonZeroWinding="1">s 4 376 l 4 4 l 648 4 l 648 376 x</PATH>
  </BACKGROUND>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

