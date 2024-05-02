#include <juce_core/juce_core.h>
#include <iostream>
#include "AudioRecordingDemo.h"
#include <JuceHeader.h>

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
                    
                    auto setup = audioDeviceManager.getAudioDeviceSetup();
                    setup.inputDeviceName = "sof-hda-dsp, ; Direct hardware device without any conversions (3)";
                    audioDeviceManager.setAudioDeviceSetup (setup, true);
                    cout << "Success!\n";
                }
            }
        }

    AudioRecordingDemo recorder {audioDeviceManager};
   
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
        }
    }

    return 0;
}
