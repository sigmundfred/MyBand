/*
  ==============================================================================

    AudioUnit.cpp
    Created: 27 Jul 2023 5:00:59pm
    Author:  Frédéric FELTEN

  ==============================================================================
*/

#include "MyBandUnit.h"

MyBandUnit::MyBandUnit()
        : state (Stopped)
    {

        setSize (640, 480);

        formatManager.registerBasicFormats();
        //transportSource.addChangeListener (this);

        setAudioChannels (2, 2);
        startTimer (20);
    }

MyBandUnit::~MyBandUnit()
{
        shutdownAudio();
}

void MyBandUnit::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    mixSource.prepareToPlay (samplesPerBlockExpected, sampleRate);
}

    void MyBandUnit::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
    {
        if (!checkReaderSources())
        {
            bufferToFill.clearActiveBufferRegion();
            return;
        }

        mixSource.getNextAudioBlock (bufferToFill);
    }

    void MyBandUnit::releaseResources()
    {
        mixSource.releaseResources();
    }


    void MyBandUnit::resized()
    {
    }

    void MyBandUnit::changeListenerCallback (juce::ChangeBroadcaster* source)
    {
        if (typeid(source) == typeid(juce::AudioTransportSource))
        {
            if (isOneSourcePlaying())
                changeState (Playing);
            else
                changeState (Stopped);
        }
    }

    void MyBandUnit::timerCallback()
    {
        if (isOneSourcePlaying())
        {
            juce::RelativeTime position (getCurrentPositionTransportSources());

            auto minutes = ((int) position.inMinutes()) % 60;
            auto seconds = ((int) position.inSeconds()) % 60;
            auto millis  = ((int) position.inMilliseconds()) % 1000;

            auto positionString = juce::String::formatted ("%02d:%02d:%03d", minutes, seconds, millis);

        }
        else
        {
        }
    }

    void MyBandUnit::updateLoopState (bool shouldLoop)
    {
        if (readerSource.get() != nullptr)
            readerSource->setLooping (shouldLoop);
    }
    
    void MyBandUnit::start()
    {
        startTransportSources();
    }

    void MyBandUnit::changeState (TransportState newState)
    {
        if (state != newState)
        {
            state = newState;

            switch (state)
            {
                case Stopped:
                    setPositionTransportSources(0.0);
                    break;

                case Starting:
                    start();
                    break;

                case Playing:
                    break;

                case Stopping:
                    stopTransportSources();
                    break;
            }
        }
    }

    void MyBandUnit::openButtonClicked()
    {
            File* dir = new File(File::getCurrentWorkingDirectory().getChildFile("library/1"));
            juce::Array<juce::File> list = dir->findChildFiles(2, false, "*.wav");
            
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
    }

    void MyBandUnit::playButtonClicked()
    {
        changeState (Starting);
    }

    void MyBandUnit::stopButtonClicked()
    {
        changeState (Stopping);
    }

    void MyBandUnit::loopButtonChanged()
    {
    }

    bool MyBandUnit::checkReaderSources()
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
    
    void MyBandUnit::stopTransportSources()
    {
        for(int i=0; i<transportSources.size(); i++)
        {
            transportSources[i]->stop();
        }
    }
    
    void MyBandUnit::startTransportSources()
    {
        for(int i=0; i<transportSources.size(); i++)
        {
            transportSources[i]->start();
        }
    }
    
    void MyBandUnit::setPositionTransportSources(double d)
    {
        for(int i=0; i<transportSources.size(); i++)
        {
            transportSources[i]->setPosition(d);
        }
    }
    
    bool MyBandUnit::isOneSourcePlaying()
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
    
    double MyBandUnit::getCurrentPositionTransportSources()
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

    // heritage InterporcessConnection
    void MyBandUnit::connectionMade()
    {
        juce::Logger::writeToLog("Connected to GUI");
        
        char header[3] = {0x01,0x02,0x01};
        //String myString("Welcome");
        //MemoryBlock *message = new MemoryBlock(myString.toUTF8(), myString.length());
        MemoryBlock *message = new MemoryBlock();
        message->append(header, 1);
        message->append(header+1, 1);
        message->append(header+2, 1);
        message->append(library.Serialize().toUTF8(), library.Serialize().length());
        
        sendMessage(*message);
    }

    void MyBandUnit::connectionLost()
    {
        juce::Logger::writeToLog("Disconnected from unit");
    }

    void MyBandUnit::messageReceived (const juce::MemoryBlock &message)
    {
        juce::Logger::writeToLog("Message received");
    }
