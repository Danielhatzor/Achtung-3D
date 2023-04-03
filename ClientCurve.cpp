#include <string>
#include <SFML\Graphics.hpp>
#include <SFML\Network\TcpSocket.hpp>
#include "ClientCurve.h"
#include "MyMat.h"

ClientCurve::ClientCurve(int id, std::string& name, int colorIndex)
	: Curve(id, name, colorIndex),
	m_bHasGodMod(true)
{
	m_fPreviewAngle = 0;
	Curve::setPosition(sf::Vector3f(0, 0, -1));
}
ClientCurve::~ClientCurve()
{
}

void ClientCurve::setPosition(sf::Vector3f position)
{
	if (isAlive())
	{
		Curve::setPosition(position);

		if (!hasGodMod())
		{
			m_av3Trajectory.push_back(position);

			if (m_aav3PathBounders.empty())
			{
				m_aav3PathBounders.resize(m_aav3PathBounders.size() + 1);
			}

			MyMat mat(m_v3RotationAxis);
			sf::Vector3f RightBorder = mat.LerpToSeed(position, Constants::CURVE_HEAD_RADIUS / (Constants::SPHERE_RADIUS * Constants::PI / 2));
			sf::Vector3f LeftBorder = mat.LerpToSeed(position, -Constants::CURVE_HEAD_RADIUS / (Constants::SPHERE_RADIUS * Constants::PI / 2));
			m_aav3PathBounders[m_aav3PathBounders.size() - 1].push_back(RightBorder);
			m_aav3PathBounders[m_aav3PathBounders.size() - 1].push_back(LeftBorder);
		}
	}
}

void ClientCurve::setNewDirection(int stepID, AbsoluteDirection)
{
	/*
	if (m_aav3PathBounders.size() / 2 < stepID + 1)
	{

	}
	*/
}

void ClientCurve::setGodMod(bool on)
{
	if (m_bHasGodMod && !on)
	{
		m_aav3PathBounders.resize(m_aav3PathBounders.size() + 1);
	}

	m_bHasGodMod = on;
}

bool ClientCurve::hasGodMod()
{
	return m_bHasGodMod;
}

std::vector<sf::Vector3f>& ClientCurve::getTrajectory()
{
	return m_av3Trajectory;
}

void ClientCurve::clearPath()
{
	m_av3Trajectory.clear();
	m_aav3PathBounders.clear();
}

float ClientCurve::getPreviewAngle()
{
	return m_fPreviewAngle;
}
