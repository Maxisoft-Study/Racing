#include "stdafx.h"
#include "BoxGameObject.h"


BoxGameObject::BoxGameObject(b2World *world, b2BodyDef *bodydefparam) : body(NULL)
{
        bodydef = bodydefparam ? new b2BodyDef(*bodydefparam) : new b2BodyDef();
}


BoxGameObject::~BoxGameObject()
{
        if (body)
        {
                getWorld()->DestroyBody(body);
        }
        delete bodydef;
}


b2World *BoxGameObject::getWorld(void) const
{
        return body->GetWorld();
}

b2Body *BoxGameObject::getBody(void) const
{
        return body;
}
