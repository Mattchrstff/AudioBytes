#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent
{
public:
    //==============================================================================
    MainComponent();

    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;
    void updateAngleDelta();

private:
    // UI
        juce::Slider gainSlider, toneSlider, volumeSlider, delaySlider;
        juce::Label gainLabel, toneLabel, volumeLabel, delayLabel;
    
        juce::dsp::ProcessSpec spec;

        float gain = 5.0f;
        float tone = 8000.0f;
        float volume = 0.8f;
        float delay = 0.0f;

        // Filter
//        juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> toneFilter;
        juce::dsp::IIR::Filter<float> toneFilter;
        juce::Reverb reverbFilter;
        juce::Reverb::Parameters reverbParams;
    
        juce::dsp::DelayLine<float> delayFilter{44100};
    
        

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
