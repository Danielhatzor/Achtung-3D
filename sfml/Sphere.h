#pragma once

#include <vector>
#include <SFML\Graphics.hpp>
#include "Material.h"

namespace Sphere
{
	extern Material getMaterial(sf::Vector3f);
	extern void load();
	extern void unload();
	extern bool isLoaded();
	extern const sf::Sprite& getSphere(sf::Vector3f lightPos);

	extern std::vector < sf::Sprite > sprites;
	extern bool loaded;
	extern int texturesLoaded;
};


//#pragma once
//
//#include <vector>
//#include <SFML\Graphics.hpp>
//
//class Sphere
//{
//public:
//	Sphere();
//	~Sphere();
//
//	const sf::Sprite getSphere(sf::Vector3f lightPos);
//	void update(const Material material, sf::Vector3f absolutePosition);
//private:
//	std::vector < sf::RenderTexture* > m_aEditedTextures;
//	std::vector < sf::Texture > m_aOrigTextures;
//};