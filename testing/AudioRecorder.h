/*
  ==============================================================================

   This file is part of the JUCE examples.
   Copyright (c) 2017 - ROLI Ltd.

   The code included in this file is provided under the terms of the ISC license
   http://www.isc.org/downloads/software-support-policy/isc-license. Permission
   To use, copy, modify, and/or distribute this software for any purpose with or
   without fee is hereby granted provided that the above copyright notice and
   this permission notice appear in all copies.

   THE SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES,
   WHETHER EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR
   PURPOSE, ARE DISCLAIMED.

  ==============================================================================
*/

/*******************************************************************************
 The block below describes the properties of this PIP. A PIP is a short snippet
 of code that can be read by the Projucer and used to generate a JUCE project.

 BEGIN_JUCE_PIP_METADATA

 name:             AudioRecorder
 version:          1.0.0
 vendor:           JUCE
 website:          https://gitlab.com/DamienAuvray/simpleaudiorecorder
 description:      Records audio to a file.

 dependencies:     juce_audio_basics, juce_audio_devices, juce_audio_formats,
                   juce_audio_processors, juce_audio_utils, juce_core,
                   juce_data_structures, juce_events, juce_graphics,
                   juce_gui_basics, juce_gui_extra
 exporters:        xcode_mac, vs2017, linux_make

 moduleFlags:      JUCE_STRICT_REFCOUNTEDPOINTER=1

 type:             Component
 mainClass:        AudioRecorder

 useLocalCopy:     1

 END_JUCE_PIP_METADATA

*******************************************************************************/

#pragma once

#include <JuceHeader.h>
#include "./DemoUtilities.h"
//==============================================================================
/** A simple class that acts as an AudioIODeviceCallback and writes the
	incoming audio data to a WAV file.
*/
class AudioRecorder : public AudioIODeviceCallback
{
public:
	AudioRecorder()
	{
		backgroundThread.startThread();
	}

	~AudioRecorder()
	{
		stop();
	}

	//==============================================================================
	void startRecording(const File& file)
	{
		stop();

		if (sampleRate > 0)
		{
			// Create an OutputStream to write to our destination file...
			file.deleteFile();

			if (auto fileStream = std::unique_ptr<FileOutputStream>(file.createOutputStream()))
			{
				// Now create a WAV writer object that writes to our output stream...
				WavAudioFormat wavFormat;

				if (auto writer = wavFormat.createWriterFor(fileStream.get(), sampleRate, 1, 16, {}, 0))
				{
					fileStream.release(); // (passes responsibility for deleting the stream to the writer object that is now using it)

					// Now we'll create one of these helper objects which will act as a FIFO buffer, and will
					// write the data to disk on our background thread.
					threadedWriter.reset(new AudioFormatWriter::ThreadedWriter(writer, backgroundThread, 32768));

					// Reset our recording thumbnail
					nextSampleNum = 0;

					// And now, swap over our active writer pointer so that the audio callback will start using it..
					const ScopedLock sl(writerLock);
					activeWriter = threadedWriter.get();
				}
			}
		}
	}

	void stop()
	{
		// First, clear this pointer to stop the audio callback from using our writer object..
		{
			const ScopedLock sl(writerLock);
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

	//==============================================================================
	void audioDeviceAboutToStart(AudioIODevice* device) override
	{
		sampleRate = device->getCurrentSampleRate();
	}

	void audioDeviceStopped() override
	{
		sampleRate = 0;
	}

	void audioDeviceIOCallback(const float** inputChannelData, int numInputChannels,
		float** outputChannelData, int numOutputChannels,
		int numSamples) override
	{
		const ScopedLock sl(writerLock);
		if (activeWriter.load() != nullptr)
		{
			activeWriter.load()->write(inputChannelData, numSamples);

			// Create an AudioBuffer to wrap our incoming data, note that this does no allocations or copies, it simply references our input data
			nextSampleNum += numSamples;
		}

		// We need to clear the output buffers, in case they're full of junk..
		for (int i = 0; i < numOutputChannels; ++i)
			if (outputChannelData[i] != nullptr)
				FloatVectorOperations::clear(outputChannelData[i], numSamples);

		numInputChannels;
	}

private:
	TimeSliceThread backgroundThread{ "Audio Recorder Thread" }; // the thread that will write our audio data to disk
	std::unique_ptr<AudioFormatWriter::ThreadedWriter> threadedWriter; // the FIFO used to buffer the incoming data
	double sampleRate = 0.0;
	int64 nextSampleNum = 0;

	CriticalSection writerLock;
	std::atomic<AudioFormatWriter::ThreadedWriter*> activeWriter{ nullptr };
};


//==============================================================================
class MainComponent : public Component,
                      private AudioIODeviceCallback
{
public:
    //==============================================================================
    MainComponent()
        : audioSetupComp (audioDeviceManager, 0, 256, 0, 256,
                          false, false, false, false)
    {
        audioDeviceManager.initialise (2, 2, nullptr, true, {}, nullptr);
        audioDeviceManager.addAudioCallback (this);

        addAndMakeVisible (audioSetupComp);

		addAndMakeVisible(explanationLabel);
		explanationLabel.setFont(Font(15.0f, Font::plain));
		explanationLabel.setJustificationType(Justification::topLeft);
		explanationLabel.setEditable(false, false, false);
		explanationLabel.setColour(TextEditor::textColourId, Colours::black);
		explanationLabel.setColour(TextEditor::backgroundColourId, Colour(0x00000000));

		addAndMakeVisible(recordButton);
		recordButton.setColour(TextButton::buttonColourId, Colour(0xffff5c5c));
		recordButton.setColour(TextButton::textColourOnId, Colours::black);

		recordButton.onClick = [this]
		{
			if (recorder.isRecording())
				stopRecording();
			else
				startRecording();
		};
		audioDeviceManager.addAudioCallback(&recorder);
		
		setSize(650, 450);
    }

    ~MainComponent()
    {
		audioDeviceManager.removeAudioCallback(&recorder);
    }

    //==============================================================================
    void resized() override
    {
		auto area = getLocalBounds();

		recordButton.setBounds(area.removeFromTop(36).reduced(8));
		explanationLabel.setBounds(area.removeFromTop(100).reduced(8));

		audioSetupComp.setBounds(area.removeFromTop(audioSetupComp.getHeight()));
    }

    //==============================================================================
    void audioDeviceIOCallback (const float** /*inputChannelData*/, int /*numInputChannels*/,
                                float** outputChannelData, int numOutputChannels,
                                int numSamples) override
    {
        // make buffer
        AudioBuffer<float> buffer (outputChannelData, numOutputChannels, numSamples);

        // clear it to silence
        buffer.clear();
    }

    void audioDeviceAboutToStart (AudioIODevice* ) override    { }

    void audioDeviceStopped() override {}

private:
    //==============================================================================

    //==============================================================================
    AudioDeviceManager audioDeviceManager;         // [3]
    AudioDeviceSelectorComponent audioSetupComp;   // [4]

	//--------------------------
	AudioRecorder recorder{ };

	Label explanationLabel{ {}, "This page demonstrates how to record a wave file from the live audio input..\n\n"
								 #if (JUCE_ANDROID || JUCE_IOS)
								  "After you are done with your recording you can share with other apps."
								 #else
								  "Pressing record will start recording a file in your \"Documents\" folder."
								 #endif
	};
	TextButton recordButton{ "Record" };
	File lastRecording;

	void startRecording()
	{
		if (!RuntimePermissions::isGranted(RuntimePermissions::writeExternalStorage))
		{
			SafePointer<MainComponent> safeThis(this);

			RuntimePermissions::request(RuntimePermissions::writeExternalStorage,
				[safeThis](bool granted) mutable
			{
				if (granted)
					safeThis->startRecording();
			});
			return;
		}

#if (JUCE_ANDROID || JUCE_IOS)
		auto parentDir = File::getSpecialLocation(File::tempDirectory);
#else
		auto parentDir = File::getSpecialLocation(File::userDocumentsDirectory);
#endif

		lastRecording = parentDir.getNonexistentChildFile("JUCE Demo Audio Recording", ".wav");

		recorder.startRecording(lastRecording);

		recordButton.setButtonText("Stop");

	}

	void stopRecording()
	{
		recorder.stop();

#if (JUCE_ANDROID || JUCE_IOS)
		SafePointer<AudioRecordingDemo> safeThis(this);
		File fileToShare = lastRecording;

		ContentSharer::getInstance()->shareFiles(Array<URL>({ URL(fileToShare) }),
			[safeThis, fileToShare](bool success, const String& error)
		{
			if (fileToShare.existsAsFile())
				fileToShare.deleteFile();

			if (!success && error.isNotEmpty())
			{
				NativeMessageBox::showMessageBoxAsync(AlertWindow::WarningIcon,
					"Sharing Error",
					error);
			}
		});
#endif

		lastRecording = File();
		recordButton.setButtonText("Record");
	}


	//--------------------------

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
