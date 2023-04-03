#include <math.h>
#include <bitset>
#include "Constants1.h"
#include "CollisionMap.h"
#include "SCurve.h"
#include "MyMat.h"


CollisionMap::CollisionMap(std::vector<SCurve*>& collection) 
	: m_sccCurves(collection),
	m_nRoundNumber(1)
{
	m_bsMap.resize(LENGTH);

	for (int i = 0; i < LENGTH; ++i)
	{
		m_bsMap[i].resize(LENGTH);

		for (int j = 0; j < LENGTH; ++j)
		{
			m_bsMap[i][j].resize(LENGTH);
		}
	}
}

CollisionMap::~CollisionMap()
{
}

void CollisionMap::GetCollisionPoints(std::vector<sf::Vector3f>& arr, sf::Vector3f position, sf::Vector3f rotationAxis)
{
	// Changing current axis by 90, to start from right
	MyMat mat(position);
	sf::Vector3f proportionlAxis = mat.UnRotateFromSeed(rotationAxis);
	proportionlAxis = MyMat::RotateZ(proportionlAxis, Constants::PI / 2);

	for (int i = 0; i < Constants::COLLISION_CHECKS_PER_STEP; ++i)
	{
		// Getting new axis
		sf::Vector3f newAxis = mat.RotateToSeed(proportionlAxis);

		// Incrementing position by 2 head radiuses, searching for collision
		MyMat mat1(newAxis);
		sf::Vector3f newPosition = mat1.UnRotateFromSeed(position);
		newPosition = MyMat::RotateZ(newPosition, atan2(Constants::CURVE_HEAD_RADIUS * 2 - 2, Constants::SPHERE_RADIUS));
		newPosition = mat1.RotateToSeed(newPosition);

		// Generating 3 points on the vector
		for (int i = -1; i <= 1; ++i)
		{
			sf::Vector3f tempPos(0, 0, Constants::SPHERE_RADIUS + i);
			MyMat tempMat(newPosition);
			tempPos = tempMat.RotateToSeed(tempPos);

			arr.push_back(tempPos);
		}

		// Changing rotation axis to next collision point
		proportionlAxis = MyMat::RotateZ(proportionlAxis, -Constants::PI / (Constants::COLLISION_CHECKS_PER_STEP - 1));
	}
}

bool CollisionMap::At(sf::Vector3f vector) const
{
	return m_bsMap[vector.z + LENGTH / 2]
		[vector.y + LENGTH / 2]
		[vector.x + LENGTH / 2] == m_nRoundNumber;
}

bool CollisionMap::IsColliding(sf::Vector3f position, sf::Vector3f rotationAxis) const
{
	std::vector<sf::Vector3f> positions;
	
	positions.reserve(Constants::COLLISION_CHECKS_PER_STEP * 3);
	GetCollisionPoints(positions, position, rotationAxis);

	for (auto& vector : positions)
	{
		if (At(vector))
		{
			return true;
		}
	}

	return false;
}

bool CollisionMap::IsColliding(SCurve& curve)
{
	return IsColliding(curve.getPosition(), curve.getRotationAxis());
}

void CollisionMap::UpdateMap()
{
	for (SCurve* curve : m_sccCurves)
	{
		if (curve->isAlive() && !curve->hasGodMod())
		{
			sf::Vector3f position = curve->getPosition();
			int z = round(position.z) + LENGTH / 2;
			int y = round(position.y) + LENGTH / 2;
			int x = round(position.x) + LENGTH / 2;

			m_bsMap[z][y][x] = m_nRoundNumber;
		}
	}
}

void CollisionMap::Reset()
{
	++m_nRoundNumber;
}