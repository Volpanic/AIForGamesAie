#pragma once
#include "raylib.h"
#include <iostream>
#include <string>
#include <filesystem>
#include <map>

class ResourceManager
{
public:

	void Init();
	void Unload();

	Texture2D& GetTexture(const char* fileName);
	Font& GetFont(const char* fileName);

private:
	const std::string m_spritePath = "/Sprites";
	const std::string m_fontPath = "/Fonts";

	std::map<std::string, Texture2D> m_textures;
	std::map<std::string, Font> m_fonts;

};
