#pragma once
#include "raylib.h"
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <map>

class ResourceManager
{
public:

	void Init();
	void Unload();

	Texture2D& GetTexture(const char* fileName);
	Texture2D& GetTileset(const char* fileName);
	Font& GetFont(const char* fileName);

	bool TextureExists(const char* fileName);
	bool TilesetExists(const char* fileName);
	bool FontExists(const char* fileName);

	std::vector<std::string> GetAllTilesetNames();

private:
	const std::string m_spritePath = "/Sprites";
	const std::string m_tilesPath = "/Tilesets";
	const std::string m_fontPath = "/Fonts";

	std::map<std::string, Texture2D> m_textures;
	std::map<std::string, Texture2D> m_tilesets;
	std::map<std::string, Font> m_fonts;

};
