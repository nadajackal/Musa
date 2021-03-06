//
//  ResourceHolder.hpp
//  Musa
//
//  Created by Ian Wilson on 2/3/16.
//  Copyright (c) 2016 APM. All rights reserved.
//

#ifndef Musa_ResourceHolder_hpp
#define Musa_ResourceHolder_hpp

#include <map>
#include <string>
#include <memory>
#include <stdexcept>
#include <cassert>


template <typename Resource, typename Identifier>
class ResourceHolder
{
public:
    void						load(Identifier id, const std::string& filename);
    
    template <typename Parameter>
    void						load(Identifier id, const std::string& filename, const Parameter& secondParam);
    
    Resource&					get(Identifier id);
    const Resource&				get(Identifier id) const;
    
    
private:
    void						insertResource(Identifier id, std::unique_ptr<Resource> resource);
    
    
private:
    std::map<Identifier, std::unique_ptr<Resource>>	mResourceMap;
};

#include "ResourceHolder.inl"


#endif
