#pragma once

#include <JuceHeader.h>
#include "Grid.h"

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
    
    
private:
    //==============================================================================
    Grid grid;
    int gridWidth;
    int gridSize;
    int offSet;
    float springConstant;
    juce::TextButton button;

    
    //==============================================================================
    float currentAngle;
    float angleDelta;
    float currentSamplerate;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
