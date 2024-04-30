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

    AudioRecordingDemo recorder;
   
    cout << "Hotkeys:\nAlt+Z: Clip it\nQ: Exit\n";
    while(auto c = getchar()) {
        if(c == 'Q') {
            cout << "done.\n";
            break;
        }
        /*if(c == 'R') {    
            lastRecording = parentDir.getNonexistentChildFile ("JUCE Demo Audio Recording", ".wav");

            recorder.startRecording (lastRecording);
        }
        if(c == 'S' {
            recorder.stop();
            chooser.launchAsync (  FileBrowserComponent::saveMode
                | FileBrowserComponent::canSelectFiles
                | FileBrowserComponent::warnAboutOverwriting,
                [this] (const FileChooser& c)
                {
                    if (FileInputStream inputStream (lastRecording); inputStream.openedOk())
                    if (const auto outputStream = makeOutputStream (c.getURLResult()))
                        outputStream->writeFromInputStream (inputStream, -1);

                    recordButton.setButtonText ("Record");
                    recordingThumbnail.setDisplayFullThumbnail (true);
                });
        })*/
    }

    return 0;
}
