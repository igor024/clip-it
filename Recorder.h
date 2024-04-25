#pragma once
#include <juce_core/juce_core.h>
#include <juce_dsp/juce_dsp.h>
#include <JuceHeader.h>

using namespace juce;
using namespace std;

class Recorder : public AudioIODeviceCallback {
    public:
    AudioDeviceManager deviceManager;
    Recorder()
    {
        cout << "Recorder initialized \n";
        deviceManager.initialise(2, 2, nullptr, true);
        deviceManager.addAudioCallback(this);
    }

    ~Recorder()
    {
        cout << "Recorder destructor\n";
        deviceManager.removeAudioCallback(this);
    }

    void audioDeviceIOCallback(const float** inputChannelData, int numInputChannels, float** outputChannelData, int numOutputChannels, int numSamples)
    {
        cout << "num input channels: " << numInputChannels << "\n";
        for (int channi = 0; channi < numOutputChannels; channi++)
        {
            for (int i = 0; i < numSamples; i++)
            {
                outputChannelData[channi][i] = inputChannelData[channi][i] * 1000; //gain
            }
        }
    }

    void audioDeviceAboutToStart(AudioIODevice* device)
    {
        cout << "start device:\nDevice : " << device->getName() << "\nbuffer size : " << device->getCurrentBufferSizeSamples()  << "\nsample rate : " << device->getCurrentSampleRate() << "\n";
    }

    void audioDeviceStopped()
    {
        cout << "audioDeviceStopped\n";
    }
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Recorder)
};
