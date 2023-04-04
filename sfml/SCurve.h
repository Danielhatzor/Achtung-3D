#pragma once

#include <array>
#include <string>
#include <SFML\Graphics.hpp>
#include "Constants1.h"
#include "Curve.h"
#include "FuturePositions.h"
#include "CurvePath.h"

class SCurve : public Curve
{
public:
	std::array<CurvePath, 3>	futurePaths;
	//CurvePath persistantPath;
	//FuturePositions futurePositions;
	AbsoluteDirection m_nMovingTo;
	int stepsTillNewDirection;

	SCurve(int id, std::string& name, int colorIndex);
	~SCurve();

	static sf::Vector3f getNextStep(sf::Vector3f position, sf::Vector3f rotationAxis);

	static sf::Vector3f changeRotAxis(sf::Vector3f position, sf::Vector3f rotationAxis, AbsoluteDirection direction);

	// Update the general direction (left, straight or right) of the curve, according to its pressed keys
	void setMovementDirection(AbsoluteDirection direction);

	int getAngle();

	virtual void step();

	virtual bool hasGodMod() override;
	virtual bool isGodModChanged();

	void applyGodMod(int factor = 1);

	void randomPosition();

protected:
	sf::Vector3f GetStep(AbsoluteDirection direction, int steps);

	///AbsoluteDirection			m_nMovingTo;
	int							m_nColorIndex;
	int							m_nGodModPoints;
	int							m_nLastGodModPoints;

};
