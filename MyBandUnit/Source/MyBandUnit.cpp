/*
  ==============================================================================

    AudioUnit.cpp
    Created: 27 Jul 2023 5:00:59pm
    Author:  Frédéric FELTEN

  ==============================================================================
*/

#include "MyBandUnit.h"
#include "../../Common/BandMessage.h"

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
        if (typeid(*source) == typeid(juce::AudioTransportSource))
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
                {
                    setPositionTransportSources(0.0);
                    BandMessage* _message = new BandMessage(NOTIFICATION, PLAYER, 0x00);
                    _message->addContent(String(0x02));
                    sendMessage(*_message->getRawMsg());
                    break;
                }
                case Starting:
                    start();
                    break;

                case Playing:
                {
                    BandMessage* _message = new BandMessage(NOTIFICATION, PLAYER, 0x00);
                    _message->addContent(String(0x01));
                    sendMessage(*_message->getRawMsg());
                    break;
                }
                case Stopping:
                    stopTransportSources();
                    break;
            }
        }
    }

    void MyBandUnit::loadTransportSources()
    {
        TrackLibrary::Track* _currentTrack = library.getSelectedTrack();
        transportSources.clear();
        
        for (int i=0;i<_currentTrack->getNbMusicians();i++)
        {
            if (_currentTrack->getMusician(i)->isActivated())
            {
                File* file = new File(File::getCurrentWorkingDirectory().getChildFile(_currentTrack->getPath()+_currentTrack->getMusician(i)->getFileName()));
                
                auto* reader = formatManager.createReaderFor(*file);
                
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
        }
        
        /*
        File* dir = new File(File::getCurrentWorkingDirectory().getChildFile(_currentTrack->getPath()));
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

    

//-----------------------------------------------------------------------------------------------------------
// heritage InterprocessConnection
//-----------------------------------------------------------------------------------------------------------
void MyBandUnit::connectionMade()
{
    juce::Logger::writeToLog("Connected to GUI");
    
    char header[3] = {0x01,0x02,0x01};

    MemoryBlock *message = new MemoryBlock();
    message->append(header, 1);
    message->append(header+1, 1);
    message->append(header+2, 1);
    String _content = library.Serialize();
    juce::Logger::writeToLog(_content+" : "+String(_content.length()));
    message->append(_content.toUTF8(), _content.length()+2); // +2 nécessaire pour transmettre la fin de la chaine. ???
    
    sendMessage(*message);
}

void MyBandUnit::connectionLost()
{
    juce::Logger::writeToLog("Disconnected from GUI");
}

void MyBandUnit::messageReceived (const juce::MemoryBlock &message)
{
    juce::Logger::writeToLog("Message received: "+String(message[0])+"/"+String(message[1])+"/"+String(message[2]));
    BandMessage* _message = new BandMessage(new MemoryBlock(message));
    
    if (_message->getType() == REQUEST )
    {
        juce::Logger::writeToLog("type: REQUEST");
        switch (_message->getSection())
        {
            case 0x00:
                dispatchAppRequest(_message->getMessageNumber(), _message->getContent());
                break;
            case 0x01:
                dispatchGUIRequest(_message->getMessageNumber(), _message->getContent());
                break;
            case 0x02:
                dispatchLibraryRequest(_message->getMessageNumber(), _message->getContent(),&library);
                break;
            case 0x03:
                dispatchPlayerRequest(_message->getMessageNumber(), _message->getContent());
                break;
        }
        
    }
    
}

//-----------------------------------------------------------------------------------------------------------
// heritage MessageDispatcher (TODO)
//-----------------------------------------------------------------------------------------------------------
void MyBandUnit::dispatchAppRequest(int _messageNb, String _content)
{
    
}

void MyBandUnit::dispatchGUIRequest(int _messageNb, String _content)
{
    
}

void MyBandUnit::dispatchLibraryRequest(int _messageNb, String _content,TrackLibrary* _library)
{
    double position = 0;
    bool playing = (state == Playing)?true:false;
    
    switch (_messageNb)
    {
        case 0x01:
            break;
        case 0x02:
            break;
        case 0x03:
            break;
        case 0x04: // Selection d'un morceau
        {
            int id = _content.getHexValue32();
            _library->selectTrack(id);
            loadTransportSources();
            // load transport sources
            break;
        }
        case 0x05:
            if (playing)
            {
                position = getCurrentPositionTransportSources();
                stopTransportSources();
            }
            mixSource.removeAllInputs();
            //releaseResources();
            _library->getSelectedTrack()->setMusicianState(_content.getHexValue32(), true);
            loadTransportSources();
            if (playing)
            {
                setPositionTransportSources(position);
                start();
            }
            break;
        case 0x06:
            if (playing)
            {
                position = getCurrentPositionTransportSources();
                stopTransportSources();
            }
            mixSource.removeAllInputs();
            //releaseResources();
            _library->getSelectedTrack()->setMusicianState(_content.getHexValue32(), false);
            loadTransportSources();
            if (playing)
            {
                setPositionTransportSources(position);
                start();
            }
            break;
    }
}

void MyBandUnit::dispatchPlayerRequest(int _messageNb, String _content)
{
    switch (_messageNb)
    {
        case 0x01: // play
        {
            changeState (Starting);
            break;
        }
        case 0x02: // stop/pause
        {
            changeState(Stopping);
            break;
        }
    }
}
