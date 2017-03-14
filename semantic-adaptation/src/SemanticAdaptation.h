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

	virtual void setFmiValue(fmi2ValueReference id , int value)=0;
	virtual void setFmiValue(fmi2ValueReference id , bool value)=0;
	virtual void setFmiValue(fmi2ValueReference id , double value)=0;

	virtual int getFmiValueInteger(fmi2ValueReference id )=0;
	virtual bool getFmiValueBoolean(fmi2ValueReference id )=0;
	virtual double getFmiValueDouble(fmi2ValueReference id )=0;

	void flushAllEnabledOutRules();

protected:
	virtual void executeInternalControlFlow(double h, double dt)=0;
	void do_step(shared_ptr<FmuComponent>,double t,double H);

	MooreOrMealy machineType;

	void setValue(shared_ptr<FmuComponent>,fmi2ValueReference id , int value);
	void setValue(shared_ptr<FmuComponent>,fmi2ValueReference id , bool value);
	void setValue(shared_ptr<FmuComponent>,fmi2ValueReference id , double value);

	int getValueInteger(shared_ptr<FmuComponent>,fmi2ValueReference id );
	bool getValueBoolean(shared_ptr<FmuComponent>,fmi2ValueReference id );
	double getValueDouble(shared_ptr<FmuComponent>,fmi2ValueReference id );

	virtual T* getRuleThis() =0;

	shared_ptr<std::string> resourceLocation;
private:
	shared_ptr<std::list<Rule<T>>> inRules;
	shared_ptr<std::list<Rule<T>>> outRules;

	shared_ptr<std::list<Rule<T>>> enablesInRules;
	shared_ptr<std::list<Rule<T>>> enablesOutRules;

	void flushAllEnabledInRules();


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
	if(this->enablesOutRules->size() >0)
	{
		//not sure why
		return;
	}

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
			this->enablesOutRules->push_back(*itr);
		}
	}
}

template<class T>
void SemanticAdaptation<T>::flushAllEnabledInRules()
{
	for (auto itr = this->enablesInRules->begin(), end = this->enablesInRules->end(); itr != end; ++itr)
	{
		Rule<T> rule = *itr;
		((*getRuleThis()).*rule.flush)();

	}
}

template<class T>
void SemanticAdaptation<T>::flushAllEnabledOutRules()
{
	for (auto itr = this->enablesOutRules->begin(), end = this->enablesOutRules->end(); itr != end; ++itr)
	{
		Rule<T> rule = *itr;
		((*getRuleThis()).*rule.flush)();

	}
}



template<class T>
void SemanticAdaptation<T>::executeControlFlow(double h, double dt)
{
	this->enablesOutRules->clear();

	//flush all enabled in-rules
	flushAllEnabledInRules();

	executeInternalControlFlow(h, dt);

	//execute the body of all out-rules with a true condition and enable them
	executeOutRules();

	this->enablesInRules->clear();
}

template<class T>
void SemanticAdaptation<T>::setValue(shared_ptr<FmuComponent> fmuComp, fmi2ValueReference id, int value)
{
	const fmi2ValueReference vr[]
	{ id };
	size_t nvr = 1;
	fmi2Integer v[]
	{ value };

	fmi2Status status = fmuComp->fmu->setInteger(fmuComp->component, vr, nvr, v);
	if (status != fmi2OK)
	{
		cerr << "setInteger failed: " << id << " " << status << endl;
	}
}

template<class T>
void SemanticAdaptation<T>::setValue(shared_ptr<FmuComponent> fmuComp, fmi2ValueReference id, bool value)
{
	const fmi2ValueReference vr[]
	{ id };
	size_t nvr = 1;
	fmi2Boolean v[]
	{ value };

	fmi2Status status = fmuComp->fmu->setBoolean(fmuComp->component, vr, nvr, v);
	if (status != fmi2OK)
	{
		cerr << "setBoolean failed: " << id << " " << status << endl;
	}
}

template<class T>
void SemanticAdaptation<T>::setValue(shared_ptr<FmuComponent> fmuComp, fmi2ValueReference id, double value)
{
	const fmi2ValueReference vr[]
	{ id };
	size_t nvr = 1;
	fmi2Real v[]
	{ value };

	fmi2Status status = fmuComp->fmu->setReal(fmuComp->component, vr, nvr, v);
	if (status != fmi2OK)
	{
		cerr << "setReal failed: " << id << " " << status << endl;
	}
}

template<class T>
void SemanticAdaptation<T>::do_step(shared_ptr<FmuComponent> fmuComp, double t, double H)
{
	fmi2Status status = fmuComp->fmu->doStep(fmuComp->component, t, H, false);
	if (status != fmi2OK)
	{
		cerr << "do_step failed: t: " << t << " h: " << H << " " << status << endl;
	}
}

template<class T>
int SemanticAdaptation<T>::getValueInteger(shared_ptr<FmuComponent> fmuComp, fmi2ValueReference id)
{
	const fmi2ValueReference vr[]
	{ id };
	size_t nvr = 1;
	fmi2Integer v[1];

	fmi2Status status = fmuComp->fmu->getInteger(fmuComp->component, vr, nvr, v);
	if (status != fmi2OK)
	{
		cerr << "getInteger failed: " << id << " " << status << endl;
	}

	return v[0];
}
template<class T>
bool SemanticAdaptation<T>::getValueBoolean(shared_ptr<FmuComponent> fmuComp, fmi2ValueReference id)
{
	const fmi2ValueReference vr[]
	{ id };
	size_t nvr = 1;
	fmi2Boolean v[1];

	fmi2Status status = fmuComp->fmu->getBoolean(fmuComp->component, vr, nvr, v);
	if (status != fmi2OK)
	{
		cerr << "getBoolean failed: " << id << " " << status << endl;
	}

	return v[0];
}
template<class T>
double SemanticAdaptation<T>::getValueDouble(shared_ptr<FmuComponent> fmuComp, fmi2ValueReference id)
{
	const fmi2ValueReference vr[]
	{ id };
	size_t nvr = 1;
	fmi2Real v[1];

	fmi2Status status = fmuComp->fmu->getReal(fmuComp->component, vr, nvr, v);
	if (status != fmi2OK)
	{
		cerr << "getReal failed: " << id << " " << status << endl;
	}

	return v[0];
}

}
/* namespace fmi2 */

#endif /* SRC_SEMANTICADAPTATION_H_ */
