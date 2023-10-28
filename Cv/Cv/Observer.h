#pragma once

enum ObserverAction
{
	LIGHT,
	CAMERA
};

class Observer
{
public:
	virtual void Update(ObserverAction action, void* object) = 0;
};

