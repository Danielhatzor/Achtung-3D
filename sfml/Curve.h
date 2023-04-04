#pragma once

#include <string>
#include <SFML\Graphics.hpp>

class Curve
{
public:
	int points;
	float previewAngle;

	Curve(int id, std::string& name, int colorIndex);
	~Curve();

	virtual bool hasGodMod() = 0;

	// Set the position of the curve
	virtual void setPosition(sf::Vector3f position);

	virtual sf::Vector3f getPosition();

	virtual sf::Vector3f getRotationAxis();

	virtual void setRotationAxis(sf::Vector3f vector);

	// Return true if the curve is alive and needs updating/drawing, otherwise false
	virtual bool isAlive();

	// Kill the curve
	virtual void setAlive(bool isAlive);

	// Return the unique id of the curve
	virtual int getID();

	virtual const std::string& getName();

	virtual int getColorIndex();

protected:
	const int ID;

	sf::Vector3f	m_v3Position;
	sf::Vector3f	m_v3RotationAxis;
	std::string		m_strName;
	bool			m_bAlive;
	int				m_nColorIndex;
};