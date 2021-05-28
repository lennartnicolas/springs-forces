#include "MainComponent.h"



//==============================================================================
MainComponent::MainComponent()
{
    gridWidth = 500;
    gridSize  = 6;
    springConstant = 0.01;
    offSet = 10;
    grid.setGrid(gridSize, springConstant, gridWidth, offSet / 2);
    addAndMakeVisible(&grid);
    
    gridSizeSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    gridSizeSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxRight, false, 50, 20);
    gridSizeSlider.setColour(juce::Slider::trackColourId, juce::Colours::transparentBlack);
    gridSizeSlider.setSliderSnapsToMousePosition(false);
    gridSizeSlider.setTextValueSuffix(" columns");
    gridSizeSlider.setRange(juce::Range<double>(3,15), 1);
    gridSizeSlider.setValue(gridSize);
    gridSizeSlider.onValueChange = [this](){
        gridSize = gridSizeSlider.getValue();
    };
    addAndMakeVisible(&gridSizeSlider);
    
    stiffnessSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    stiffnessSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxRight, false, 50, 20);
    stiffnessSlider.setSliderSnapsToMousePosition(false);
    stiffnessSlider.setColour(juce::Slider::trackColourId, juce::Colours::transparentBlack);
    stiffnessSlider.setTextValueSuffix(" % springy");
    stiffnessSlider.setValue(springConstant * 1000);
    stiffnessSlider.setRange(juce::Range<double>(1,100), 1);
    stiffnessSlider.onValueChange = [this](){
        springConstant = (float) (stiffnessSlider.getValue() / 1010);
    };
    addAndMakeVisible(&stiffnessSlider);
    
    updateGridButton.setButtonText("Update Grid");
    updateGridButton.onClick = [this](){
        grid.setGrid(gridSize, 0.01, gridWidth, offSet / 2);
        grid.setStiffness(springConstant);
        updateOscillators();
    };
    addAndMakeVisible(&updateGridButton);
    
    applyGravityButton.setButtonText("Add Gravity");
    applyGravityButton.onStateChange = [this](){
        grid.setGravity(applyGravityButton.getToggleState());
    };
    addAndMakeVisible(&applyGravityButton);
    
    
    createWavetable();
    
    
    startTimerHz(30);
    setSize (900, 600);
    setAudioChannels(0, 2);
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
}

void MainComponent::resized()
{
    grid.setBounds((getWidth() / 3) + 50 - offSet, (getHeight() - gridWidth) / 2 - offSet / 2, gridWidth + offSet, gridWidth + offSet );
    
    auto xOff = 65;
    auto yOff = 65;
    
    gridSizeSlider.setBounds(0 + xOff, 0 + yOff, 100, 30);
    stiffnessSlider.setBounds(0 + xOff, 40 + yOff, 100, 30);
   
    updateGridButton.setBounds(110 + xOff, 40 + yOff, 100, 30);
    applyGravityButton.setBounds(110 + xOff, 0 + yOff, 100, 30);
}

void MainComponent::timerCallback()
{
    grid.updateGrid();
    grid.resized();
    grid.repaint();
}

//==============================================================================
void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    currentSamplerate = sampleRate;
    updateOscillators();
}

void MainComponent::updateOscillators()
{
    oscillators.clear();
    relatedIndices.clear();
    pSmoother.clear();
    
    for(int i = 0; i < grid.particles.size(); i++)
    {
        if(!grid.particles.at(i)->isLocked)
        {
            auto* osc = new WavetableOscillator(waveTable);
            osc->setFrequency((i+1) * 30, currentSamplerate);
            oscillators.push_back(osc);
            relatedIndices.push_back(i);
            auto level = fabs(roundf((grid.particles.at(i)->position.x / grid.particles.at(i)->initPos.x - 1.f) * 1000.f) / 1000.f);
            pSmoother.push_back(juce::SmoothedValue<float>(level));
            pSmoother.back().reset(currentSamplerate, 0.01);
        }
    }
}

void MainComponent::releaseResources()
{
    
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo &bufferToFill)
{
    auto* leftBuffer = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
    auto* rightBuffer = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);
    
    bufferToFill.clearActiveBufferRegion();
    
    for(int i = 0; i < oscillators.size(); i++)
    {
        auto particle = grid.particles.at(relatedIndices.at(i));
        auto level = fabs(roundf((particle->position.x / particle->initPos.x - 1.f) * 1000.f) / 1000.f);
        
        pSmoother.at(i).setTargetValue(level);
        
        for(int sample = 0; sample < bufferToFill.numSamples; sample++)
        {
            auto levelSample = oscillators.at(i)->getNextSample();
            leftBuffer[sample] += levelSample * pSmoother.at(i).getNextValue();
            rightBuffer[sample] += levelSample * pSmoother.at(i).getNextValue();
        }
    }


}

float MainComponent::map(float value, float start1, float stop1, float start2, float stop2)
{
    return start2 + (stop2 - start2) * ((value - start1) / (stop1 - start1));
}

void MainComponent::createWavetable()
{
    waveTable.setSize(1, (int) tableSize + 1);
    waveTable.clear();
    
    auto* tableSamples = waveTable.getWritePointer(0);
    auto angleDelta = M_PI * 2 / (float) (tableSize - 1);
    auto currentAngle = 0.f;
    
    for(int i = 0; i < tableSize; i++)
    {
        auto sample = std::sin(currentAngle);
        tableSamples[i] = sample * 0.5f;
        currentAngle += angleDelta;
    }
    
    tableSamples[tableSize] = tableSamples[0];
}

