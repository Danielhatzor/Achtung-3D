#pragma once

#include "SCurve.h"
#include "CollisionMap.h"

class Bot : public SCurve
{
public:
	//CurvePath futurePath;

	Bot(int id, std::string& name, int colorIndex);
	~Bot();


private:
	int m_nRequiredRayLen;

	AbsoluteDirection ChooseDirection();
	virtual void step() override;


};