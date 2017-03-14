/*
 * SemanticAdaptation.cpp
 *
 *  Created on: Mar 3, 2017
 *      Author: kel
 */

#include "SemanticAdaptation.h"

namespace adaptation
{

template<class T>
SemanticAdaptation<T>::SemanticAdaptation(shared_ptr<std::string> resourceLocation,shared_ptr<std::list<Rule<T>>>inRules, shared_ptr<std::list<Rule<T>>> outRules)
{
	this->resourceLocation = resourceLocation;
	this->machineType = Mealy;
	this->inRules = inRules;
	this->outRules = outRules;

}

template<class T>
SemanticAdaptation<T>::~SemanticAdaptation()
{
	// TODO Auto-generated destructor stub
}

template<class T>
void SemanticAdaptation<T>::executeInRules()
{
	for (auto itr = this->inRules->begin(), end = this->inRules->end(); itr != end; ++itr)
	{

		if (itr->condition())
		{
			itr->body();
			if (this->machineType == Mealy)
			{
				itr->flush();
			}
			this->enablesInRules->push_back(*itr);
		}
	}
}

template<class T>
void SemanticAdaptation<T>::executeOutRules()
{
	for (auto itr = this->outRules->begin(), end = this->outRules->end(); itr != end; ++itr)
	{

		if (itr->condition())
		{
			itr->body();
			if (this->machineType == Mealy)
			{
				itr->flush();
			}
			this->enablesOutRules->push_back(*itr);
		}
	}
}

template<class T>
void SemanticAdaptation<T>::executeControlFlow(double h, double dt)
{
	this->enablesOutRules->clear();

	executeInternalControlFlow(h, dt);

	this->enablesInRules->clear();
}

template<class T>
void SemanticAdaptation<T>::setValue(shared_ptr<Fmu>, int id, int value)
{
}

template<class T>
void SemanticAdaptation<T>::setValue(shared_ptr<Fmu>, int id, bool value)
{
}

template<class T>
void SemanticAdaptation<T>::setValue(shared_ptr<Fmu>, int id, double value)
{
}

template<class T>
void SemanticAdaptation<T>::do_step(shared_ptr<Fmu> fmu, double t, double H)
{

}

template<class T>
int SemanticAdaptation<T>::getValueInteger(shared_ptr<Fmu>, int id)
{
	return 0;
}
template<class T>
bool SemanticAdaptation<T>::getValueBoolean(shared_ptr<Fmu>, int id)
{
	return false;
}
template<class T>
double SemanticAdaptation<T>::getValueDouble(shared_ptr<Fmu>, int id)
{
	return 0.0;
}

} /* namespace fmi2 */
