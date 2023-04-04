#include <string>
#include <SFML\Graphics.hpp>
#include "Constants1.h"
#include "ClientCurve.h"
#include "LocalCurve.h"

LocalCurve::LocalCurve(int id, std::string& name, int colorIndex, const CurveKeys &keys)
	: ClientCurve(id, name, colorIndex),
	m_nAbsDirection(AbsoluteDirection::Straight),
	m_nLastAbsDirection(AbsoluteDirection::Straight),
	m_ckKeys(keys)
{
}

LocalCurve::~LocalCurve()
{

}

void LocalCurve::updateMovementDirection()
{
	m_nLastAbsDirection = m_nAbsDirection;

	if (sf::Keyboard::isKeyPressed(m_ckKeys.LeftKey))
		m_nAbsDirection = AbsoluteDirection::Left;
	else if (sf::Keyboard::isKeyPressed(m_ckKeys.RightKey))
		m_nAbsDirection = AbsoluteDirection::Right;
	else
		m_nAbsDirection = AbsoluteDirection::Straight;
}

AbsoluteDirection LocalCurve::getAbsDirection()
{
	return m_nAbsDirection;
}

CurveKeys LocalCurve::GetKeys()
{
	return m_ckKeys;
}

bool LocalCurve::isDirectionChanged()
{
	return m_nAbsDirection != m_nLastAbsDirection;
}