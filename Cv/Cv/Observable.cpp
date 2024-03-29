#include "Observable.h"

void Observable::AttachObserver(Observer* observer)
{
	observers.push_back(observer);
	this->NotifyObservers();
}

void Observable::DettachObserver(Observer* observer)
{
	observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
}