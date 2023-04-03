#pragma once

#include <SFML\Graphics.hpp>

struct MaterialReflection
{
	float ambient;
	float diffuse;
	float specular;
};

class Material
{
public:
	Material(MaterialReflection red, MaterialReflection green, MaterialReflection blue, float shininess);
	~Material();

	sf::Color getColor(const sf::Vector3f &materialVector, const sf::Vector3f &lightVector, const sf::Color &lightColor) const;

	const MaterialReflection RED;
	const MaterialReflection GREEN;
	const MaterialReflection BLUE;
	const float SHININESS;
};