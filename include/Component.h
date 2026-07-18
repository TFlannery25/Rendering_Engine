#ifndef COMPONENT_H
#define COMPONENT_H

class Object; 

class Component
{
    public:
        virtual void Update(Object& owner, float deltaTime) = 0;
        virtual ~Component() {}
};

#endif