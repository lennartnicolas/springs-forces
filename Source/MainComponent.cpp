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
    
    startTimerHz(60);
    setSize (600, 600);
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
    grid.setBounds(getLocalBounds().withSizeKeepingCentre(gridWidth + offSet, gridWidth + offSet));
}

void MainComponent::timerCallback()
{
    grid.updateGrid();
    grid.resized();
    grid.repaint();
}
