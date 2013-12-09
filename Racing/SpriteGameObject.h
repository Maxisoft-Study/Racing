#pragma once
#include "stdafx.h"
#include "GameObject.h"
class SpriteGameObject :
        public GameObject,
        public sf::Drawable
{

friend class sf::RenderTarget;
public:
        virtual void update(float delta) {}; //TODO
        SpriteGameObject(const std::string &file, bool center=true);
        ~SpriteGameObject(void);
        void move(const sf::Vector2f vec)
        {
                base_sprite.move(vec);
        }

        void rotate(const float f)
        {
                base_sprite.rotate(f);
        }
        void setPosition(const sf::Vector2f vec)
        {
                base_sprite.setPosition(vec);
        }
protected:
        sf::Sprite base_sprite;
        sf::Texture texture;
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
private:
};
