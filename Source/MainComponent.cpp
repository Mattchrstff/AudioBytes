// MainComponent.cpp
#include "MainComponent.h"

// Constructor: Setup sliders, labels, audio channels
MainComponent::MainComponent()
{
    // Configure gain slider
    gainSlider.setRange(1.0, 20.0, 0.1); // Set range from 1.0 to 20.0
    gainSlider.setValue(gain); // Initial value
    gainSlider.onValueChange = [this] { gain = gainSlider.getValue(); }; // Update variable when slider changes
    addAndMakeVisible(gainSlider); // Make slider visible

    gainLabel.setText("Gain", juce::dontSendNotification); // Set label text
    addAndMakeVisible(gainLabel); // Make label visible

    // Configure tone slider
    toneSlider.setRange(500.0, 20000.0, 1.0); // Hz range
    toneSlider.setValue(tone);
    toneSlider.onValueChange = [this] { tone = toneSlider.getValue(); };
    addAndMakeVisible(toneSlider);

    toneLabel.setText("Tone", juce::dontSendNotification);
    addAndMakeVisible(toneLabel);

    // Configure volume slider
    volumeSlider.setRange(0.0, 1.0, 0.01); // Volume from 0 to 1
    volumeSlider.setValue(volume);
    volumeSlider.onValueChange = [this] { volume = volumeSlider.getValue(); };
    addAndMakeVisible(volumeSlider);

    volumeLabel.setText("Volume", juce::dontSendNotification);
    addAndMakeVisible(volumeLabel);
    
    juce::AudioDeviceManager::AudioDeviceSetup currentAudioSetup;
    deviceManager.getAudioDeviceSetup (currentAudioSetup);
    currentAudioSetup.bufferSize = 100;
    deviceManager.setAudioDeviceSetup (currentAudioSetup, true);

    setSize (400, 200); // Set window size
    setAudioChannels (1, 2); // One input (mono), two outputs (stereo)
}

// Destructor: clean up audio
MainComponent::~MainComponent()
{
    shutdownAudio(); // Free audio resources
}

// Prepare filter and allocate resources
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate; // Set sample rate for filter
    spec.maximumBlockSize = static_cast<juce::uint32> (samplesPerBlockExpected); // Set block size
    spec.numChannels = 1; // Mono filter

    toneFilter.prepare(spec); // Initialize the filter with this spec
    
    
    reverbFilter.setSampleRate(tone);
}

// Called when audio stops
void MainComponent::releaseResources()
{
    // Nothing to release in this simple example
}

// Audio processing loop
void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    // Get input buffer (mic input assumed to be mono)
    auto* input = bufferToFill.buffer->getReadPointer(0);
    // Get output buffers for left/right
    auto* left = bufferToFill.buffer->getWritePointer(0);
    auto* right = bufferToFill.buffer->getNumChannels() > 1 ? bufferToFill.buffer->getWritePointer(1) : nullptr;

    // Update filter coefficients based on tone slider
    auto coeffs = juce::dsp::IIR::Coefficients<float>::makeLowPass(44100, tone);

    
    *toneFilter.coefficients = *coeffs;

    // Process each audio sample
    for (int i = 0; i < bufferToFill.numSamples; ++i)
    {
        reverbFilter.processMono(left, bufferToFill.numSamples);
        float dry = input[i]; // Read input sample
        float boosted = dry * gain; // Apply gain
        float clipped = std::clamp(boosted, -0.8f, 0.8f); // Hard clipping
        // Apply tone filter
        float output = clipped * volume; // Apply volume

        left[i] = output; // Write to left output
        if (right) right[i] = output; // Write to right output
    }
}

// Fill the background
void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black); // Black background
}

// Position sliders and labels
void MainComponent::resized()
{
    auto area = getLocalBounds().reduced(10); // Padding
    auto rowHeight = 40;

    gainLabel.setBounds(area.removeFromTop(20));
    gainSlider.setBounds(area.removeFromTop(rowHeight));

    toneLabel.setBounds(area.removeFromTop(20));
    toneSlider.setBounds(area.removeFromTop(rowHeight));

    volumeLabel.setBounds(area.removeFromTop(20));
    volumeSlider.setBounds(area.removeFromTop(rowHeight));
}
