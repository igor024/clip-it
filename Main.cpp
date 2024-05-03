#include <juce_core/juce_core.h>
#include "Recorder.h"
#include <JuceHeader.h>
#include "Clipper.h"

#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <cstdio>


using namespace std;
int main (int argc, char* argv[])
{

    cout <<"\nstart program\n";
    juce::ignoreUnused (argc, argv);
    
    /*AudioFormatManager formatManager;
    AudioThumbnailCache thumbnailCache  { 10 };
    AudioThumbnail thumbnail            { 512, formatManager, thumbnailCache };
    Recorder rec{thumbnail};
    File lastRecording;
    FileChooser chooser { "Output file...", File::getCurrentWorkingDirectory().getChildFile ("recording.wav"), "*.wav" };
    */
    AudioDeviceManager audioDeviceManager;

   auto& deviceTypes = audioDeviceManager.getAvailableDeviceTypes();
        
        for (auto type : deviceTypes)
        {
            auto deviceNames (type->getDeviceNames());
            
            for (auto device : deviceNames)
            {
                cout << "device found:" << device << "\n";
                if(device == "sof-hda-dsp, ; Direct hardware device without any conversions (3)") {
                    cout << "have a go!\n";
                    
                    /*// Set up your desired audio device parameters
                    juce::AudioDeviceManager::AudioDeviceSetup setup;
                    setup.inputDeviceName = "sof-hda-dsp, ; Direct hardware device without any conversions (3)"; // Specify the name of your input device
                    setup.useDefaultInputChannels = false;
                    setup.inputChannels = 1; // Set the number of input channels you want to use
                    //setup.inputChannels.set(0, true); // Enable the first input channel, adjust index if needed

                    // Set the audio device setup
                    audioDeviceManager.setAudioDeviceSetup(setup, true);*/
                    
                    auto setup = audioDeviceManager.getAudioDeviceSetup();
                    setup.inputDeviceName = device;

                    const String& devName = "sof*3*";
                    
                    //const juce::AudioDeviceManager::AudioDeviceSetup* ref = &setup;
                    audioDeviceManager.initialise (128, 128, nullptr, true, devName, nullptr);
                    
                    //auto res = audioDeviceManager.setAudioDeviceSetup (setup, true);
                    //cout << "Success!\n";
                    //cout << "Res: " << res << "\n";                    

                }
            }
        }

    Recorder recorder {audioDeviceManager};

    cout << "\nCurrent device: " << audioDeviceManager.getCurrentAudioDevice()->getName() << "\n";
   
    cout << "Hotkeys:\nAlt+Z: Clip it\nQ: Exit\n";
    while(auto c = getchar()) {
        if(c == 'Q') {
            cout << "done.\n";
            break;
        }
        if(c == 'R') {    
            recorder.startRecording();
        }
        if(c == 'S') {
            recorder.stopRecording();
            Clipper::clip();
        }
    }

    return 0;
}
