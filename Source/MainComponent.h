#pragma once

#include <JuceHeader.h>
#include "Grid.h"


//==============================================================================
class WavetableOscillator
{
public:
    WavetableOscillator (const juce::AudioSampleBuffer& wavetableToUse)
        : wavetable (wavetableToUse),
          tableSize (wavetable.getNumSamples() - 1)
    {
        jassert (wavetable.getNumChannels() == 1);
    }

    void setFrequency (float frequency, float sampleRate)
    {
        auto tableSizeOverSampleRate = (float) tableSize / sampleRate;
        tableDelta = frequency * tableSizeOverSampleRate;
    }

    forcedinline float getNextSample() noexcept
    {
        auto index0 = (unsigned int) currentIndex;
        auto index1 = index0 + 1;

        auto frac = currentIndex - (float) index0;

        auto* table = wavetable.getReadPointer (0);
        auto value0 = table[index0];
        auto value1 = table[index1];

        auto currentSample = value0 + frac * (value1 - value0);

        if ((currentIndex += tableDelta) > (float) tableSize)
            currentIndex -= (float) tableSize;

        return currentSample;
    }

private:
    const juce::AudioSampleBuffer& wavetable;
    const int tableSize;
    float currentIndex = 0.0f, tableDelta = 0.0f;
};

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent, private juce::Timer
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;
    
    //==============================================================================
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void releaseResources() override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo &bufferToFill) override;
    
    float map(float value, float start1, float stop1, float start2, float stop2);
    void updateOscillators();
    void createWavetable();
    
    
private:
    //==============================================================================
    Grid grid;
    int gridSize;
    float gridWidth;
    float offSet;
    float springConstant;
    
    juce::TextButton updateGridButton;
    juce::ToggleButton applyGravityButton;
    juce::Slider gridSizeSlider;
    juce::Slider stiffnessSlider;
    
    const unsigned int tableSize = 1 << 8;
    juce::AudioSampleBuffer waveTable;
    std::vector<WavetableOscillator*> oscillators;
    std::vector<int> relatedIndices;
    float currentSamplerate;
        
    //==============================================================================

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
