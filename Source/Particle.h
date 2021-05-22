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
class Particle
{
public:
    Particle()
    {
    }
    
    Particle(float x, float y)
    {
        acceleration = glm::vec2(0,0);
        velocity     = glm::vec2(0,0);
        position     = glm::vec2(x,y);
        mass = 0.3f;
        isLocked = false;
        pArea = new juce::Rectangle<float>(position.x - 15 , position.y - 15, 30, 30);
    }

    ~Particle()
    {
        delete pArea;
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
            pArea->setPosition(position.x - 15, position.y - 15);
        }
    }
    
    juce::Rectangle<float>* pArea = nullptr;
    glm::vec2 acceleration;
    glm::vec2 velocity;
    glm::vec2 position;
    float mass;
    bool isLocked;
    
};
