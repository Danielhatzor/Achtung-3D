#include <SFML\Graphics.hpp>
#pragma once

class MyMat
{
public:
	MyMat(sf::Vector3f seed);
	~MyMat();

	// Seeding a vector, means setting it's angles for rotating points
	void SeedVector(sf::Vector3f seed);
	void SeedVector2(sf::Vector3f seed);
	sf::Vector3f RotateToSeed(sf::Vector3f vector);
	sf::Vector3f UnRotateFromSeed(sf::Vector3f vector);
	sf::Vector3f LerpToSeed(sf::Vector3f vector, float amount);

	sf::Vector3f UnRotateFromSeed2(sf::Vector3f vector);

	static sf::Vector3f RotateZ(sf::Vector3f vector, double angle);
	static sf::Vector3f RotateZ(sf::Vector3f vector, double sinAngle, double cosAngle);
	static sf::Vector3f RotateX(sf::Vector3f vector, double angle);
	static sf::Vector3f RotateX(sf::Vector3f vector, double sinAngle, double cosAngle);
	static sf::Vector3f RotateY(sf::Vector3f vector, double angle);
	static sf::Vector3f RotateY(sf::Vector3f vector, double sinAngle, double cosAngle);
private:
	sf::Vector3f m_v3Seed;

	// Alpha - angle between the seeded vector to the xz plain
	double alpha;
	double fSinAlpha;
	double fCosAlpha;

	// Beta - angle between the seeded vector to the yz plain
	double beta;
	double fSinBeta;
	double fCosBeta;

};