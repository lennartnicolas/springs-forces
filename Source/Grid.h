/*
  ==============================================================================

    Grid.h
    Created: 23 May 2021 7:02:24pm
    Author:  Lennart Krebs

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Particle.h"
#include "Spring.h"
#include <vector>

//==============================================================================
/*
*/
class Grid  : public juce::Component
{
public:
    Grid()
    {
        gravity = glm::vec2(0.0,0.1);
        applyGravity = false;
    }
    
    ~Grid() override
    {
    }
    
    void setGrid(int gSize, float springConstant, float localBounds, float offSet = 0)
    {
        particles.clear();
        springs.clear();

        k = springConstant;
        int gridSize = gSize;
        spacing = localBounds / (gridSize - 1);

        for(int i = 0; i < gridSize; i++)
        {
            for(int j = 0; j < gridSize; j++)
            {
                particles.push_back(std::make_shared<Particle>(i * spacing + offSet, j * spacing + offSet));
                
                if(i == 0)
                    particles.at(j)->setLock(true);
                else if (i == gridSize-1)
                    particles.at(j + gridSize * (gridSize-1))->setLock(true);
            }
            
            if(i != 0 && i != gridSize - 1)
            {
                particles.at(i * gridSize)->setLock(true);
                particles.at(i * gridSize + gridSize-1)->setLock(true);
            }
        }
        
        for(int j = 0; j < particles.size(); j++)
        {
            if(j % gridSize != 0)
            {
                springs.push_back(std::make_shared<Spring>(k, spacing, particles.at(j), particles.at(j - 1)));
            }
            
            if(j >= gridSize)
            {
                springs.push_back(std::make_shared<Spring>(k, spacing, particles.at(j), particles.at(j - gridSize)));
            }
        }
        
        for(const auto& p : particles)
        {
            addAndMakeVisible(*p.get());
        }
        
    }
    
    void setStiffness(float stiffness)
    {
        for(const auto& s : springs)
            s->_k = fmod(stiffness, 0.1f);
    }
    
    void setGravity(bool on)
    {
        applyGravity = on;
    }
    
    void updateGrid()
    {
        for(const auto& s : springs)
        {
            s->update();
        }

        for(const auto& p : particles)
        {
            if(p->isMouseButtonDown())
            {
                if(!p->isLocked)
                {
                    //p->setParticleVisible(true);
                    p->position.x = getMouseXYRelative().getX();
                    p->position.y = getMouseXYRelative().getY();
                }
            } else {
                    //p->setParticleVisible(false);
            }

            if(applyGravity)
                p->applyForce(gravity);
            
            p->update();
            p->repaint();
        }
    }
    
    
    //==============================================================================
    
    void paint (juce::Graphics& g) override
    {
        g.setColour(juce::Colours::orange);
        g.drawRect(getLocalBounds());
        g.setColour(juce::Colours::white);
        
        for(const auto& s : springs)
        {
            g.drawLine(s->p1->position.x, s->p1->position.y, s->p2->position.x, s->p2->position.y, 2.f);
        }
    }

    void resized() override
    {
        for(const auto& p : particles)
        {
            p->setBounds(p->position.x - 10, p->position.y - 10, 50, 50);
        }
    }
    
    //==============================================================================

    std::vector<std::shared_ptr<Spring> > springs;
    std::vector<std::shared_ptr<Particle> > particles;

private:
    
    bool applyGravity;
    glm::vec2 gravity;
    float k;
    float spacing;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Grid)
};
