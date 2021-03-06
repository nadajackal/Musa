//
//  NPC.cpp
//  Musa
//
//  Created by Ian Wilson on 2/13/16.
//  Copyright (c) 2016 APM. All rights reserved.
//

#include "NPC.hpp"
#include "Random.hpp"
#include "Utility.hpp"
#include "CommandQueue.hpp"
#include "DataTables.hpp"

#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/RenderStates.hpp"

#include <cmath>
#include <iostream>

using namespace std::placeholders;


namespace
{
    const std::vector<NPCData> Table = initializeNPCData();
}

NPC::NPC(Type type, sf::Texture texture, const FontHolder& fonts)
: Entity(Table[type].hitpoints)
, mType(type)
, mAnimatedSprite(sf::seconds(0.2), true, false)
, mCurrentAnimation(nullptr)
, mTexture(texture)
, mTravelledDistance(0.f)
, mDirectionIndex(0)
, mCategory(3)
{
}

void NPC::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mAnimatedSprite, states);
}

unsigned int NPC::getCategory() const
{
    return mCategory;
}

void NPC::updateCurrent(sf::Time dt, CommandQueue& commands)
{
    updateMoveAnimation(dt);
    Entity::updateCurrent(dt, commands);
}

sf::FloatRect NPC::getBoundingRect() const
{
    return getWorldTransform().transformRect(mAnimatedSprite.getGlobalBounds());
}

bool NPC::isMarkedForRemoval() const
{
    
}

void NPC::remove()
{
    Entity::remove();
}

int NPC::updateMovementPattern(sf::Time dt)
{
    return random(1, 10);
}

void NPC::updateMoveAnimation(sf::Time dt)
{
    Animation walkingAnimationDown;
    walkingAnimationDown.setSpriteSheet(mTexture);
    walkingAnimationDown.addFrameLine(4, 4, 0);
    
    Animation walkingAnimationLeft;
    walkingAnimationLeft.setSpriteSheet(mTexture);
    walkingAnimationLeft.addFrameLine(4, 4, 1);
    
    Animation walkingAnimationRight;
    walkingAnimationRight.setSpriteSheet(mTexture);
    walkingAnimationRight.addFrameLine(4, 4, 2);
    
    Animation walkingAnimationUp;
    walkingAnimationUp.setSpriteSheet(mTexture);
    walkingAnimationUp.addFrameLine(4, 4, 3);
    
    mCurrentAnimation = &walkingAnimationLeft;
    
    sf::Vector2f movement(0.f, 0.f);
    
    int i = updateMovementPattern(dt);
    
    switch (i)
    {
        case 1:
            mCurrentAnimation = &walkingAnimationUp;
            movement.y -= Table[mType].speed;
            break;
        case 2:
            mCurrentAnimation = &walkingAnimationDown;
            movement.y += Table[mType].speed;
            break;
        case 3:
            mCurrentAnimation = &walkingAnimationLeft;
            movement.x -= Table[mType].speed;
            break;
        case 4:
            mCurrentAnimation = &walkingAnimationRight;
            movement.x += Table[mType].speed;
            break;
            
        default:
            break;
    }

    
    mAnimatedSprite.play(*mCurrentAnimation);
    mAnimatedSprite.move(movement * dt.asSeconds());
    
    
    mAnimatedSprite.update(dt);


}




