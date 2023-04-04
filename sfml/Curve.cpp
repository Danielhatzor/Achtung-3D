#include <iostream>
#include <string>
#include "Curve.h"

Curve::Curve(int id, std::string& name, int colorIndex)
	: ID(id),
	m_strName(name),
	m_nColorIndex(colorIndex),
	m_bAlive(true),
	points(0),
	previewAngle(0)
{
}

Curve::~Curve()
{
}

bool Curve::isAlive()
{
	return m_bAlive;
}

void Curve::setAlive(bool isAlive)
{
	m_bAlive = isAlive;
}

int Curve::getID()
{
	return ID;
}

const std::string& Curve::getName()
{
	return m_strName;
}

int Curve::getColorIndex()
{
	return m_nColorIndex;
}

void Curve::setPosition(sf::Vector3f position)
{
	m_v3Position = position;
}

sf::Vector3f Curve::getPosition()
{
	return m_v3Position;
}

sf::Vector3f Curve::getRotationAxis()
{
	return m_v3RotationAxis;
}

void Curve::setRotationAxis(sf::Vector3f vector)
{
	m_v3RotationAxis = vector;
}