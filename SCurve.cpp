#include <SFML\Graphics.hpp>
#include "Constants1.h"
#include "SCurve.h"
#include "MyMat.h"
#include "Patterns.h"

SCurve::SCurve(int id, std::string& name, int colorIndex)
	: Curve(id, name, colorIndex),
	m_nColorIndex(colorIndex),
	m_nLastGodModPoints(0),
	stepsTillNewDirection(Constants::CURVE_BOT_MAX_RAY_LEN),
	m_nMovingTo(AbsoluteDirection::Straight) // 3 seconds
	//m_v3Position(position)
{
	randomPosition();
	applyGodMod(15);
	//futurePositions.futurePositions[0] = new FuturePositions();
}

SCurve::~SCurve()
{
}

sf::Vector3f SCurve::getNextStep(sf::Vector3f position, sf::Vector3f rotationAxis)
{
	static double rotationAngle = atan2(1, Constants::SPHERE_RADIUS);

	MyMat mat(rotationAxis);
	sf::Vector3f proportionalPos = mat.UnRotateFromSeed(position);
	proportionalPos = MyMat::RotateZ(proportionalPos, rotationAngle);

	return mat.RotateToSeed(proportionalPos);
}

sf::Vector3f SCurve::changeRotAxis(sf::Vector3f position, sf::Vector3f rotationAxis, AbsoluteDirection direction)
{
	MyMat mat(position);
	sf::Vector3f proportionalAxis = mat.UnRotateFromSeed(rotationAxis);
	proportionalAxis = MyMat::RotateZ(proportionalAxis, Constants::CURVE_ANGULAR_SPEED * Constants::DEG_TO_RADS * (int)direction);

	return mat.RotateToSeed(proportionalAxis);
}

void SCurve::step()
{
	// Update position
	setPosition(getNextStep(getPosition(), getRotationAxis()));
	//setPosition(GetStep(m_nMovingTo, 1));

	// Update angle
	setRotationAxis(changeRotAxis(getPosition(), getRotationAxis(), m_nMovingTo));
	previewAngle += (int)m_nMovingTo * Constants::CURVE_ANGULAR_SPEED;

	// Update godmod points
	m_nLastGodModPoints = m_nGodModPoints;
	if (m_nGodModPoints)
	{
		--m_nGodModPoints;
	}

	stepsTillNewDirection--;
}

void SCurve::setMovementDirection(AbsoluteDirection direction)
{
	m_nMovingTo = direction;
}

bool SCurve::hasGodMod()
{
	return m_nGodModPoints > 0;
}

bool SCurve::isGodModChanged()
{
	return ((m_nGodModPoints == 0 && m_nLastGodModPoints > 0) ||
			(m_nGodModPoints > m_nLastGodModPoints));
}

void SCurve::applyGodMod(int factor)
{
	m_nLastGodModPoints = m_nGodModPoints;
	m_nGodModPoints = Constants::CURVE_GOD_MOD_POINTS * factor;
}

void SCurve::randomPosition()
{
	// Generating a random vector on the sphere;
	double x = rand() % (int)(Constants::SPHERE_RADIUS * 2) - Constants::SPHERE_RADIUS;
	double y = rand() % (int)(Constants::SPHERE_RADIUS * 2) - Constants::SPHERE_RADIUS;
	double z = rand() % (int)(Constants::SPHERE_RADIUS * 2) - Constants::SPHERE_RADIUS;
	double length = sqrt(x * x + y * y + z * z);
	double scale = Constants::SPHERE_RADIUS / length;
	sf::Vector3f position(x * scale, y * scale, z * scale);

	// Generating a random rotation axis:
	MyMat mat(position);
	sf::Vector3f rotAxis(Constants::SPHERE_RADIUS, 0, 0);
	rotAxis = MyMat::RotateZ(rotAxis, rand());
	rotAxis = mat.RotateToSeed(rotAxis);

	setPosition(position);
	setRotationAxis(rotAxis);
}