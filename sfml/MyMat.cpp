#include <math.h>
#include "Constants1.h"
#include "MyMat.h"

MyMat::MyMat(sf::Vector3f seed)
{
	SeedVector(seed);
}

MyMat::~MyMat()
{
}

void MyMat::SeedVector(sf::Vector3f seed)
{
	double x = seed.x;
	double y = seed.y;
	double z = seed.z;

	m_v3Seed = sf::Vector3f(x, y, z);

	alpha = atan2(y, x);
	fSinAlpha = sin(alpha);
	fCosAlpha = cos(alpha);

	sf::Vector3f newVec(sqrt(x*x + y*y), 0, z);

	beta = atan2(newVec.x, newVec.z);
	fSinBeta = sin(beta);
	fCosBeta = cos(beta);
}

void MyMat::SeedVector2(sf::Vector3f seed)
{
	double x = seed.z;
	double y = seed.y;
	double z = seed.x;

	m_v3Seed = sf::Vector3f(x, y, z);

	alpha = atan2(y, x);
	fSinAlpha = sin(alpha);
	fCosAlpha = cos(alpha);

	sf::Vector3f newVec(sqrt(x*x + y*y), 0, z);

	beta = atan2(newVec.x, newVec.z);
	fSinBeta = sin(beta);
	fCosBeta = cos(beta);
}

sf::Vector3f MyMat::RotateToSeed(sf::Vector3f vector)
{
	sf::Vector3f newVec, newVec2;

	// Rotating about y to xz plane
	newVec.x = fCosBeta * vector.x + fSinBeta * vector.z;
	newVec.y = vector.y;
	newVec.z = -fSinBeta * vector.x + fCosBeta * vector.z;

	// Rotating about z to seed
	newVec2.x = fCosAlpha * newVec.x - fSinAlpha * newVec.y;
	newVec2.y = fSinAlpha * newVec.x + fCosAlpha * newVec.y;
	newVec2.z = newVec.z;

	return newVec2;
}

sf::Vector3f MyMat::UnRotateFromSeed(sf::Vector3f vector)
{
	sf::Vector3f newVec, newVec2;

	// UnRotating about z to xz plane
	newVec.x = fCosAlpha * vector.x + fSinAlpha * vector.y;
	newVec.y = -fSinAlpha * vector.x + fCosAlpha * vector.y;
	newVec.z = vector.z;

	// UnRotating about y to z axis
	newVec2.x = fCosBeta * newVec.x - fSinBeta * newVec.z;
	newVec2.y = newVec.y;
	newVec2.z = fSinBeta * newVec.x + fCosBeta * newVec.z;

	return newVec2;
}

sf::Vector3f MyMat::UnRotateFromSeed2(sf::Vector3f vector)
{
	sf::Vector3f newVec, newVec2;

	// UnRotating about z to xz plane
	newVec.z = fCosAlpha * vector.z + fSinAlpha * vector.y;
	newVec.y = -fSinAlpha * vector.z + fCosAlpha * vector.y;
	newVec.x = vector.x;

	// UnRotating about y to z axis
	newVec2.z = fCosBeta * newVec.z - fSinBeta * newVec.x;
	newVec2.y = newVec.y;
	newVec2.x = fSinBeta * newVec.z + fCosBeta * newVec.x;

	return newVec2;
}

sf::Vector3f MyMat::LerpToSeed(sf::Vector3f vector, float amount)
{
	sf::Vector3f proportionalVec = UnRotateFromSeed(vector);
	float angleToYZ = atan2(proportionalVec.x, proportionalVec.y);

	// Rotating about z to yz plane
	proportionalVec = RotateZ(proportionalVec, angleToYZ);

	// Getting angle between vectors
	float angleBetweenVectors = atan2(proportionalVec.y, proportionalVec.z);

	// Changing this angle
	proportionalVec = RotateX(proportionalVec, angleBetweenVectors * amount);

	// Undoing first steps
	proportionalVec = RotateZ(proportionalVec, -angleToYZ);

	return RotateToSeed(proportionalVec);
}

sf::Vector3f MyMat::RotateZ(sf::Vector3f vector, double angle)
{
	return RotateZ(vector, sin(angle), cos(angle));
}

sf::Vector3f MyMat::RotateZ(sf::Vector3f vector, double sinAngle, double cosAngle)
{
	sf::Vector3f newVec;

	newVec.x = cosAngle * vector.x - sinAngle * vector.y;
	newVec.y = sinAngle * vector.x + cosAngle * vector.y;
	newVec.z = vector.z;

	return newVec;
}

sf::Vector3f MyMat::RotateY(sf::Vector3f vector, double angle)
{
	return RotateY(vector, sin(angle), cos(angle));
}

sf::Vector3f MyMat::RotateY(sf::Vector3f vector, double sinAngle, double cosAngle)
{
	sf::Vector3f newVec;

	newVec.x = cosAngle * vector.x + sinAngle * vector.z;
	newVec.y = vector.y;
	newVec.z = -sinAngle * vector.x + cosAngle * vector.z;

	return newVec;
}

sf::Vector3f MyMat::RotateX(sf::Vector3f vector, double angle)
{
	return RotateX(vector, sin(angle), cos(angle));
}

sf::Vector3f MyMat::RotateX(sf::Vector3f vector, double sinAngle, double cosAngle)
{
	sf::Vector3f newVec;

	newVec.x = vector.x;
	newVec.y = cosAngle * vector.y - sinAngle * vector.z;
	newVec.z = sinAngle * vector.y + cosAngle * vector.z;

	return newVec;
}