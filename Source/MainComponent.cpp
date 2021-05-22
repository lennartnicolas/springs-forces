#include "MainComponent.h"



//==============================================================================
MainComponent::MainComponent()
{
    frameCounter = 0;
    eWidth = 10;
    k = 0.01;
    gravity = glm::vec2(0, 0.1);
    int gridSize = 7;
    spacing = 100;

    for(int i = 0; i < gridSize; i++)
    {
        for(int j = 0; j < gridSize; j++)
        {
            particles.push_back(std::make_shared<Particle>(i * spacing, j * spacing));
            
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
        
        if(j > gridSize)
        {
            springs.push_back(std::make_shared<Spring>(k, spacing, particles.at(j), particles.at(j - gridSize)));
        }
    }
    
    for(const auto& p : particles)
    {
        addAndMakeVisible(*p.get());
    }
    
    startTimerHz(60);
    setSize (600, 600);
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    g.setColour(juce::Colours::white);
    
    for(const auto& s : springs)
    {
        g.drawLine(s->p1->position.x, s->p1->position.y, s->p2->position.x, s->p2->position.y);
    }
}

void MainComponent::resized()
{
    for(const auto& p : particles)
    {
        p->setBounds(p->position.x - 25, p->position.y - 25, 50, 50);
    }
}

void MainComponent::update()
{
    for(const auto& s : springs)
    {
        s->update();
    }

    for(const auto& p : particles)
    {
        if(p->isMouseButtonDown())
        {
            p->position.x = getMouseXYRelative().getX();
            p->position.y = getMouseXYRelative().getY();
        }
        
        //p->applyForce(gravity);
        p->update();
        p->repaint();
    }
}


void MainComponent::timerCallback()
{
    update();
    frameCounter++;
    resized();
    repaint();
}
