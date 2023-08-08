/*
  ==============================================================================

    This file contains the basic startup code for a JUCE application.

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MyBandGUI.h"
#include "Monitor.h"
#include "ZeroConfManager.h"


//==============================================================================
/*
int main (int argc, char* argv[])
{

    auto args = juce::ArgumentList (argc, argv);

    // use 'args' to check if arguments exist, retrieve their values, etc.
    args.failIfOptionIsMissing ("required_arg");
    auto requiredValue = args.getValueForOption ("required_arg");

    if (args.contains ("optional_arg"))
    {
        auto optionalValue = args.getValueForOption ("optional_arg");

    return 0;
}*/



class Application    : public juce::JUCEApplication, public ZeroConfListener
{
public:
    //==============================================================================
    Application() = default;

    const juce::String getApplicationName() override       { return "MyBandUnit"; }
    const juce::String getApplicationVersion() override    { return "0.1.0"; }

    void initialise (const juce::String&) override
    {
        startZeroconf();
        mainWindow.reset (new MainWindow ("MyBand", &gui, *this));
    }

    void shutdown() override
    {
        mainWindow = nullptr;
        stopZeroconf();
    }
    
    void startZeroconf()
    {
        monitor.startMonitoring();
        
        zeroConfManager = std::unique_ptr<ZeroConfManager>(new ZeroConfManager(&monitor, this));
        zeroConfManager->browseService("_mybandunit._udp");
    }
    
    void stopZeroconf()
    {
        // arreter le monitor
        // le serveur de socket
    }
    
    // heritage ZeroConf Listener
    void handleZeroConfUpdate(juce::OwnedArray<ZeroConfService, juce::CriticalSection> *serviceList) override
    {
        juce::Logger::writeToLog("notified");
        for (int i = 0; i < serviceList->size(); i++) {
            
            ZeroConfService *service = serviceList->getUnchecked(i);
            juce::String status = service->status == ZeroConfService::ResultStatus::queryResult ? "final" : "unfinished";
            juce::Logger::writeToLog(juce::String::formatted("Found service %s %s %s:%d %s",service->getRegType().toRawUTF8(), service->getServiceName().toRawUTF8(),service->ip.toRawUTF8(), service->getPort(), status.toRawUTF8() ));
            
            gui.ConnectToUnit(service->ip.toRawUTF8(), service->getPort(), 3000);
        }
    };
    
    
private:
    class MainWindow    : public juce::DocumentWindow
    {
    public:
        MainWindow (const juce::String& name, juce::Component* c, juce::JUCEApplication& a)
            : DocumentWindow (name, juce::Desktop::getInstance().getDefaultLookAndFeel()
                                                                .findColour (ResizableWindow::backgroundColourId),
                              juce::DocumentWindow::allButtons),
              app (a)
        {
            setUsingNativeTitleBar (true);
            setContentOwned (c, true);

           #if JUCE_ANDROID || JUCE_IOS
            setFullScreen (true);
           #else
            setResizable (true, false);
            setResizeLimits (300, 250, 10000, 10000);
            centreWithSize (getWidth(), getHeight());
           #endif

            setVisible (true);
        }

        void closeButtonPressed() override
        {
            app.systemRequestedQuit();
        }

        
        
    private:
        JUCEApplication& app;

        //==============================================================================
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };

    class ZeroConfObserver : public ZeroConfListener
    {
        public:
                
                ZeroConfObserver(){
                    
                };
                ~ZeroConfObserver(){
                    
                };
                
                void handleZeroConfUpdate(juce::OwnedArray<ZeroConfService, juce::CriticalSection> *serviceList)
                {
                    juce::Logger::writeToLog("notified");
                    for (int i = 0; i < serviceList->size(); i++) {
                        
                        ZeroConfService *service = serviceList->getUnchecked(i);
                        juce::String status = service->status == ZeroConfService::ResultStatus::queryResult ? "final" : "unfinished";
                        juce::Logger::writeToLog(juce::String::formatted("Found service %s %s %s:%d %s",service->getRegType().toRawUTF8(), service->getServiceName().toRawUTF8(),service->ip.toRawUTF8(), service->getPort(), status.toRawUTF8() ));
                    }
                };

        private:
        
    };
    std::unique_ptr<MainWindow> mainWindow;
    Monitor monitor;
    std::unique_ptr<ZeroConfManager> zeroConfManager;
    std::unique_ptr<ZeroConfService> service;
    MyBandGUI gui;
    
};

//==============================================================================
START_JUCE_APPLICATION (Application)
