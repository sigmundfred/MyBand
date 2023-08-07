/*
  ==============================================================================

    AudioGUI.cpp
    Created: 27 Jul 2023 5:00:59pm
    Author:  Frédéric FELTEN

  ==============================================================================
*/

#include "MyBandGUI.h"

MyBandGUI::MyBandGUI()
        : state (Stopped)
    {
        
        addAndMakeVisible (&openButton);
        openButton.setButtonText ("Open...");
        openButton.onClick = [this] { openButtonClicked(); };

        addAndMakeVisible (&playButton);
        playButton.setButtonText ("Play");
        playButton.onClick = [this] { playButtonClicked(); };
        playButton.setColour (juce::TextButton::buttonColourId, juce::Colours::green);
        playButton.setEnabled (false);

        addAndMakeVisible (&stopButton);
        stopButton.setButtonText ("Stop");
        stopButton.onClick = [this] { stopButtonClicked(); };
        stopButton.setColour (juce::TextButton::buttonColourId, juce::Colours::red);
        stopButton.setEnabled (false);

        addAndMakeVisible (&loopingToggle);
        loopingToggle.setButtonText ("Loop");
        loopingToggle.onClick = [this] { loopButtonChanged(); };

        addAndMakeVisible (&currentPositionLabel);
        currentPositionLabel.setText ("Stopped", juce::dontSendNotification);
        addAndMakeVisible (&song1);
        song1.setText("None", juce::dontSendNotification);
        addAndMakeVisible (&song2);
        song2.setText("None", juce::dontSendNotification);

        setSize (640, 480);

        formatManager.registerBasicFormats();
        //transportSource.addChangeListener (this);

        setAudioChannels (2, 2);
        startTimer (20);
    }

MyBandGUI::~MyBandGUI()
{
        shutdownAudio();
}

void MyBandGUI::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    mixSource.prepareToPlay (samplesPerBlockExpected, sampleRate);
}

    void MyBandGUI::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
    {
        if (!checkReaderSources())
        {
            bufferToFill.clearActiveBufferRegion();
            return;
        }

        mixSource.getNextAudioBlock (bufferToFill);
    }

    void MyBandGUI::releaseResources()
    {
        mixSource.releaseResources();
    }

    void MyBandGUI::resized()
    {
        openButton          .setBounds (10, 10,  getWidth() - 20, 20);
        playButton          .setBounds (10, 40,  getWidth() - 20, 20);
        stopButton          .setBounds (10, 70,  getWidth() - 20, 20);
        loopingToggle       .setBounds (10, 100, getWidth() - 20, 20);
        currentPositionLabel.setBounds (10, 130, getWidth() - 20, 20);
    }

    void MyBandGUI::changeListenerCallback (juce::ChangeBroadcaster* source)
    {
        if (typeid(source) == typeid(juce::AudioTransportSource))
        {
            if (isOneSourcePlaying())
                changeState (Playing);
            else
                changeState (Stopped);
        }
    }

    void MyBandGUI::timerCallback()
    {
        if (isOneSourcePlaying())
        {
            juce::RelativeTime position (getCurrentPositionTransportSources());

            auto minutes = ((int) position.inMinutes()) % 60;
            auto seconds = ((int) position.inSeconds()) % 60;
            auto millis  = ((int) position.inMilliseconds()) % 1000;

            auto positionString = juce::String::formatted ("%02d:%02d:%03d", minutes, seconds, millis);

            currentPositionLabel.setText (positionString, juce::dontSendNotification);
        }
        else
        {
            currentPositionLabel.setText ("Stopped", juce::dontSendNotification);
        }
    }

    void MyBandGUI::updateLoopState (bool shouldLoop)
    {
        if (readerSource.get() != nullptr)
            readerSource->setLooping (shouldLoop);
    }
    
    void MyBandGUI::start()
    {
        startTransportSources();
    }

    void MyBandGUI::changeState (TransportState newState)
    {
        if (state != newState)
        {
            state = newState;

            switch (state)
            {
                case Stopped:
                    stopButton.setEnabled (false);
                    playButton.setEnabled (true);
                    setPositionTransportSources(0.0);
                    break;

                case Starting:
                    playButton.setEnabled (false);
                    start();
                    break;

                case Playing:
                    stopButton.setEnabled (true);
                    break;

                case Stopping:
                    stopTransportSources();
                    break;
            }
        }
    }

    void MyBandGUI::openButtonClicked()
    {
        chooser = std::make_unique<juce::FileChooser> ("Select a Wave file to play...",
                                                       juce::File{},
                                                       "*.wav");
        auto chooserFlags = juce::FileBrowserComponent::openMode
        //                  | juce::FileBrowserComponent::canSelectFiles;
        | juce::FileBrowserComponent::canSelectDirectories;

        //chooser->launchAsync (chooserFlags, [this] (const juce::FileChooser& fc)
        chooser->launchAsync (chooserFlags, [this] (const juce::FileChooser& fc)
         {
            auto dir = fc.getResult();
            juce::Array<juce::File> list = dir.findChildFiles(2, false, "*.wav");
            
            for (int i=0; i<list.size(); i++)
            {
                auto file = list.getReference(i);
                auto* reader = formatManager.createReaderFor(file);
                
                if (reader != nullptr)
                {
                    auto newSource = std::make_unique<juce::AudioFormatReaderSource> (reader, true);
                    juce::AudioTransportSource* transportSource = new juce::AudioTransportSource();
                    transportSource->addChangeListener (this);
                    transportSource->setSource (newSource.get(), 0, nullptr, reader->sampleRate);
                    
                    transportSources.add(transportSource);
                    mixSource.addInputSource(transportSource, false);
                    readerSources.add(newSource.release());
                }
            }
            playButton.setEnabled (true); // verifier taille liste d'abord
            
            /*
            if (file != juce::File{})
            {
                auto* reader = formatManager.createReaderFor (file);

                if (reader != nullptr)
                {
                    auto newSource = std::make_unique<juce::AudioFormatReaderSource> (reader, true);
                    transportSource.setSource (newSource.get(), 0, nullptr, reader->sampleRate);
                    mixSource.addInputSource(&transportSource, false);
                    playButton.setEnabled (true);
                    readerSource.reset (newSource.release());
                }
            }*/
        });
    }

    void MyBandGUI::playButtonClicked()
    {
        updateLoopState (loopingToggle.getToggleState());
        changeState (Starting);
    }

    void MyBandGUI::stopButtonClicked()
    {
        changeState (Stopping);
    }

    void MyBandGUI::loopButtonChanged()
    {
        updateLoopState (loopingToggle.getToggleState());
    }

    bool MyBandGUI::checkReaderSources()
    {
        //TODO
        /*
        for(int i=0; i<readerSources.size();i++)
        {
            if(readerSources.getReference(i) == NULL)
                return false;
        }*/
        
        return true;
    }
    
    void MyBandGUI::stopTransportSources()
    {
        for(int i=0; i<transportSources.size(); i++)
        {
            transportSources[i]->stop();
        }
    }
    
    void MyBandGUI::startTransportSources()
    {
        for(int i=0; i<transportSources.size(); i++)
        {
            transportSources[i]->start();
        }
    }
    
    void MyBandGUI::setPositionTransportSources(double d)
    {
        for(int i=0; i<transportSources.size(); i++)
        {
            transportSources[i]->setPosition(d);
        }
    }
    
    bool MyBandGUI::isOneSourcePlaying()
    {
        for(int i=0; i<transportSources.size(); i++)
        {
            if ( transportSources[i]->isPlaying())
            {
                return true;
            }
        }
        
        return false;
    }
    
    double MyBandGUI::getCurrentPositionTransportSources()
    {
        for(int i=0; i<transportSources.size(); i++)
        {
            if ( transportSources[i]->isPlaying())
            {
                return  transportSources[i]->getCurrentPosition();
            }
        }
        
        return 0;
    }