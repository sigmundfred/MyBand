/*
  ==============================================================================

    AudioUnit.h
    Created: 27 Jul 2023 5:00:59pm
    Author:  Frédéric FELTEN

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "../../Common/TrackLibrary.h"
#include "Stage.h"

using namespace juce;

//==============================================================================
class MyBandGUI   : public juce::AudioAppComponent,public juce::ChangeListener,public juce::Timer, public InterprocessConnection
{
public:
    MyBandGUI();
    ~MyBandGUI() override;

    void selectTrack(int id);
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    void resized() override;
    void changeListenerCallback (juce::ChangeBroadcaster* source) override;
    void timerCallback() override;
    void updateLoopState (bool shouldLoop);
    void start();
    
    void ConnectToUnit(const String &hostName, int portNumber, int timeOutMillisecs);
    // heritage InterporcessConnection
    void connectionMade() override;
    void connectionLost() override;
    void messageReceived (const MemoryBlock &message) override;

private:
    enum TransportState
    {
        Stopped,
        Starting,
        Playing,
        Stopping
    };

    void changeState (TransportState newState);
    void libraryButtonClicked();
    void playButtonClicked();
    void stopButtonClicked();
    void loopButtonChanged();
    bool checkReaderSources();
    void stopTransportSources();
    void startTransportSources();
    void setPositionTransportSources(double d);
    bool isOneSourcePlaying();
    double getCurrentPositionTransportSources();
    void handleCommandMessage(int commandId) override;
    
    void refreshList();
    
    //==========================================================================
    //LibraryWindow* libraryModal;
    TrackLibrary library;
    ComboBox listTracks;
    juce::TextButton libraryButton;
    juce::TextButton addButton;
    juce::TextButton infoButton;
    juce::TextButton playButton;
    juce::TextButton stopButton;
    Stage stage;
    //juce::ToggleButton loopingToggle;
    juce::Label currentPositionLabel;
    //juce::Label song1;
    //juce::Label song2;

    //std::unique_ptr<juce::FileChooser> chooser;

    //juce::AudioFormatManager formatManager;
    //std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    //std::unique_ptr<juce::AudioFormatReaderSource> readerSource2;
    //juce::OwnedArray<juce::AudioFormatReaderSource> readerSources = juce::OwnedArray<juce::AudioFormatReaderSource>();
    //juce::OwnedArray<juce::AudioTransportSource> transportSources = juce::OwnedArray<juce::AudioTransportSource>();
    //juce::AudioTransportSource transportSource2;
    //juce::MixerAudioSource mixSource;
    TransportState state;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MyBandGUI)
};
