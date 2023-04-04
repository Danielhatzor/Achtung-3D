#include <string>
#include <stack>
#include "LocalCurve.h"
#include <SFML\Graphics.hpp>
#include <cassert>
#include "Constants1.h"
#include "CurveCollection.h"

CurveCollection::CurveCollection()
{
	for (auto KeyPair : Constants::PLAYER_KEYS)
	{
		m_stkCurveKeyPairs.push(KeyPair);
	}
}

CurveCollection::~CurveCollection()
{
	for (ClientCurve* curve : m_aCurves)
	{
		delete curve;
	}
}

ClientCurve& CurveCollection::operator[](const int id)
{
	for (auto curve : m_aCurves)
	{
		if (curve && curve->getID() == id)
		{
			return *curve;
		}
	}

	//assert(false);
}

void CurveCollection::AddCurve(int id, std::string& name, int colorIndex, bool local)
{
	if (local)
	{
		LocalCurve* NewCurve = new LocalCurve(id, name, colorIndex, m_stkCurveKeyPairs.top());

		m_stkCurveKeyPairs.pop();
		m_aLocalCurves.push_back(NewCurve);
		m_aCurves.push_back(NewCurve);
	}
	else
	{
		ClientCurve* NewCurve = new ClientCurve(id, name, colorIndex);
		m_aCurves.push_back(NewCurve);
	}
}

void CurveCollection::RemoveCurve(int id)
{
	ClientCurve* curve = nullptr;

	for (int i = 0; i < m_aCurves.size(); ++i)
	{
		if (m_aCurves[i]->getID() == id)
		{
			curve = m_aCurves[i];
			m_aCurves.erase(m_aCurves.begin() + i);
		}
	}

	for (int i = 0; i < m_aLocalCurves.size(); ++i)
	{
		if (m_aLocalCurves[i]->getID() == id)
		{
			m_stkCurveKeyPairs.push(m_aLocalCurves[i]->GetKeys());
			delete m_aLocalCurves[i];
			m_aLocalCurves.erase(m_aLocalCurves.begin() + i);

			return;
		}
	}

	delete curve;
}

int CurveCollection::getLength()
{
	return m_aCurves.size();
}

void CurveCollection::updateDirections()
{
	for (auto curve : m_aLocalCurves)
	{
		if (curve)
		{
			curve->updateMovementDirection();
		}
	}
}

std::vector<ClientCurve*>& CurveCollection::getCurves()
{
	return m_aCurves;
}

std::vector<LocalCurve*>& CurveCollection::getLocals()
{
	return m_aLocalCurves;
}

std::vector<ClientCurve*> CurveCollection::GetCurvesByScores()
{
	std::vector<ClientCurve*> list;

	for (ClientCurve* curve : getCurves())
	{
		bool inserted = false;

		for (int i = 0; i < list.size(); ++i)
		{
			if (curve->points > list[i]->points)
			{
				list.insert(list.begin() + i, curve);
				inserted = true;

				break;
			}
		}

		if (!inserted)
		{
			list.push_back(curve);
		}
	}

	return list;
}

void CurveCollection::Reset()
{
	for (ClientCurve* curve : m_aCurves)
	{
		curve->clearPath();
		curve->setAlive(true);
		curve->setGodMod(true);
	}
}