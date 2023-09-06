/*
  ==============================================================================

    AudioGUI.cpp
    Created: 27 Jul 2023 5:00:59pm
    Author:  Frédéric FELTEN

  ==============================================================================
*/

#include "MyBandGUI.h"
#include "../../Common/BandMessage.h"

MyBandGUI::MyBandGUI()
        : state (Stopped)
{
    refreshList();
    listTracks.setTextWhenNothingSelected ("Choisissez un morceau ...");
    listTracks.onChange = ([this]{
        juce::Logger::writeToLog(String(listTracks.getSelectedId()));
        selectTrack(listTracks.getSelectedId()-1);
    });
    addAndMakeVisible(listTracks);
    
    addAndMakeVisible (&addButton);
    addButton.setButtonText ("+");
    addButton.onClick = [this] {}; //playButtonClicked(); };
    addButton.setEnabled (true);

    addAndMakeVisible (&infoButton);
    infoButton.setButtonText ("i");
        infoButton.onClick = [this] {}; //stopButtonClicked(); };
    infoButton.setEnabled (false);

    addAndMakeVisible (&stage);
    
    addAndMakeVisible(playButton);
    playButton.setButtonText(String(CharPointer_UTF8("\u25B6")));
    playButton.onClick = [this] { playButtonClicked(); };
    playButton.setEnabled(true);
    
    addAndMakeVisible(stopButton);
    stopButton.setButtonText(String(CharPointer_UTF8("\u25B0")));
    stopButton.onClick = [this] { stopButtonClicked(); };
    stopButton.setEnabled(false);
    
    //addAndMakeVisible (&loopingToggle);
    //loopingToggle.setButtonText ("Loop");
    //loopingToggle.onClick = [this] { loopButtonChanged(); };

    //addAndMakeVisible (&currentPositionLabel);
    currentPositionLabel.setText ("Stopped", juce::dontSendNotification);
    

    setSize (844, 390);

    //formatManager.registerBasicFormats();
    //transportSource.addChangeListener (this);

    //setAudioChannels (2, 2);
    //startTimer (20);
}

MyBandGUI::~MyBandGUI()
{
        //shutdownAudio();
}


void MyBandGUI::selectTrack (int id)
{
    stage.refreshTrack(library.getTrack(id));
    
    BandMessage* _message = new BandMessage(REQUEST, LIBRARY, 0x04);
    _message->setContent(String(id));
    
    sendMessage(*_message->getRawMsg());
    
}

void MyBandGUI::handleCommandMessage(int commandId)
{
    juce::Logger::writeToLog("Command:"+String(commandId));
    juce::Logger::writeToLog("Command:"+String(commandId >>24));
    juce::Logger::writeToLog("Id     :"+String( (commandId&0x00FFFF00) >>8));
    juce::Logger::writeToLog("data   :"+String( commandId&0x00000001));
    switch (commandId >> 24) {
        case 0:
            BandMessage* _message = new BandMessage(REQUEST, LIBRARY, stage.getMusicianState((commandId&0x00FFFF00)>>8)?0x05:0x06);
            _message->addContent(String((commandId&0x00FFFF00)>>8));
            sendMessage(*_message->getRawMsg());
            break;
    }
}

void MyBandGUI::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    //mixSource.prepareToPlay (samplesPerBlockExpected, sampleRate);
}

void MyBandGUI::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    if (!checkReaderSources())
    {
        bufferToFill.clearActiveBufferRegion();
        return;
    }

    //mixSource.getNextAudioBlock (bufferToFill);
}

    void MyBandGUI::releaseResources()
    {
        //mixSource.releaseResources();
    }

    void MyBandGUI::resized()
    {
        listTracks.setBounds (10, 10,  getWidth() - 90, 20);
        infoButton          .setBounds (getWidth() - 70, 10,  20, 20);
        addButton          .setBounds (getWidth() - 40, 10,  20, 20);
        stage       .setBounds (0, 50, 844, 300);
        playButton.setBounds(20,350, 30, 30);
        stopButton.setBounds(60,350, 30, 30);
        //currentPositionLabel.setBounds (10, 130, getWidth() - 20, 20);
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
        /*if (readerSource.get() != nullptr)
            readerSource->setLooping (shouldLoop);*/
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

    void MyBandGUI::libraryButtonClicked()
    {
        //libraryModal->Show();
        /*
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
            }
        });*/
    }

    void MyBandGUI::playButtonClicked()
    {
        //updateLoopState (loopingToggle.getToggleState());
        changeState (Starting);
        BandMessage* _message = new BandMessage(REQUEST, PLAYER, 0x01);
        
        sendMessage(*_message->getRawMsg());
    }

    void MyBandGUI::stopButtonClicked()
    {
        changeState (Stopping);
        BandMessage* _message = new BandMessage(REQUEST, PLAYER, 0x02);
        
        sendMessage(*_message->getRawMsg());
    }

    void MyBandGUI::loopButtonChanged()
    {
        //updateLoopState (loopingToggle.getToggleState());
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
        /*
        for(int i=0; i<transportSources.size(); i++)
        {
            transportSources[i]->stop();
        }*/
    }
    
    void MyBandGUI::startTransportSources()
    {
        /*
        for(int i=0; i<transportSources.size(); i++)
        {
            transportSources[i]->start();
        }*/
    }
    
    void MyBandGUI::setPositionTransportSources(double d)
    {
        /*
        for(int i=0; i<transportSources.size(); i++)
        {
            transportSources[i]->setPosition(d);
        }*/
    }
    
    bool MyBandGUI::isOneSourcePlaying()
    {
        /*
        for(int i=0; i<transportSources.size(); i++)
        {
            if ( transportSources[i]->isPlaying())
            {
                return true;
            }
        }*/
        
        return false;
    }
    
    double MyBandGUI::getCurrentPositionTransportSources()
    {
        /*
        for(int i=0; i<transportSources.size(); i++)
        {
            if ( transportSources[i]->isPlaying())
            {
                return  transportSources[i]->getCurrentPosition();
            }
        }*/
        
        return 0;
    }

//Combobox tracks
void MyBandGUI::refreshList()
{
    listTracks.clear();
    for(int i =0;i<library.getSize();i++)
    {
        const String text = String((i+1)) + " - " + library.getTrack(i)->getTitle() + " - " + library.getTrack(i)->getAuthor();
        listTracks.addItem(text, i+1);
    }
}

    // heritage InterporcessConnection
    void MyBandGUI::connectionMade()
    {
        juce::Logger::writeToLog("Connected to unit");
        
    }

    void MyBandGUI::connectionLost()
    {
        juce::Logger::writeToLog("Disconnected from unit");
        
        // nettoyage des contextes
        listTracks.clear();
    }

    void MyBandGUI::messageReceived (const MemoryBlock &message)
    {
        juce::Logger::writeToLog("Message received");
        BandMessage* _message = new BandMessage(new MemoryBlock(message));
        
        if (_message->getType() == REQUEST )
        {
            juce::Logger::writeToLog("type: REQUEST");
            
        }
        else
        {
            juce::Logger::writeToLog("type: NOTFICATION");
            switch(_message->getSection())
            {
            
                case LIBRARY:
                {
                    String _content = _message->getContent();
                    library.LoadLibrary(&_content);
                    refreshList();
                    juce::Logger::writeToLog(_message->getContent());
                    break;
                }
                case PLAYER:
                {
                    String _content = _message->getContent();
                    switch (_content.getHexValue32()) {
                        case 1:
                            changeState(Playing);
                            break;
                            
                        case 2:
                            changeState(Stopped);
                            break;
                            
                        default:
                            break;
                    }
                }
                    
            }
        }
                
    }

void MyBandGUI::ConnectToUnit(const String &hostName, int portNumber, int timeOutMillisecs){
    connectToSocket(hostName, portNumber, timeOutMillisecs);
}

