#ifndef SHADOW_MAP_H
#define SHADOW_MAP_H
#include "glad/glad.h"

const int SHADOW_WIDTH  = 1024;
const int SHADOW_HEIGHT = 1024;


class ShadowMap{
    
    private: 
        GLuint FBO;
        GLuint depthTexture;

    public:
        ShadowMap();
        ~ShadowMap();

        // Not copyable — copying would duplicate GL handles and cause a double-free
        ShadowMap(const ShadowMap&) = delete;
        ShadowMap& operator=(const ShadowMap&) = delete;

        // Movable — transfers ownership of the GL handles
        ShadowMap(ShadowMap&& other) noexcept;
        ShadowMap& operator=(ShadowMap&& other) noexcept;

        void Bind();
        void Unbind(int SCREEN_WIDTH, int SCREEN_HEIGHT);
        GLuint GetTexture() const;
    
};


#endif