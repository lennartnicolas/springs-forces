/*
  ==============================================================================

    Spring.h
    Created: 21 May 2021 5:08:49pm
    Author:  Lennart Krebs

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <glm.hpp>
#include "Particle.h"

//==============================================================================
/*
*/
class Spring
{
public:
    Spring()
    {
    }

    Spring(float k, float restlth, std::shared_ptr<Particle> a, std::shared_ptr<Particle> b)
    {
        _k = k;
        restLength = restlth;
        p1 = a;
        p2 = b;
    }

    ~Spring()
    {
    }    
    
    void update()
    {
        glm::vec2 force = p2->position - p1->position;
        float x = glm::length(force) - restLength;
        force = glm::normalize(force);
        force *= _k * x;
        p1->applyForce(force);
        force *= -1.f;
        p2->applyForce(force);
    }
    
    std::shared_ptr<Particle> p1;
    std::shared_ptr<Particle> p2;
    float _k;
    float restLength;
};
