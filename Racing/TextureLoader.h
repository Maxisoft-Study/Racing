#pragma once


#include "stdafx.h"
typedef std::shared_ptr<sf::Texture> texture_ptr;

class TextureLoader
{
public:
	TextureLoader(void) :textures(){};
	~TextureLoader(){};

	texture_ptr get(const std::string &s)
	{
		std::string fs = pathFormat(s);
		std::unordered_map<std::string, std::shared_ptr<sf::Texture>>::iterator it = textures.find(fs);
		if (it != textures.end())
			return it->second;

		if (!load(fs))
			throw "Impossible de charger la texture.";
		return textures.at(fs);
		
	}

	texture_ptr operator [](const std::string &s)
	{
		return get(s);
	}

	const std::string pathFormat(const std::string &s)
	{
		boost::filesystem::path full_path(boost::filesystem::initial_path<boost::filesystem::path>());
		full_path = boost::filesystem::system_complete(boost::filesystem::path(s));
		return full_path.generic_string();
	}

	bool load(const std::string &s)
	{
		std::string fs = pathFormat(s);
		texture_ptr texture = std::shared_ptr<sf::Texture>(new sf::Texture());
		if (!texture->loadFromFile(fs))
		{
			return false;
		}
		textures[fs] = texture;
		return true;
	}

	void trim(void)
	{
		for (auto &pair : textures)
		{
			if (pair.second.use_count() == 1)
			{
				textures.erase(pair.first);
			}
		}
	}

private:
	std::unordered_map<std::string, std::shared_ptr<sf::Texture>> textures;
};