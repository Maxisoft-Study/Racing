#include "stdafx.h"
#include "BoxGameObject.h"


BoxGameObject::BoxGameObject(b2World *world, const float init_pos_x, const float init_pos_y) : GameObject(), body(NULL)
{
    bodydef = new b2BodyDef();
	bodydef->position.Set(init_pos_x, init_pos_y);
}

BoxGameObject::BoxGameObject(b2World *world, b2BodyDef *bodydefparam) : GameObject(), body(NULL), bodydef(bodydefparam)
{
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
