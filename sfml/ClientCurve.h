#pragma once

#include <string>
#include <SFML\Graphics.hpp>
#include "Constants1.h"
#include "Curve.h"

class ClientCurve : public Curve
{
public:
	std::vector<std::vector<sf::Vector3f>>	m_aav3PathBounders;
protected:
	std::vector<sf::Vector3f>				m_av3Trajectory;
	bool									m_bHasGodMod;
	float									m_fPreviewAngle;

public:
	ClientCurve(int id, std::string& name, const int colorIndex);

	~ClientCurve();

	virtual void setPosition(sf::Vector3f position) override;

	virtual void setNewDirection(int stepID, AbsoluteDirection);

	virtual void setGodMod(bool on);

	virtual bool hasGodMod() override;

	virtual std::vector<sf::Vector3f>& getTrajectory();

	virtual float getPreviewAngle();

	virtual void clearPath();
};