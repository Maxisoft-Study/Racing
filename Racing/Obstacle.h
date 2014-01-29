#pragma once
#include "SpriteGameObject.h"
class Obstacle :
        public SpriteGameObject
{
public:
        explicit Obstacle(const std::string& file);
        ~Obstacle(void);
};
