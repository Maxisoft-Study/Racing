#pragma once

#include "stdafx.h"
#include "TextureLoader.h"

namespace racing{

	//RUNTIME GLOBALS
	extern TextureLoader TEXTURE_LOADER;
	extern YAML::Node CONFIG;
	extern sf::VideoMode VIDEO_MODE;
	extern boost::bimap<const std::string, const sf::Keyboard::Key> STRING_TO_SFML_KEYBOARD_KEY;
}