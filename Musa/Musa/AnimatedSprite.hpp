//
//  AnimatedSprite.hpp
//  Musa
//
//  Created by Ian Wilson on 2/6/16.
//  Copyright (c) 2016 APM. All rights reserved.
//

#ifndef __Musa__AnimatedSprite__
#define __Musa__AnimatedSprite__

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Vector2.hpp>

#include "Animation.hpp"

class AnimatedSprite : public sf::Drawable, public sf::Transformable
{
public:
    explicit AnimatedSprite(sf::Time frameTime = sf::seconds(0.2f), bool paused = false, bool looped = true);
    
    void update(sf::Time deltaTime);
    void setAnimation(const Animation& animation);
    void setFrameTime(sf::Time time);
    void play();
    void play(const Animation& animation);
    void pause();
    void stop();
    void setLooped(bool looped);
    void setColor(const sf::Color& color);
    const Animation* getAnimation() const;
    sf::FloatRect getLocalBounds() const;
    sf::FloatRect getGlobalBounds() const;
    bool isLooped() const;
    bool isPlaying() const;
    sf::Time getFrameTime() const;
    void setFrame(std::size_t newFrame, bool resetTime = true);
    
private:
    const Animation* mAnimation;
    sf::Time mFrameTime;
    sf::Time mCurrentTime;
    std::size_t mCurrentFrame;
    bool mIsPaused;
    bool mIsLooped;
    const sf::Texture* mTexture;
    sf::Vertex mVertices[4];
    
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    
};

#endif /* defined(__Musa__AnimatedSprite__) */
