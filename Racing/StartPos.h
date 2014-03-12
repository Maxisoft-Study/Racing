#pragma once
#include "stdafx.h"
struct StartPos
{
	const b2Vec2 pos;
	const float angle;
	explicit StartPos(float x, float y, float a) : pos(x, y), angle(a) {}
};