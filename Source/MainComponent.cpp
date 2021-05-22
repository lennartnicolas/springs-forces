#include "MainComponent.h"



//==============================================================================
MainComponent::MainComponent()
{


    frameCounter = 0;
    eWidth = 10;
    k = 0.01;
    setSize (600, 600);
    gravity = glm::vec2(0, 0.1);
    
    int gridSize = 7;
    spacing = 100;

    for(int i = 0; i < gridSize; i++)
    {
        for(int j = 0; j < gridSize; j++)
        {
            particles.push_back(std::make_shared<Particle>(i * spacing, j * spacing));
            
            if(i == 0)
                particles.at(j)->isLocked = true;
            else if (i == gridSize-1)
                particles.at(j + gridSize * (gridSize-1))->isLocked = true;
        }
        
        if(i != 0 && i != gridSize - 1)
        {
            particles.at(i * gridSize)->isLocked = true;
            particles.at(i * gridSize + gridSize-1)->isLocked = true;
        }
    }
    
    for(int j = 0; j < particles.size(); j++)
    {
        if(j % gridSize != 0)
        {
            springs.push_back(std::make_shared<Spring>(k, spacing, particles.at(j), particles.at(j - 1)));
        }
        
        if(j > gridSize)
        {
            springs.push_back(std::make_shared<Spring>(k, spacing, particles.at(j), particles.at(j - gridSize)));
        }
    }
    
    startTimerHz(60);
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    g.setColour(juce::Colours::white);
    
    for(const auto& p : particles)
    {
        if(!p->isLocked)
            g.fillEllipse(p->position.x - eWidth / 2, p->position.y - eWidth / 2, eWidth, eWidth);
        if(isClicked)
            g.setColour(juce::Colours::red);
        g.drawRect(*(p->pArea));
    }

    
    g.setColour(juce::Colours::white);
    for(const auto& s : springs)
    {
        g.drawLine(s->p1->position.x, s->p1->position.y, s->p2->position.x, s->p2->position.y);
    }

    
}

void MainComponent::resized()
{
    
}

void MainComponent::update()
{
    
    for(const auto& s : springs)
    {
        s->update();
    }

    for(const auto& p : particles)
    {
       // p->applyForce(gravity);
        p->update();
        
        if(p->pArea->contains(getMouseXYRelative().getX(), getMouseXYRelative().getY()))
        {
            if(!p->isLocked)
            {
                if(isMouseButtonDown())
                {
                    p->position.x = getMouseXYRelative().getX();
                    p->position.y = getMouseXYRelative().getY();
                }

            }
        }
    }

}


void MainComponent::timerCallback()
{
    update();
    frameCounter++;
    repaint();
}
