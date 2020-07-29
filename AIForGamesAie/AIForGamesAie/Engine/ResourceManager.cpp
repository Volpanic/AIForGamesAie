#include "ResourceManager.h"

Texture2D& ResourceManager::GetTexture(const char* fileName)
{
	return m_textures[fileName];
}

Font& ResourceManager::GetFont(const char* fileName)
{
	return m_fonts[fileName];
}

void ResourceManager::Init()
{
	std::string currentDir = std::filesystem::current_path().generic_string();
	//Sprites
	for (const auto& entry : std::filesystem::directory_iterator(currentDir + m_spritePath))
	{
		m_textures[entry.path().filename().generic_string().c_str()] = LoadTexture(entry.path().generic_string().c_str());
		std::cout << "Texture: " << entry.path().filename().generic_string().c_str() << " : " << entry.path().generic_string().c_str() << std::endl;
	}

	//Fonts
	//for (const auto& entry : std::filesystem::directory_iterator(currentDir + m_fontPath))
	//{
	//	std::string key = entry.path().filename().generic_string().c_str();
	//	m_fonts[key] = LoadFont(entry.path().generic_string().c_str());
	//	std::cout << "Font: " << entry.path().filename().generic_string().c_str() << " : " << entry.path().generic_string().c_str() << std::endl;

	//	SetTextureFilter(m_fonts[key].texture, TextureFilterMode::FILTER_POINT);
	//}
}

void ResourceManager::Unload()
{
	//Sprites
	for (auto const& tex : m_textures)
	{
		UnloadTexture(tex.second);
	}

	//Fonts
	for (auto const& tex : m_fonts)
	{
		UnloadFont(tex.second);
	}
}
