#pragma once
#include "stdafx.h"
#include "EventHandler.h"

namespace racing{
	const sf::ContextSettings settings{0, 0, 2, 0, 0};
	const float BOX2D_METERS_TO_PIXEL = 72.f;
	const uint MAXIMUM_TEXTURE_SIZE = sf::Texture::getMaximumSize();
}
