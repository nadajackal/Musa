//
//  Hero.cpp
//  Musa
//
//  Created by Ian Wilson on 2/6/16.
//  Copyright (c) 2016 APM. All rights reserved.
//

#include "Hero.hpp"
#include "ResourceHolder.hpp"
#include "Utility.hpp"
#include "CommandQueue.hpp"
#include "DataTables.hpp"

#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/RenderStates.hpp"

#include <cmath>

using namespace std::placeholders;


namespace
{
    const std::vector<HeroData> Table = initializeHeroData();
}

Hero::Hero(Type type, const TextureHolder& textures, const FontHolder& fonts)
: Entity(Table[type].hitpoints)
, mType(type)
, mSprite(textures.get(Table[type].texture), Table[type].textureRect)
, mTravelledDistance(0.f)
, mDirectionIndex(0)
, mIdentifier(0)
{
    centerOrigin(mSprite);
}

/******************************************************************************************************
 drawCurrent only draws the sprite. We call sf::RenderTarget::draw(), and pass const sf::Drawable& (our sprite) as first argument and the render states as second argument.
 ******************************************************************************************************/
void Hero::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
}

unsigned int Hero::getCategory() const
{

}

void Hero::updateCurrent(sf::Time dt, CommandQueue& commands)
{

}

sf::FloatRect Hero::getBoundingRect() const
{
    return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

bool Hero::isMarkedForRemoval() const
{
    
}

void Hero::remove()
{
    Entity::remove();
}


float Hero::getMaxSpeed() const
{
    return Table[mType].speed;
}


