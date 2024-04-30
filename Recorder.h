/*
#pragma once
#include <juce_core/juce_core.h>
#include <juce_dsp/juce_dsp.h>
#include <JuceHeader.h>

using namespace juce;
using namespace std;

class Recorder : public AudioIODeviceCallback {
public:
    AudioDeviceManager deviceManager;
    Recorder(AudioThumbnail& thumbnailToUpdate) : thumbnail (thumbnailToUpdate)
    {
        cout << "Recorder initialized \n";
        deviceManager.initialise(2, 2, nullptr, true);
        deviceManager.addAudioCallback(this);
        backgroundThread.startThread();
    }

    ~Recorder()
    {
        cout << "Recorder destructor\n";
        deviceManager.removeAudioCallback(this);
    }

    //helper methods

    void startRecording (const File& file)
    {
        stop();

        if (sampleRate > 0)
        {
            // Create an OutputStream to write to our destination file...
            file.deleteFile();

            if (auto fileStream = std::unique_ptr<FileOutputStream> (file.createOutputStream()))
            {
                // Now create a WAV writer object that writes to our output stream...
                WavAudioFormat wavFormat;

                if (auto writer = wavFormat.createWriterFor (fileStream.get(), sampleRate, 1, 16, {}, 0))
                {
                    fileStream.release(); // (passes responsibility for deleting the stream to the writer object that is now using it)

                    // Now we'll create one of these helper objects which will act as a FIFO buffer, and will
                    // write the data to disk on our background thread.
                    threadedWriter.reset (new AudioFormatWriter::ThreadedWriter (writer, backgroundThread, 32768));

                    // Reset our recording thumbnail
                    nextSampleNum = 0;

                    // And now, swap over our active writer pointer so that the audio callback will start using it..
                    const ScopedLock sl (writerLock);
                    activeWriter = threadedWriter.get();
                }
            }
        }
    }

    
    void stop()
    {
        // First, clear this pointer to stop the audio callback from using our writer object..
        {
            const ScopedLock sl (writerLock);
            activeWriter = nullptr;
        }

        // Now we can delete the writer object. It's done in this order because the deletion could
        // take a little time while remaining data gets flushed to disk, so it's best to avoid blocking
        // the audio callback while this happens.
        threadedWriter.reset();
    }

    bool isRecording() const
    {
        return activeWriter.load() != nullptr;
    }

    void audioDeviceAboutToStart (AudioIODevice* device)
    {cx 
        sampleRate = device->getCurrentSampleRate();
    }

    void audioDeviceStopped()
    {
        sampleRate = 0;
    }

    //class methods
    void audioDeviceIOCallbackWithContext (const float* const* inputChannelData, int numInputChannels,
                                           float* const* outputChannelData, int numOutputChannels,
                                           int numSamples, const AudioIODeviceCallbackContext& context) override {
        cout << "num input channels: " << numInputChannels << "\n";
        /*for (int channi = 0; channi < numOutputChannels; channi++)
        {
            for (int i = 0; i < numSamples; i++)
            {
                outputChannelData[channi][i] = inputChannelData[channi][i] * 1000; //gain
            }
        }

        ignoreUnused (context);

        const ScopedLock sl (writerLock);

        if (activeWriter.load() != nullptr && numInputChannels >= thumbnail.getNumChannels()) {
            activeWriter.load()->write (inputChannelData, numSamples);

            // Create an AudioBuffer to wrap our incoming data, note that this does no allocations or copies, it simply references our input data
            AudioBuffer<float> buffer (const_cast<float**> (inputChannelData), thumbnail.getNumChannels(), numSamples);
            thumbnail.addBlock (nextSampleNum, buffer, 0, numSamples);
            nextSampleNum += numSamples;
        }

        // We need to clear the output buffers, in case they're full of junk..
        for (int i = 0; i < numOutputChannels; ++i) {
            if (outputChannelData[i] != nullptr) {
                FloatVectorOperations::clear (outputChannelData[i], numSamples);
            }
        }
    }

    void audioDeviceAboutToStart (AudioIODevice* device) override
    {
        sampleRate = device->getCurrentSampleRate();
        cout << "start device:\nDevice : " << device->getName() << "\nbuffer size : " << device->getCurrentBufferSizeSamples()  << "\nsample rate : " << device->getCurrentSampleRate() << "\n";
    }


    void audioDeviceStopped()
    {
        sampleRate = 0;
        cout << "audioDeviceStopped\n";
    }
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Recorder)

private:
    AudioThumbnail& thumbnail;
    TimeSliceThread backgroundThread { "Audio Recorder Thread" }; // the thread that will write our audio data to disk
    std::unique_ptr<AudioFormatWriter::ThreadedWriter> threadedWriter; // the FIFO used to buffer the incoming data
    double sampleRate = 0.0;
    int64 nextSampleNum = 0;

    CriticalSection writerLock;
    std::atomic<AudioFormatWriter::ThreadedWriter*> activeWriter { nullptr };
};
*/