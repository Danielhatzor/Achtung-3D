#include <SFML\Graphics.hpp>
#include "Material.h"

Material::Material(MaterialReflection red, MaterialReflection green, MaterialReflection blue, float shininess)
	: RED(red),
	GREEN(green),
	BLUE(blue),
	SHININESS(shininess)
{
}

Material::~Material()
{
}

sf::Color Material::getColor(const sf::Vector3f &materialVector, const sf::Vector3f &lightVector, const sf::Color &lightColor) const
{
	sf::Color returnedCol;
	float diffuseDotProduct;
	float reflectionDotProduct;
	sf::Vector3f reflection;
	sf::Vector3f view(0, 0, 1);

	// ambient
	returnedCol.r = RED.ambient * 255;
	returnedCol.g = GREEN.ambient * 255;
	returnedCol.b = BLUE.ambient * 255;

	diffuseDotProduct =
		(materialVector.x * lightVector.x +
		materialVector.y * lightVector.y +
		materialVector.z * lightVector.z);

	if (diffuseDotProduct > 0)
	{
		// diffuse
		returnedCol.r += RED.diffuse * lightColor.r * diffuseDotProduct;
		returnedCol.g += GREEN.diffuse * lightColor.g * diffuseDotProduct;
		returnedCol.b += BLUE.diffuse * lightColor.b * diffuseDotProduct;

		reflection.x = (2 * diffuseDotProduct * materialVector.x - lightVector.x);
		reflection.y = (2 * diffuseDotProduct * materialVector.y - lightVector.y);
		reflection.z = (2 * diffuseDotProduct * materialVector.z - lightVector.z);

		reflectionDotProduct =
			view.x * reflection.x +
			view.y * reflection.y +
			view.z * reflection.z;

		if (reflectionDotProduct > 0)
		{
			float temp = pow(reflectionDotProduct, SHININESS);

			// specular
			returnedCol.r += RED.specular * temp * lightColor.r;
			returnedCol.g += GREEN.specular * temp * lightColor.g;
			returnedCol.b += BLUE.specular * temp * lightColor.b;
		}

	}

	return returnedCol;
}