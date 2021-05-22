#pragma once

#include <JuceHeader.h>
#include "Particle.h"
#include "Spring.h"
#include <vector>
#include <glm.hpp>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::Component, private juce::Timer
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;
    void update();
    
private:
    //==============================================================================
    unsigned int frameCounter;
    float eWidth;
    float k;
    float spacing;
    glm::vec2 gravity;
    bool isClicked = false;
    
    std::vector<std::shared_ptr<Spring> > springs;
    std::vector<std::shared_ptr<Particle> > particles;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
