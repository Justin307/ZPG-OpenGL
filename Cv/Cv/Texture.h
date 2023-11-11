#pragma once

#include <string>
#include "texture.h"

class Texture
{
private:
	int width;
	int height;
	int nrChannels;
	unsigned int texture;
public:
	Texture(std::string path);
	void Use();
};

