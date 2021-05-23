#include "MainComponent.h"



//==============================================================================
MainComponent::MainComponent()
{
    gridWidth = 500;
    gridSize  = 6;
    springConstant = 0.01;
    offSet = 10;

    grid.setGrid(gridSize, springConstant, gridWidth, offSet / 2);
    addAndMakeVisible(grid);
    
    addAndMakeVisible(button);
    button.onClick = [this](){
        grid.setGrid(4, 0.04, gridWidth, offSet / 2);
    };
    
    startTimerHz(60);
    setSize (900, 600);
    
    setAudioChannels(0, 2);
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{

}

void MainComponent::resized()
{
    grid.setBounds((getWidth() / 3) + 50 - offSet, 50, gridWidth + offSet, gridWidth + offSet );
    button.setBounds(10, 10, 100, 30);
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
    angleDelta = 150.f / currentSamplerate * 2.f * M_PI;
}

void MainComponent::releaseResources()
{
    
}


void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo &bufferToFill)
{
    auto* leftBuffer = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
    auto* rightBuffer = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);
    
    for(int i = 0; i < bufferToFill.numSamples; i++)
    {
        auto sample = std::sin(currentAngle);
        currentAngle += angleDelta;
        leftBuffer[i] = sample * (grid.particles.at(7)->position.x / 1000);
        rightBuffer[i] = sample * (grid.particles.at(7)->position.x / 1000);
    }

    
}
