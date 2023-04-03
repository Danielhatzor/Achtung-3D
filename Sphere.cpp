#include <cassert>
#include <cmath>
#include <vector>
#include <SFML\Graphics.hpp>
#include "Constants1.h"
#include "MyMat.h"
#include "Sphere.h"
#include "Material.h"

std::vector < sf::Sprite > Sphere::sprites;

bool Sphere::loaded = false;
int Sphere::texturesLoaded = 0;

Material Sphere::getMaterial(sf::Vector3f vector)
{
	float z = vector.z * vector.z; z *= z; z *= z; z *= z;
	float y = vector.y * vector.y; y *= y; y *= y; y *= y;
	float x = vector.x * vector.x; x *= x; x *= x; x *= x;
	float vecLen = sqrt(x * x + y * y + z * z);
	x /= vecLen;
	y /= vecLen;
	z /= vecLen;

	return Material
	{
		{ x, 0, 0 },
		{ y, 0, 0 },
		{ z, 0, 0 }, 64
		/*{ 0.1f * x, 0.6f * x, 0.3f },
		{ 0.1f * y, 0.6f * y, 0.3f },
		{ 0.1f * z, 0.6f * z, 0.3f }, 64 */
	};
}

void Sphere::load()
{
	sf::Vector3f lightPos(0, 0, 1);

	const int radius = Constants::SPHERE_RADIUS * Constants::SCALE;
	const float rotationAngle = atan2(1, Constants::SPHERE_RADIUS);
	const float sinAngle = sin(rotationAngle);
	const float cosAngle = cos(rotationAngle);

	sprites.resize(Constants::PI * Constants::SPHERE_RADIUS);

	for (int i = 0; i < sprites.size(); ++i)
	{
		sf::Texture *texture = new sf::Texture();
		sf::Image image;

		image.create(radius * 2, radius * 2, sf::Color::Transparent);

		for (int j = 0; j < radius * 2; ++j)
		{
			for (int k = 0; k < radius * 2; ++k)
			{
				sf::Color pixelCol;
				sf::Vector3f normalizedVector(k, j, 0);

				float tempX = normalizedVector.x - radius;
				float tempY = normalizedVector.y - radius;

				if (tempX * tempX + tempY * tempY <= radius * radius)
				{
					normalizedVector.x = tempX / radius;
					normalizedVector.y = tempY / radius;
					normalizedVector.z = sqrt(1 - normalizedVector.x * normalizedVector.x - normalizedVector.y * normalizedVector.y);

					//MyMat newMat(lightPos);
					//sf::Vector3f realVector = newMat.UnRotateFromSeed(normalizedVector);

					pixelCol = Constants::SPHERE_MATERIAL.getColor(normalizedVector, lightPos, sf::Color::White);
					image.setPixel(k, j, pixelCol);
				}
			}
		}

		assert(texture->loadFromImage(image));
		texture->setSmooth(true);
		sprites[i].setTexture(*texture);
		sprites[i].setOrigin(radius, radius);
		sprites[i].setPosition(Constants::GAME_LENGTH / 2, Constants::GAME_HIGHT / 2);
		lightPos = MyMat::RotateX(lightPos, sinAngle, cosAngle);

		++texturesLoaded;
	}

	loaded = true;
}

void Sphere::unload()
{
	for (auto& sprite : sprites)
	{
		delete sprite.getTexture();
	}
}

bool Sphere::isLoaded()
{
	return loaded;
}

const sf::Sprite& Sphere::getSphere(sf::Vector3f lightPos)
{
	float rotation = atan2(lightPos.y, lightPos.x);

	sf::Vector3f temp = MyMat::RotateZ(lightPos, -rotation);
	float gap = atan2(temp.x, temp.z);
	int index = gap * (sprites.size() - 1) / Constants::PI;

	sprites[index].setRotation(rotation / Constants::DEG_TO_RADS + 90);

	return sprites[index];
}


/*
#include <cassert>
#include <cmath>
#include <vector>
#include <SFML\Graphics.hpp>
#include "Constants1.h"
#include "MyMat.h"
#include "Material.h"
#include "Sphere.h"

Sphere::Sphere()
{
	sf::Vector3f lightPos(0, 0, 1);

	const int radius = Constants::SPHERE_RADIUS * Constants::SCALE;
	const float rotationAngle = atan2(1, Constants::SPHERE_RADIUS);
	const float sinAngle = sin(rotationAngle);
	const float cosAngle = cos(rotationAngle);

	m_aOrigTextures.resize(Constants::PI * Constants::SPHERE_RADIUS);
	m_aEditedTextures.resize(Constants::PI * Constants::SPHERE_RADIUS);

	for (int i = 0; i < m_aOrigTextures.size(); ++i)
	{
		sf::Texture& texture = m_aOrigTextures[i];
		sf::Image image;

		image.create(radius * 2, radius * 2, sf::Color::Transparent);

		for (int j = 0; j < radius * 2; ++j)
		{
			for (int k = 0; k < radius * 2; ++k)
			{
				sf::Color pixelCol;
				sf::Vector3f normalizedVector(k, j, 0);

				float tempX = normalizedVector.x - radius;
				float tempY = normalizedVector.y - radius;

				if (tempX * tempX + tempY * tempY <= radius * radius)
				{
					normalizedVector.x = tempX / radius;
					normalizedVector.y = tempY / radius;
					normalizedVector.z = sqrt(1 - normalizedVector.x * normalizedVector.x - normalizedVector.y * normalizedVector.y);

					pixelCol = Constants::SPHERE_MATERIAL.getColor(normalizedVector, lightPos, sf::Color::White);
					image.setPixel(k, j, pixelCol);
				}
			}
		}

		while(!texture.loadFromImage(image));
		texture.setSmooth(true);

		sf::Sprite sprite(texture);
		sf::RenderTexture* edited = new sf::RenderTexture();
		edited->create(radius * 2, radius * 2);
		edited->clear();
		edited->draw(sprite);
		m_aEditedTextures[i] = edited;

		lightPos = MyMat::RotateX(lightPos, sinAngle, cosAngle);
	}
}

Sphere::~Sphere()
{
	//for (auto& sprite : m_aSprites)
	//{
	//	delete sprite.getTexture();
	//}
}

const sf::Sprite Sphere::getSphere(sf::Vector3f lightPos)
{
	float rotation = atan2(lightPos.y, lightPos.x);

	sf::Vector3f temp = MyMat::RotateZ(lightPos, -rotation);
	float gap = atan2(temp.x, temp.z);
	int index = gap * (m_aEditedTextures.size() - 1) / Constants::PI;

	m_aEditedTextures[index]->display();
	const sf::Texture& texture = m_aEditedTextures[index]->getTexture();
	sf::Sprite sprite(texture);

	sprite.setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);
	sprite.setPosition(Constants::GAME_LENGTH / 2, Constants::GAME_HIGHT / 2);
	sprite.setRotation(rotation / Constants::DEG_TO_RADS + 90);

	return sprite;
}

void Sphere::update(const Material material, sf::Vector3f absolutePosition)
{
	using namespace Constants;

	for (int i = 0; i < m_aEditedTextures.size(); ++i)
	{
		const float sqrtSphereRad = sqrt(SPHERE_RADIUS);

		sf::CircleShape curveHead;
		sf::Vector3f proportianlPosition = MyMat::RotateX(absolutePosition, -i / (float)SPHERE_RADIUS);
		sf::Vector3f lightPosition = MyMat::RotateX(sf::Vector3f(0, 0, 1), i / (float)SPHERE_RADIUS);

		if (proportianlPosition.z > 0)
		{
			float curveRad = SCALE * CURVE_HEAD_RADIUS * sqrt(proportianlPosition.z) / sqrtSphereRad;
			curveHead.setRadius(curveRad);
			float x = SCALE * proportianlPosition.x + GAME_LENGTH / 2 - curveRad;
			float y = SCALE * proportianlPosition.y + GAME_HIGHT / 2 - curveRad;
			curveHead.setPosition(x, y);

			sf::Vector3f normal = sf::Vector3f(proportianlPosition.x / SPHERE_RADIUS, proportianlPosition.y / SPHERE_RADIUS, proportianlPosition.z / SPHERE_RADIUS);
			curveHead.setFillColor(material.getColor(normal, lightPosition, LIGHT_COLOR));

			m_aEditedTextures[i]->draw(curveHead);
		}
	}
}
*/