//
//  Component.h
//  Musa
//
//  Created by Ian Wilson on 2/4/16.
//  Copyright (c) 2016 APM. All rights reserved.
//

#ifndef __Musa__Component__
#define __Musa__Component__

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include <memory>


namespace sf
{
    class Event;
}

namespace GUI   // Graphical User Interface
{
    
    class Component : public sf::Drawable, public sf::Transformable, private sf::NonCopyable
    {
    public:
        typedef std::shared_ptr<Component> Ptr;
        
        
    public:
        Component();
        virtual				~Component();
        
        virtual bool		isSelectable() const = 0;
        bool				isSelected() const;
        virtual void		select();
        virtual void		deselect();
        
        virtual bool		isActive() const;
        virtual void		activate();
        virtual void		deactivate();
        
        virtual void		handleEvent(const sf::Event& event) = 0;
        
        
    private:
        bool				mIsSelected;
        bool				mIsActive;
    };
    
}


#endif /* defined(__Musa__Component__) */
