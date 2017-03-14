/*
 * SemanticAdaptation.h
 *
 *  Created on: Mar 3, 2017
 *      Author: kel
 */

#ifndef SRC_SEMANTICADAPTATION_H_
#define SRC_SEMANTICADAPTATION_H_

#include <iostream>
#include <list>
#include "Rule.h"
#include "Fmu.h"
using namespace std;
using namespace fmi2;
namespace adaptation
{

template<class T>
class SemanticAdaptation
{
public:
	SemanticAdaptation(shared_ptr<std::string> resourceLocation, shared_ptr<std::list<Rule<T>>>inRules, shared_ptr<std::list<Rule<T>>> outRules);
	virtual ~SemanticAdaptation();

	void executeControlFlow(double h, double dt);
	void executeInRules();
	void executeOutRules();

	enum MooreOrMealy
	{	Moore,Mealy};

	virtual void setFmiValue(int id , int value)=0;
	virtual void setFmiValue(int id , bool value)=0;
	virtual void setFmiValue(int id , double value)=0;

	virtual int getFmiValueInteger(int id )=0;
	virtual bool getFmiValueBoolean(int id )=0;
	virtual double getFmiValueDouble(int id )=0;

protected:
	virtual void executeInternalControlFlow(double h, double dt)=0;
	void do_step(shared_ptr<Fmu>,double t,double H);

	MooreOrMealy machineType;

	void setValue(shared_ptr<Fmu>,int id , int value);
	void setValue(shared_ptr<Fmu>,int id , bool value);
	void setValue(shared_ptr<Fmu>,int id , double value);

	int getValueInteger(shared_ptr<Fmu>,int id );
	bool getValueBoolean(shared_ptr<Fmu>,int id );
	double getValueDouble(shared_ptr<Fmu>,int id );

	virtual T* getRuleThis() =0;

	shared_ptr<std::string> resourceLocation;
private:
	shared_ptr<std::list<Rule<T>>> inRules;
	shared_ptr<std::list<Rule<T>>> outRules;

	shared_ptr<std::list<Rule<T>>> enablesInRules;
	shared_ptr<std::list<Rule<T>>> enablesOutRules;

};

template<class T>
SemanticAdaptation<T>::SemanticAdaptation(shared_ptr<std::string> resourceLocation, shared_ptr<std::list<Rule<T>>>inRules, shared_ptr<std::list<Rule<T>>> outRules)
{
	this->resourceLocation = resourceLocation;
	this->machineType = Mealy;
	this->inRules = inRules;
	cout << "added "<< this->inRules->size() <<endl;
	this->outRules = outRules;

	this->enablesInRules = make_shared<std::list<Rule<T>>>();
	this->enablesOutRules = make_shared<std::list<Rule<T>>>();

}

template<class T>
SemanticAdaptation<T>::~SemanticAdaptation()
{
	// TODO Auto-generated destructor stub
}

template<class T>
void SemanticAdaptation<T>::executeInRules()
{
	//http://stackoverflow.com/questions/2898316/using-a-member-function-pointer-within-a-class
	for (auto itr = this->inRules->begin(), end = this->inRules->end(); itr != end; ++itr)
	{
		Rule<T> rule = *itr;
		if (((*getRuleThis()).*rule.condition)())
		{
			((*getRuleThis()).*rule.body)();
			if (this->machineType == Mealy)
			{
				((*getRuleThis()).*rule.flush)();
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
		Rule<T> rule = *itr;
		if (((*getRuleThis()).*rule.condition)())
		{
			((*getRuleThis()).*rule.body)();
			if (this->machineType == Mealy)
			{
				((*getRuleThis()).*rule.flush)();
			}
			this->enablesInRules->push_back(*itr);
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

}
/* namespace fmi2 */

#endif /* SRC_SEMANTICADAPTATION_H_ */
