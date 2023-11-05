#pragma once

enum ObserverAction
{
	CAMERA,
	POS_LIGHT,
	DIR_LIGHT,
	REF_LIGHT
};

class Observer
{
public:
	virtual void Update(ObserverAction action, void* object) = 0;
};

