#pragma once
#include "stdafx.h"
#include "LoggerConfig.h"

typedef std::shared_ptr<sf::Texture> texture_ptr;

class TextureLoader
{
public:

	static TextureLoader& Instance(){ return instance; }

	~TextureLoader();

	texture_ptr get(const std::string& s);
	texture_ptr operator[](const std::string &s);
	const std::string pathFormat(const std::string& s);
	bool load(const std::string& s);
	void trim(void);

private:
	static TextureLoader instance;
	std::map<std::string, std::shared_ptr<sf::Texture>> textures;
	TextureLoader(void);
};