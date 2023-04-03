#pragma once

#include <string>
#include <stack>
#include <SFML\Graphics.hpp>
#include "ClientCurve.h"
#include "LocalCurve.h"


//A drawable collection of curves on one 2D surface
class CurveCollection
{
private:
	std::vector<ClientCurve*>	m_aCurves;
	std::vector<LocalCurve*>	m_aLocalCurves;
	std::stack<CurveKeys>		m_stkCurveKeyPairs;

public:
	CurveCollection();

	~CurveCollection();

	//Return a reference to a curve in the collection accoding to a given ID
	ClientCurve& operator[](const int curveIndex);

	//Add a curve to the collection
	void AddCurve(int id, std::string& name, int colorIndex, bool local);

	void RemoveCurve(int id);

	int getLength();

	std::vector<ClientCurve*>& getCurves();

	std::vector<LocalCurve*>& getLocals();

	std::vector<ClientCurve*> CurveCollection::GetCurvesByScores();

	//Update each of the curves in the collection
	void updateDirections();

	void Reset();
};