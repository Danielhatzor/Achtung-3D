#pragma once

class IMatch
{
public:
	virtual bool HasStarted() = 0;
	virtual void Start() = 0;
	virtual void InGameCycle() = 0;
	virtual void PreGameCycle() = 0;
};
