#pragma once
#include <vector>
#include "Observer.h"

/*
*	https://www.infoworld.com/article/2077258/observer-and-observable.html
*/

class Observable
{
protected:
	std::vector<Observer*> observers;

public:
	void AttachObserver(Observer* observer);
	void DettachObserver(Observer* observer);
	virtual void NotifyObservers() = 0;

};

