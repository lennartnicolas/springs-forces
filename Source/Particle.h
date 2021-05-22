/*
  ==============================================================================

    Particle.h
    Created: 21 May 2021 5:08:31pm
    Author:  Lennart Krebs

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <glm.hpp>


//==============================================================================
/*
*/
class Particle : public juce::Component
{
public:
    Particle()
    {
    }
    
    void paint (juce::Graphics& g) override
    {
        g.setColour(juce::Colours::white);
        //g.fillRect(0, 0, 40, 40);
    }
    
    void resized() override
    {
    }
    
//    void mouseEnter(const juce::MouseEvent& event) override
//    {
//        if(!isLocked)
//        {
//            position.x = getMouseXYRelative().getX();
//            position.y = getMouseXYRelative().getY();
//        }
//
//    }

    Particle(float x, float y)
    {
        acceleration = glm::vec2(0,0);
        velocity     = glm::vec2(0,0);
        position     = glm::vec2(x,y);
        mass = 0.3f;
        isLocked = false;
    }

    ~Particle()
    {
    }
    
    void applyForce(glm::vec2 force)
    {
        acceleration += force / mass;
    }

    void update()
    {
        if(!isLocked)
        {
            velocity *= 0.99f;
            velocity += acceleration;
            position += velocity;
            acceleration *= 0.0f;
        }
    }
    
    void setLock(bool lock)
    {
        isLocked = lock;
    }
    
    glm::vec2 acceleration;
    glm::vec2 velocity;
    glm::vec2 position;
    float mass;
    bool isLocked;
    
};
