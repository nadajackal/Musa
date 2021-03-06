//
//  World.cpp
//  Musa
//
//  Created by Ian Wilson on 2/5/16.
//  Copyright (c) 2016 APM. All rights reserved.
//

#include "World.hpp"
#include "Utility.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

#include <algorithm>
#include <cmath>
#include <limits>
#include <iostream>


World::World(sf::RenderTarget& outputTarget, FontHolder& fonts)
: mTarget(outputTarget)
, mSceneTexture()
, mWorldView(outputTarget.getDefaultView())
, mFonts(fonts)
//, mSounds(sounds)
, mTextures()
, mSceneGraph()
, mSceneLayers()
, mWorldBounds(0.f, 0.f, mWorldView.getSize().x, 5000.f)
, mSpawnPosition(mWorldView.getSize().x / 2.f, mWorldBounds.height - mWorldView.getSize().y / 2.f)
, mPlayerHeros()
, mFinishSprite(nullptr)

{
    mSceneTexture.create(mTarget.getSize().x, mTarget.getSize().y);
    
    loadTextures();
    buildScene();
    
    // Prepare the view
    mWorldView.setCenter(mSpawnPosition);
}


void World::update(sf::Time dt)
{
    // Scroll the world, reset player velocity
    for(Hero* h : mPlayerHeros)
        h->setVelocity(0.f, 0.f);
    
    
    // All commands from queue are forwarded to scenegraph
    while (!mCommandQueue.isEmpty())
        mSceneGraph.onCommand(mCommandQueue.pop(), dt);
    adaptPlayerVelocity();
        
    
    // Remove all destroyed entities, create new ones
    mSceneGraph.removeWrecks();
    spawnNPCs();
    
    // Regular update step, adapt position (correct if outside view)
    mSceneGraph.update(dt, mCommandQueue);
    adaptPlayerPosition();
    
}

void World::draw()
{

    mTarget.setView(mWorldView);
    mTarget.draw(mSceneGraph);
    
}

void World::loadTextures()
{
    mTextures.load(Textures::Desert, "Media/Textures/Background.png");
    mTextures.load(Textures::Auron, "Media/Textures/auron.png");
    mTextures.load(Textures::Guy, "Media/Textures/player.png");
    mTextures.load(Textures::Samurai2, "Media/Textures/samurai2.png");
    mTextures.load(Textures::Samurai3, "Media/Textures/samurai3.png");


    
}

Hero* World::addHero()
{
    std::unique_ptr<Hero> player(new Hero(Hero::Musa, mTextures, mFonts));
    player->setPosition(mWorldView.getCenter());
    
    mPlayerHeros.push_back(player.get());
    mSceneLayers[Ground]->attachChild(std::move(player));
    
    return mPlayerHeros.back();
}
/**
 Methods for adding NPCs to world view
 1) addNPC creates a spawn point for a particular NPC
 2) addNPCs allows the user to manually define NPCs for the world
 3) spawnNPCs attaches these definitions to the scene graph
 */

void World::addNPC(NPC::Type type, float relX, float relY)
{
    SpawnPoint spawn(type, mSpawnPosition.x + relX, mSpawnPosition.y - relY);
    mNPCSpawnPoints.push_back(spawn);
}

void World::addNPCs()
{
    addNPC(NPC::Samurai2, 100.f, 200.f);
    addNPC(NPC::Samurai2, 200.f, 100.f);
    addNPC(NPC::Samurai3, 300.f, 50.f);
}

void World::spawnNPCs()
{
    while(!mNPCSpawnPoints.empty() && mNPCSpawnPoints.back().y > getBattlefieldBounds().top)
    {
        SpawnPoint spawn = mNPCSpawnPoints.back();
        
        std::unique_ptr<NPC> npc(new NPC(spawn.type, mTextures.get(Textures::Samurai2), mFonts));
        npc->setPosition(spawn.x, spawn.y);
        
        mSceneLayers[Ground]->attachChild(std::move(npc));
        mNPCSpawnPoints.pop_back();
    }
        
}

/****~~~******
 This function builds the different layers, the background, and adds sprites to the scene
 ********/

void World::buildScene()
{
    // Initialize the different layers
    for (std::size_t i = 0; i < LayerCount; ++i)
    {
        Category::Type category = (i == Ground) ? Category::SceneAirLayer : Category::None;
        
        SceneNode::Ptr layer(new SceneNode(category));
        mSceneLayers[i] = layer.get();
        
        mSceneGraph.attachChild(std::move(layer));
    }
    
    // Prepare the tiled background
    sf::Texture& backTexture = mTextures.get(Textures::Desert);
    backTexture.setRepeated(true);
    
    float viewHeight = mWorldView.getSize().y;
    sf::IntRect textureRect(mWorldBounds);
    textureRect.height += static_cast<int>(viewHeight);
    
    // Add the background sprite to the scene
    std::unique_ptr<SpriteNode> backSprite(new SpriteNode(backTexture, textureRect));
    backSprite->setPosition(mWorldBounds.left, mWorldBounds.top - viewHeight);
    mSceneLayers[Background]->attachChild(std::move(backSprite));
    
    addNPCs();
}

void World::setCurrentBattleFieldPosition(float lineY)
{
    mWorldView.setCenter(mWorldView.getCenter().x, lineY - mWorldView.getSize().y/2);
    mSpawnPosition.y = mWorldBounds.height;
}

void World::setWorldHeight(float height)
{
    mWorldBounds.height = height;
}


void World::adaptPlayerPosition()
{
    // Keep player's position inside the screen bounds, at least borderDistance units from the border
    sf::FloatRect viewBounds = getViewBounds();
    const float borderDistance = 40.f;
    

    for(Hero* hero : mPlayerHeros)
    {
        sf::Vector2f position = hero->getPosition();
        position.x = std::max(position.x, viewBounds.left + borderDistance);
        position.x = std::min(position.x, viewBounds.left + viewBounds.width - borderDistance);
        position.y = std::max(position.y, viewBounds.top + borderDistance);
        position.y = std::min(position.y, viewBounds.top + viewBounds.height - borderDistance);
        hero->setPosition(position);
    }
}

void World::adaptPlayerVelocity()
{
    for(Hero* hero : mPlayerHeros)
    {
        sf::Vector2f velocity = hero->getVelocity();
        
        // If moving diagonally, reduce velocity (to have always same velocity)
        if (velocity.x != 0.f && velocity.y != 0.f)
            hero->setVelocity(velocity / std::sqrt(2.f));
    }
}


bool matchesCategories(SceneNode::Pair& colliders, Category::Type type1, Category::Type type2)
{
    unsigned int category1 = colliders.first->getCategory();
    unsigned int category2 = colliders.second->getCategory();
    
    // Make sure first pair entry has category type1 and second has type2
    if (type1 & category1 && type2 & category2)
    {
        return true;
    }
    else if (type1 & category2 && type2 & category1)
    {
        std::swap(colliders.first, colliders.second);
        return true;
    }
    else
    {
        return false;
    }
}

void World::handleCollisions()
{
//    std::set<SceneNode::Pair> collisionPairs;
//    mSceneGraph.checkSceneCollision(mSceneGraph, collisionPairs);
//    
//    for(SceneNode::Pair pair : collisionPairs)
//    {
//        if (matchesCategories(pair, Category::PlayerAircraft, Category::EnemyAircraft))
//        {
//            auto& player = static_cast<Aircraft&>(*pair.first);
//            auto& enemy = static_cast<Aircraft&>(*pair.second);
//            
//            // Collision: Player damage = enemy's remaining HP
//            player.damage(enemy.getHitpoints());
//            enemy.destroy();
//        }
//        
//        else if (matchesCategories(pair, Category::PlayerAircraft, Category::Pickup))
//        {
//            auto& player = static_cast<Aircraft&>(*pair.first);
//            auto& pickup = static_cast<Pickup&>(*pair.second);
//            
//            // Apply pickup effect to player, destroy projectile
//            pickup.apply(player);
//            pickup.destroy();
//            player.playLocalSound(mCommandQueue, SoundEffect::CollectPickup);
//            
//        }
//        
//        else if (matchesCategories(pair, Category::EnemyAircraft, Category::AlliedProjectile)
//                 || matchesCategories(pair, Category::PlayerAircraft, Category::EnemyProjectile))
//        {
//            auto& aircraft = static_cast<Aircraft&>(*pair.first);
//            auto& projectile = static_cast<Projectile&>(*pair.second);
//            
//            // Apply projectile damage to aircraft, destroy projectile
//            aircraft.damage(projectile.getDamage());
//            projectile.destroy();
//        }
//    }
}




sf::FloatRect World::getViewBounds() const
{
    return sf::FloatRect(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
}

sf::FloatRect World::getBattlefieldBounds() const
{
    // Return view bounds + some area at top, where enemies spawn
    sf::FloatRect bounds = getViewBounds();
    bounds.top -= 100.f;
    bounds.height += 100.f;
    
    return bounds;
}


CommandQueue& World::getCommandQueue()
{
    return mCommandQueue;
}


