#include "stdafx.h"
#include "TextureLoader.h"
#include "TextureNotFoundException.h"

TextureLoader TextureLoader::instance = {};


TextureLoader::TextureLoader(void) :textures()
{

}

TextureLoader::~TextureLoader()
{

}


texture_ptr TextureLoader::get(const std::string& s)
{
	std::string fs = pathFormat(s);
	auto it = textures.find(fs);
	if (it != textures.end())
		return it->second;

	if (!load(fs))
		throw TextureNotFoundException(fs);
	return textures.at(fs);

}

texture_ptr TextureLoader::operator[](const std::string &s)
{
	return get(s);
}

const std::string TextureLoader::pathFormat(const std::string& s)
{
	boost::filesystem::path full_path(boost::filesystem::initial_path<boost::filesystem::path>());
	full_path = boost::filesystem::system_complete(boost::filesystem::path(s));
	return full_path.generic_string();
}

bool TextureLoader::load(const std::string& s)
{
	std::string fs = pathFormat(s);
	texture_ptr texture = std::shared_ptr<sf::Texture>(new sf::Texture());
	if (!texture->loadFromFile(fs))
	{
		LOG_WARN << "Unable to load file " << fs;
		return false;
	}
	texture->setSmooth(true);

	textures[fs] = texture;
	return true;
}

void TextureLoader::trim(void)
{
	for (auto &pair : textures)
	{
		if (pair.second.use_count() == 1)
		{
			textures.erase(pair.first);
		}
	}
}