#pragma once
#include <exception>
#include <string>
#include <sstream>

class TextureNotFoundException : public std::exception
{
public:
	TextureNotFoundException(std::string texturepathparam) : std::exception(), texturepath(texturepathparam)
	{
		std::ostringstream oss;
		oss << "Impossible de charger la texture : \"" << texturepath << "\".";
		msg = oss.str();
	}

	virtual ~TextureNotFoundException() throw()
	{

	}

	virtual const char * what() const throw()
	{
		return msg.c_str();
	}

private:
	const std::string texturepath;
	std::string msg;
};