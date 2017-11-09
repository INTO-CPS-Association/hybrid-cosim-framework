/*
 * #%~
 * The Overture Abstract Syntax Tree
 * %%
 * Copyright (C) 2017 - 2014 Aarhus University
 * %%
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public
 * License along with this program.  If not, see
 * <http://www.gnu.org/licenses/gpl-3.0.html>.
 * #~%
 */
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
#include <vector>
#include <map>
#include <algorithm>
#include <math.h>
#include "Rule.h"
#include "Fmu.h"
using namespace std;
using namespace fmi2;
namespace adaptation
{

#define THROW_STATUS_EXCEPTION if(status== fmi2Fatal || status==fmi2Error) \
{\
	throw SemanticAdaptationFmiException(status);\
}

struct SemanticAdaptationFmiException: public exception
{
	fmi2Status status;
	SemanticAdaptationFmiException(fmi2Status status)
	{
		this->status = status;
	}

};

struct InternalSemanticAdaptationState
{
	fmi2FMUstate internalState;
	shared_ptr<std::map<fmi2Component, std::vector<fmi2FMUstate>>>instanceStates;
	double lastDt;
	double lastH;
};

template<class T>
class SemanticAdaptation: public fmi2::Callback
{
public:
	SemanticAdaptation(shared_ptr<std::string> fmiInstanceName, shared_ptr<std::string> resourceLocation,
			shared_ptr<std::list<Rule<T>>>inRules, shared_ptr<std::list<Rule<T>>> outRules,const fmi2CallbackFunctions *functions);
	virtual void initialize()=0;
	virtual ~SemanticAdaptation();

	virtual fmi2Status executeControlFlow(double h, double dt) final;
	fmi2Status executeInRules();
	fmi2Status executeOutRules();

	enum MooreOrMealy
	{	Moore,Mealy};

	enum ReactiveOrDelayed
	{	Reactive, Delayed};

	fmi2Status flushAllEnabledOutRules();

	fmi2Status getLastErrorState();
	shared_ptr<std::string> getLastErrorMessage();

	void log(fmi2String instanceName, fmi2Status status, fmi2String category, fmi2String message);
	fmi2Component getComponent();
	/*FMI Calls start*/

	virtual void setFmiValue(fmi2ValueReference id , int value)=0;
	virtual void setFmiValue(fmi2ValueReference id , bool value)=0;
	virtual void setFmiValue(fmi2ValueReference id , double value)=0;

	virtual int getFmiValueInteger(fmi2ValueReference id )=0;
	virtual bool getFmiValueBoolean(fmi2ValueReference id )=0;
	virtual double getFmiValueReal(fmi2ValueReference id )=0;

	fmi2Status fmi2SetupExperiment(fmi2Boolean toleranceDefined, fmi2Real tolerance, fmi2Real startTime,
	fmi2Boolean stopTimeDefined, fmi2Real stopTime);
	fmi2Status fmi2EnterInitializationMode();
	fmi2Status fmi2ExitInitializationMode();
	fmi2Status fmi2Terminate();

	fmi2Status fmi2GetFMUstate (fmi2Component, fmi2FMUstate*);
	fmi2Status fmi2SetFMUstate (fmi2Component, fmi2FMUstate);
	fmi2Status fmi2FreeFMUstate (fmi2Component, fmi2FMUstate*);
	/*FMI Calls END*/

	double getLastSuccessfulTime();

protected:
	virtual double executeInternalControlFlow(double t, double H);
	double do_step(shared_ptr<FmuComponent>,double t,double H);

	void save_state(shared_ptr<FmuComponent>);
	void rollback(shared_ptr<FmuComponent>);

	virtual fmi2FMUstate getInternalFMUState()=0;
	virtual void setInternalFMUState(fmi2FMUstate)=0;
	virtual void freeInternalFMUState(fmi2FMUstate)=0;

	const fmi2CallbackFunctions *fmiFunctions=NULL;
	MooreOrMealy machineType;
	ReactiveOrDelayed reactiveness;

	fmi2Status setValue(shared_ptr<FmuComponent>,fmi2ValueReference id , int value);
	fmi2Status setValue(shared_ptr<FmuComponent>,fmi2ValueReference id , bool value);
	fmi2Status setValue(shared_ptr<FmuComponent>,fmi2ValueReference id , double value);

	int getValueInteger(shared_ptr<FmuComponent>,fmi2ValueReference id );
	bool getValueBoolean(shared_ptr<FmuComponent>,fmi2ValueReference id );
	double getValueDouble(shared_ptr<FmuComponent>,fmi2ValueReference id );

	double getNextTimeStep(shared_ptr<FmuComponent>);

	virtual T* getRuleThis() =0;

	fmi2Status lastErrorState=fmi2OK;
	shared_ptr<std::string> lastErrorMessage;

	shared_ptr<std::string> resourceLocation;
	shared_ptr<std::string> fmiInstanceName;

	shared_ptr<std::list<shared_ptr<FmuComponent>>> instances;
private:
	shared_ptr<std::list<Rule<T>>> inRules;
	shared_ptr<std::list<Rule<T>>> outRules;

	shared_ptr<std::list<Rule<T>>> enablesInRules;
	shared_ptr<std::list<Rule<T>>> enablesOutRules;

	void flushAllEnabledInRules();

	double lastSuccessfulTime = 0.0;

	shared_ptr<std::map<fmi2Component,std::vector<fmi2FMUstate>>> instanceStates;

	fmi2Status freeInternalFmuStates(shared_ptr<std::map<fmi2Component,std::vector<fmi2FMUstate>>> instanceStates);
	shared_ptr<std::map<fmi2Component,std::vector<fmi2FMUstate>>> cloneInstanceStates(shared_ptr<std::map<fmi2Component,std::vector<fmi2FMUstate>>> instanceStates);

	double lastDt = 0.0;
	double lastH = 0.0;

};

template<class T>
SemanticAdaptation<T>::SemanticAdaptation(shared_ptr<std::string> fmiInstanceName,
		shared_ptr<std::string> resourceLocation, shared_ptr<std::list<Rule<T>>>inRules, shared_ptr<std::list<Rule<T>>> outRules,const fmi2CallbackFunctions *functions)
{
	this->fmiInstanceName = fmiInstanceName;
	this->resourceLocation = resourceLocation;
	this->machineType = Mealy;
	this->reactiveness = Reactive;
	this->inRules = inRules;
	this->outRules = outRules;

	if(this->inRules==NULL)
	{
		this->inRules = make_shared<std::list<Rule<T>>>();
	}
	if(this->outRules==NULL)
	{
		this->outRules = make_shared<std::list<Rule<T>>>();
	}

	cout << "added "<< this->inRules->size() <<endl;
	this->fmiFunctions = functions;

	this->enablesInRules = make_shared<std::list<Rule<T>>>();
	this->enablesOutRules = make_shared<std::list<Rule<T>>>();
	this->instances = make_shared<std::list<shared_ptr<FmuComponent>>>();
	this->instanceStates = make_shared<std::map<fmi2Component,std::vector<fmi2FMUstate>>>();

}

template<class T>
SemanticAdaptation<T>::~SemanticAdaptation()
{
	// TODO Auto-generated destructor stub
}

template<class T>
double SemanticAdaptation<T>::executeInternalControlFlow(double t, double H)
{
	return lastSuccessfulTime + H;
}

template<class T>
fmi2Status SemanticAdaptation<T>::executeInRules()
{
	//http://stackoverflow.com/questions/2898316/using-a-member-function-pointer-within-a-class
	for (auto itr = this->inRules->begin(), end = this->inRules->end(); itr != end; ++itr)
	{
		Rule<T> rule = *itr;
		if (((*getRuleThis()).*rule.condition)(this->lastDt,this->lastH))
		{
			((*getRuleThis()).*rule.body)(this->lastDt,this->lastH);
			if (this->machineType == Mealy)
			{
				((*getRuleThis()).*rule.flush)(this->lastDt,this->lastH);
			}
			this->enablesInRules->push_back(*itr);
		}
	}
	return this->lastErrorState;
}

template<class T>
fmi2Status SemanticAdaptation<T>::executeOutRules()
{
	if (this->enablesOutRules->size() > 0)
	{
		//not sure why
		return fmi2OK;
	}

	for (auto itr = this->outRules->begin(), end = this->outRules->end(); itr != end; ++itr)
	{
		Rule<T> rule = *itr;
		if (((*getRuleThis()).*rule.condition)(this->lastDt,this->lastH))
		{
			((*getRuleThis()).*rule.body)(this->lastDt,this->lastH);
			if (this->machineType == Mealy)
			{
				((*getRuleThis()).*rule.flush)(this->lastDt,this->lastH);
			}
			this->enablesOutRules->push_back(*itr);
		}
	}

	return this->lastErrorState;
}

template<class T>
void SemanticAdaptation<T>::flushAllEnabledInRules()
{
	for (auto itr = this->enablesInRules->begin(), end = this->enablesInRules->end(); itr != end; ++itr)
	{
		Rule<T> rule = *itr;
		((*getRuleThis()).*rule.flush)(this->lastDt,this->lastH);

	}
}

template<class T>
fmi2Status SemanticAdaptation<T>::flushAllEnabledOutRules()
{
	for (auto itr = this->enablesOutRules->begin(), end = this->enablesOutRules->end(); itr != end; ++itr)
	{
		Rule<T> rule = *itr;
		((*getRuleThis()).*rule.flush)(this->lastDt,this->lastH);

	}

	return this->lastErrorState;
}

template<class T>
double SemanticAdaptation<T>::getLastSuccessfulTime()
{
	return this->lastSuccessfulTime;
}

template<class T>
fmi2Status SemanticAdaptation<T>::executeControlFlow(double t, double H)
{
	this->enablesOutRules->clear();

	//flush all enabled in-rules
	flushAllEnabledInRules();

	lastSuccessfulTime = executeInternalControlFlow(t, H);

	//execute the body of all out-rules with a true condition and enable them
	executeOutRules();

	this->enablesInRules->clear();
	return this->lastErrorState;
}

template<class T>
fmi2Status SemanticAdaptation<T>::setValue(shared_ptr<FmuComponent> fmuComp, fmi2ValueReference id, int value)
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
		this->lastErrorState = status;
		THROW_STATUS_EXCEPTION;
	}
	return status;
}

template<class T>
fmi2Status SemanticAdaptation<T>::setValue(shared_ptr<FmuComponent> fmuComp, fmi2ValueReference id, bool value)
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
		this->lastErrorState = status;
		THROW_STATUS_EXCEPTION;
	}
	return status;
}

template<class T>
fmi2Status SemanticAdaptation<T>::setValue(shared_ptr<FmuComponent> fmuComp, fmi2ValueReference id, double value)
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
		this->lastErrorState = status;
		THROW_STATUS_EXCEPTION;
	}

	return status;
}

template<class T>
double SemanticAdaptation<T>::do_step(shared_ptr<FmuComponent> fmuComp, double t, double H)
{
	fmi2Status status = fmuComp->fmu->doStep(fmuComp->component, t, H, false);
	if (status != fmi2OK)
	{
		cerr << "do_step failed: t: " << t << " h: " << H << " " << status << endl;
		this->lastErrorState = status;
		if (status == fmi2Discard)
		{
			double h;
			status = fmuComp->fmu->getRealStatus(fmuComp->component, fmi2StatusKind::fmi2LastSuccessfulTime, &h);
			if (status == fmi2OK)
			{
				return h;
			} else
			{
				cerr << "fmi2getRealStatus failed: t: " << t << " h: " << H << " " << status << endl;
				this->lastErrorState = status;
				THROW_STATUS_EXCEPTION;
			}
		} else
		{
			THROW_STATUS_EXCEPTION;
		}
	}

	this->lastDt = t;
	this->lastH = H;
	status = this->executeOutRules();
	if (status != fmi2OK)
	{
		cerr << "do_step-executeOutRiles failed: t: " << t << " h: " << H << " " << status << endl;
		this->lastErrorState = status;
		THROW_STATUS_EXCEPTION;
	}

	return H;
}

template<class T>
void SemanticAdaptation<T>::save_state(shared_ptr<FmuComponent> fmuComp)
{

	fmi2FMUstate state;

	auto status = fmuComp->fmu->getFMUstate(fmuComp->component, &state);
	if (status != fmi2OK)
	{
		cerr << "save_state failed: " << status << endl;
		this->lastErrorState = status;
		THROW_STATUS_EXCEPTION;
	} else
	{
		auto itr = this->instanceStates->find(fmuComp->component);
		if (itr != this->instanceStates->end())
		{
			itr->second.push_back(state);
		}
	}
}

template<class T>
void SemanticAdaptation<T>::rollback(shared_ptr<FmuComponent> fmuComp)
{
	auto itr = this->instanceStates->find(fmuComp->component);
	if (itr != this->instanceStates->end())
	{
		if (!itr->second.empty())
		{
			auto state = itr->second.back();

			auto status = fmuComp->fmu->setFMUstate(fmuComp->component, state);
			if (status != fmi2OK)
			{
				cerr << "rollback failed: " << status << endl;
				this->lastErrorState = status;
				THROW_STATUS_EXCEPTION;
			}

			if (fmuComp->fmu->freeFMUstate(fmuComp->component, &state) != fmi2OK)
			{
				//TODO handle error for set state
			}

			itr->second.pop_back();
		}
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
		this->lastErrorState = status;
		THROW_STATUS_EXCEPTION;
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
		this->lastErrorState = status;
		THROW_STATUS_EXCEPTION;
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
		this->lastErrorState = status;
		THROW_STATUS_EXCEPTION;
	}

	return v[0];
}

template<class T>
fmi2Status SemanticAdaptation<T>::getLastErrorState()
{
	return this->lastErrorState;
}
template<class T>
shared_ptr<std::string> SemanticAdaptation<T>::getLastErrorMessage()
{
	return this->lastErrorMessage;
}

template<class T>
fmi2Status SemanticAdaptation<T>::fmi2SetupExperiment(fmi2Boolean toleranceDefined, fmi2Real tolerance,
		fmi2Real startTime, fmi2Boolean stopTimeDefined, fmi2Real stopTime)
{

	fmi2Status status = fmi2OK;
	for (auto itr = this->instances->begin(), end = this->instances->end(); itr != end; ++itr)
	{
		auto comp = (*itr)->component;
		//prepare state cache
		this->instanceStates->insert(std::make_pair(comp, std::vector<fmi2FMUstate>()));

		//setup experiment
		status = (*itr)->fmu->setupExperiment(comp, toleranceDefined, tolerance, startTime, stopTimeDefined, stopTime);

		if (status != fmi2OK)
		{
			THROW_STATUS_EXCEPTION;
			return status;
		}

	}

	return this->lastErrorState;
}

template<class T>
fmi2Status SemanticAdaptation<T>::fmi2EnterInitializationMode()
{
	fmi2Status status = fmi2OK;
	for (auto itr = this->instances->begin(), end = this->instances->end(); itr != end; ++itr)
	{
		status = (*itr)->fmu->enterInitializationMode((*itr)->component);

		if (status != fmi2OK)
		{
			THROW_STATUS_EXCEPTION;
			return status;
		}

	}
	return this->lastErrorState;
}
template<class T>
fmi2Status SemanticAdaptation<T>::fmi2ExitInitializationMode()
{
	fmi2Status status = fmi2OK;
	for (auto itr = this->instances->begin(), end = this->instances->end(); itr != end; ++itr)
	{
		status = (*itr)->fmu->exitInitializationMode((*itr)->component);

		if (status != fmi2OK)
		{
			THROW_STATUS_EXCEPTION;
			return status;
		}

	}
	return this->lastErrorState;
}

template<class T>
fmi2Status SemanticAdaptation<T>::freeInternalFmuStates(shared_ptr<std::map<fmi2Component, std::vector<fmi2FMUstate>>>instanceStates)
{
	fmi2Status status = fmi2OK;
	for (auto itr = this->instances->begin(), end = this->instances->end(); itr != end; ++itr)
	{
		auto comp = (*itr)->component;
		//release any stored states
		auto stateItr = this->instanceStates->find(comp);
		if (stateItr != this->instanceStates->end())
		{
			auto states = stateItr->second;
			while (!states.empty())
			{
				auto state = states.back();
				states.pop_back();
				auto res = (*itr)->fmu->freeFMUstate(comp, &state);
				if (res != fmi2OK)
				{
					//TODO report free state error
					status = res;
				}
			}
		}
	}
	return status;
}

template<class T>
fmi2Status SemanticAdaptation<T>::fmi2Terminate()
{
	fmi2Status status = this->freeInternalFmuStates(this->instanceStates);

	for (auto itr = this->instances->begin(), end = this->instances->end(); itr != end; ++itr)
	{
		auto comp = (*itr)->component;
		// terminate
		status = (*itr)->fmu->terminate(comp);

		if (status != fmi2OK)
		{
			THROW_STATUS_EXCEPTION;
			return status;
		}

	}
	return this->lastErrorState;
}

template<class T>
fmi2Status SemanticAdaptation<T>::fmi2GetFMUstate(fmi2Component, fmi2FMUstate* statePtr)
{
	InternalSemanticAdaptationState* s = new InternalSemanticAdaptationState();
	*statePtr = (fmi2FMUstate*) s;
	s->internalState = this->getInternalFMUState();
	s->instanceStates = this->cloneInstanceStates(this->instanceStates);
	s->lastDt = this->lastDt;
	s->lastH = this->lastH;
	return fmi2OK;
}

template<class T>
fmi2Status SemanticAdaptation<T>::fmi2SetFMUstate(fmi2Component, fmi2FMUstate state)
{
	InternalSemanticAdaptationState* s = (InternalSemanticAdaptationState*) state;
	this->setInternalFMUState(s->internalState);
	this->freeInternalFmuStates(this->instanceStates);
	this->instanceStates = this->cloneInstanceStates(s->instanceStates);
	this->lastDt = s->lastDt;
	this->lastH = s->lastH;
	return fmi2OK;
}

template<class T>
fmi2Status SemanticAdaptation<T>::fmi2FreeFMUstate(fmi2Component, fmi2FMUstate* statePtr)
{
	InternalSemanticAdaptationState* s = (InternalSemanticAdaptationState*) *statePtr;
	this->freeInternalFMUState(s->internalState);
	this->freeInternalFmuStates(s->instanceStates);
	*statePtr=NULL;
	return fmi2OK;
}

template<class T>
shared_ptr<std::map<fmi2Component, std::vector<fmi2FMUstate>>>SemanticAdaptation<T>::cloneInstanceStates(shared_ptr<std::map<fmi2Component,std::vector<fmi2FMUstate>>> instanceStates)
{
	shared_ptr<std::map<fmi2Component,std::vector<fmi2FMUstate>>> stateClone = make_shared<std::map<fmi2Component,std::vector<fmi2FMUstate>>>();;

	for (auto itr = this->instances->begin(), end = this->instances->end(); itr != end; ++itr)
	{
		auto comp = (*itr)->component;
		auto fmu = (*itr)->fmu;

		auto iStates = std::vector<fmi2FMUstate>();
		stateClone->insert(std::make_pair(comp, iStates));

		auto stateItr = this->instanceStates->find(comp);
		if (stateItr != this->instanceStates->end())
		{
			auto states = stateItr->second;

			//prepare for FMU state cloning
			fmi2FMUstate currentState=NULL;
			auto status = fmu->getFMUstate(comp, &currentState);
			if(status !=fmi2OK)
			{
				cerr << "cloneInstanceStates failed: for storing current state" << status << endl;
				this->lastErrorState = status;
				THROW_STATUS_EXCEPTION;
			}

			while (!states.empty())
			{
				for(auto const& s: states)
				{
					status = fmu->setFMUstate(comp, s);
					if(status !=fmi2OK)
					{
						cerr << "cloneInstanceStates failed: for setting the state to clone" << status << endl;
						this->lastErrorState = status;
						THROW_STATUS_EXCEPTION;
					}

					fmi2FMUstate clone=NULL;
					status = fmu->getFMUstate(comp, &clone);
					if(status !=fmi2OK)
					{
						cerr << "cloneInstanceStates failed: getting the new cloned state of the state to clone" << status << endl;
						this->lastErrorState = status;
						THROW_STATUS_EXCEPTION;
					}

					iStates.push_back(clone);
				}

			}

			status = fmu->setFMUstate(comp, currentState);
			if(status !=fmi2OK)
			{
				cerr << "cloneInstanceStates failed: to set back original current state" << status << endl;
				this->lastErrorState = status;
				THROW_STATUS_EXCEPTION;
			}
			status = fmu->freeFMUstate(comp, &currentState);
			if(status !=fmi2OK)
			{
				cerr << "cloneInstanceStates failed: to free temp current state used to hold current during cloning" << status << endl;
				this->lastErrorState = status;
				THROW_STATUS_EXCEPTION;
			}
		}
	}
	return stateClone;
}

template<class T>
fmi2Component SemanticAdaptation<T>::getComponent()
{
	return (fmi2Component) 1;
}

template<class T>
void SemanticAdaptation<T>::log(fmi2String instanceName, fmi2Status status, fmi2String category, fmi2String message)
{

	if (this->fmiFunctions != NULL && this->fmiFunctions->logger != NULL)
	{

		this->fmiFunctions->logger(getComponent(), fmiInstanceName->c_str(), status, category, message);
	} else
	{
		string name(instanceName);
		name.insert(0, string("adapted-"));
		cout << "-Adapted-" << Fmu::fmi2StatusToString(status)->c_str() << " " << instanceName << " - " << category
				<< " " << message << endl;

	}

}

template<class T>
double SemanticAdaptation<T>::getNextTimeStep(shared_ptr<FmuComponent> fmuComp)
{
	double time = 0;

	fmi2FMUstate state = NULL;
	auto status = fmuComp->fmu->getFMUstate(fmuComp->component, &state);
	THROW_STATUS_EXCEPTION;

	status = fmuComp->fmu->doStep(fmuComp->component, lastSuccessfulTime, std::numeric_limits<fmi2Real>::max(), false);
	THROW_STATUS_EXCEPTION;

	if (status == fmi2Discard)
	{
		status = fmuComp->fmu->getRealStatus(fmuComp->component, fmi2LastSuccessfulTime, &time);
		THROW_STATUS_EXCEPTION;
	}
	status = fmuComp->fmu->setFMUstate(fmuComp->component, state);
	THROW_STATUS_EXCEPTION;
	status = fmuComp->fmu->freeFMUstate(fmuComp->component, &state);
	THROW_STATUS_EXCEPTION;

	return time;
}

}
/* namespace fmi2 */

#endif /* SRC_SEMANTICADAPTATION_H_ */
