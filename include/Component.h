#ifndef COMPONENT_H
#define COMPONENT_H
#include "UpdateContext.h"

class Object; 

class Component
{
    public:
        virtual void Update(Object& owner, const UpdateContext& updateContext) = 0;
        virtual ~Component() {}
};

#endif