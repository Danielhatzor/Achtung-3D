#pragma once

#include <string>
#include <vector>
#include <stack>
#include "SCurve.h"
#include "CollisionMap.h"

class SCurveCollection
{
public:
	SCurveCollection();
	~SCurveCollection();

	int GetLength();
	SCurve& operator[](int id);
	bool HasId(int id);
	std::vector<SCurve*>& GetCurves();
	const CollisionMap& GetCollisionMap() const;
	int AddCurve(std::string& name, bool isBot);
	void RemoveCurve(int id);
	void UpdatePositions();
	void KillCollidingCurves();
	void ApplyGodMods(double chance);
	void Reset();
	SCurve* GetRoundWinner();
	SCurve*	GetMatchWinner();
private:
	int						m_nNextId;
	CollisionMap			m_cmMap;
	std::vector<SCurve*>	m_aCurves;
	std::vector<SCurve*>	m_aRoundWinners;
	std::stack<int>			m_stkCurveColors;
};
