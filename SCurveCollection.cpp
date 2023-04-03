#include <cstdlib>
#include <ctime>
#include <cassert>
#include <vector>
#include <stack>
#include <string>
#include "SCurveCollection.h"
#include "SCurve.h"
#include "CollisionMap.h"
#include "Bot.h"
#include "MyMat.h"

SCurveCollection::SCurveCollection()
	: m_cmMap(GetCurves()),
	m_nNextId(0)
{
	srand(static_cast<unsigned int>(time(0)));
	rand();

	for (int i = Constants::MAX_CURVES - 1; i >= 0; --i)
	{
		m_stkCurveColors.push(i);
	}
}

SCurveCollection::~SCurveCollection()
{
	for (SCurve* curve : m_aCurves)
	{
		delete curve;
	}
}

int SCurveCollection::GetLength()
{
	return m_aCurves.size();
}

SCurve& SCurveCollection::operator[](int id)
{
	for (SCurve* curve : m_aCurves)
	{
		if (curve->getID() == id)
		{
			return *curve;
		}
	}

	assert(false);
}

bool SCurveCollection::HasId(int id)
{
	for (SCurve* curve : m_aCurves)
	{
		if (curve->getID() == id)
		{
			return true;
		}
	}

	return false;
}

std::vector<SCurve*>& SCurveCollection::GetCurves()
{
	return m_aCurves;
}

const CollisionMap& SCurveCollection::GetCollisionMap() const
{
	return m_cmMap;
}

int SCurveCollection::AddCurve(std::string& name, bool isBot)
{
	// Creating curve
	SCurve* curve;
	
	if (isBot)
	{
		curve = new Bot(m_nNextId, "Bot " + std::to_string(m_nNextId), m_stkCurveColors.top());
	}
	else
	{
		curve = new SCurve(m_nNextId, "Player " + std::to_string(m_nNextId), m_stkCurveColors.top());
	}

	m_aCurves.push_back(curve);
	m_aRoundWinners.push_back(curve);
	m_stkCurveColors.pop();
	++m_nNextId;

	return m_nNextId - 1;
}

void SCurveCollection::RemoveCurve(int id)
{
	for (int i = 0; i < m_aCurves.size(); ++i)
	{
		if (m_aCurves.at(i)->getID() == id)
		{
			m_stkCurveColors.push(m_aCurves.at(i)->getColorIndex());
			delete m_aCurves.at(i);
			m_aCurves.erase(m_aCurves.begin() + i);
			m_aRoundWinners.erase(m_aRoundWinners.begin() + i);
		}
	}
}

void SCurveCollection::UpdatePositions()
{
	for (SCurve* curve : m_aCurves)
	{
		if (curve->isAlive())
		{
			curve->step();
		}
	}

	m_cmMap.UpdateMap();
}

void SCurveCollection::KillCollidingCurves()
{
	for (SCurve* curve : m_aCurves)
	{
		if (curve->isAlive() && !curve->hasGodMod())
		{
			if (m_cmMap.IsColliding(*curve))
			{
				curve->setAlive(false);

				// Updating alive curves
				if (m_aRoundWinners.size() > 1)
				{
					for (int i = m_aRoundWinners.size() - 1; i >= 0; --i)
					{
						if (m_aRoundWinners[i] == curve)
						{
							m_aRoundWinners.erase(m_aRoundWinners.begin() + i);
						}
						else
						{
							++(m_aRoundWinners[i]->points);
						}
					}
				}
			}
		}
	}
}

void SCurveCollection::ApplyGodMods(double chance)
{
	int max = 100 / chance;

	for (SCurve* curve : m_aCurves)
	{
		if (curve->isAlive() && !curve->hasGodMod())
		{
			if (rand() % max == 0)
			{
				curve->applyGodMod();
			}
		}
	}
}

void SCurveCollection::Reset()
{
	for (SCurve* curve : GetCurves())
	{
		curve->randomPosition();
		curve->setAlive(true);
		curve->applyGodMod(15);
	}

	m_cmMap.Reset();

	m_aRoundWinners = GetCurves();
}

SCurve* SCurveCollection::GetRoundWinner()
{
	if (m_aRoundWinners.size() == 1)
	{
		return m_aRoundWinners[0];
	}

	return nullptr;
}

SCurve*	SCurveCollection::GetMatchWinner()
{
	auto& curves = GetCurves();
	SCurve* first = nullptr;
	SCurve* second = nullptr;

	for (SCurve* curve : curves)
	{
		if (!first || curve->points > first->points)
		{
			second = first;
			first = curve;
		}
		else if (!second || curve->points > second->points)
		{
			second = curve;
		}
	}

	if (first->points - 1 > second->points &&
		first->points >= Constants::MIN_ROUNDS_TO_WIN * (curves.size() - 1))
	{
		return first;
	}

	return nullptr;
}