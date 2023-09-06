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

//==============================================================================
class MyBandUnit   : public juce::AudioAppComponent,public juce::ChangeListener,public juce::Timer, public juce::InterprocessConnection
{
public:
    MyBandUnit();
    ~MyBandUnit() override;

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    void resized() override;
    void changeListenerCallback (juce::ChangeBroadcaster* source) override;
    void timerCallback() override;
    void updateLoopState (bool shouldLoop);
    void start();
    // heritage InterporcessConnection
    void connectionMade() override;
    void connectionLost() override;
    void messageReceived (const juce::MemoryBlock &message) override;

private:
    enum TransportState
    {
        Stopped,
        Starting,
        Playing,
        Stopping,
        Pausing,
        Paused
    };

    void changeState (TransportState newState);
    void loadTransportSources();
    void playButtonClicked();
    void stopButtonClicked();
    void loopButtonChanged();
    bool checkReaderSources();
    void stopTransportSources();
    void startTransportSources();
    void setPositionTransportSources(double d);
    bool isOneSourcePlaying();
    double getCurrentPositionTransportSources();
    
    void dispatchAppRequest(int _messageNb, String _content);
    void dispatchGUIRequest(int _messageNb, String _content);
    void dispatchLibraryRequest(int _messageNb, String _content, TrackLibrary* library);
    void dispatchPlayerRequest(int _messageNb, String _content);
    
    //==========================================================================
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource2;
    juce::OwnedArray<juce::AudioFormatReaderSource> readerSources = juce::OwnedArray<juce::AudioFormatReaderSource>();
    juce::OwnedArray<juce::AudioTransportSource> transportSources = juce::OwnedArray<juce::AudioTransportSource>();
    //juce::AudioTransportSource transportSource2;
    juce::MixerAudioSource mixSource;
    TransportState state;
    TrackLibrary library;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MyBandUnit)
};
