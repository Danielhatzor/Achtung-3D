#pragma once

#include <string>
#include "Constants1.h"
#include "ClientCurve.h"

class LocalCurve : public ClientCurve
{
private:
	AbsoluteDirection	m_nAbsDirection;
	AbsoluteDirection	m_nLastAbsDirection;
	CurveKeys			m_ckKeys;

public:
	LocalCurve(int id, std::string& name, int colorIndex, const CurveKeys &keys);

	~LocalCurve();

	void updateMovementDirection();

	AbsoluteDirection getAbsDirection();

	CurveKeys GetKeys();

	bool isDirectionChanged();
};