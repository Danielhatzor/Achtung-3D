#pragma once

#include <bitset>
#include <vector>
#include "Constants1.h"
#include "SCurve.h"


class CollisionMap
{
public:
	CollisionMap(std::vector<SCurve*>& collection);
	~CollisionMap();

	static void GetCollisionPoints(std::vector<sf::Vector3f>& arr, sf::Vector3f position, sf::Vector3f rotationAxis);

	void UpdateMap();
	bool At(sf::Vector3f vector) const;
	bool IsColliding(sf::Vector3f position, sf::Vector3f rotationAxis) const;
	bool IsColliding(SCurve& curve);
	void Reset();

private:
	const int LENGTH = Constants::SPHERE_RADIUS * 2 + 4;

	std::vector<std::vector < std::vector<short> > > m_bsMap;
	std::vector<SCurve*> &m_sccCurves;
	int m_nRoundNumber;
};