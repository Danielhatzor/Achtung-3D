#pragma once

#include "IMatch.h"
#include "SConBox.h"
#include "SCurveCollection.h"

class SMatch : public IMatch
{
public:
	SMatch();
	~SMatch();

	virtual bool HasStarted() override;
	virtual void Start() override;
	virtual void InGameCycle() override;
	virtual void PreGameCycle() override;
private:
	bool				m_bHasStarted;
	SConBox				m_scbClients;
	SCurveCollection	m_sccCurves;
	int					m_nNewRoundTimeOut;
};