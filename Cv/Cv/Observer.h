#pragma once

enum ObserverAction
{
	POS_LIGHT,
	CAMERA
};

class Observer
{
public:
	virtual void Update(ObserverAction action, void* object) = 0;
};

