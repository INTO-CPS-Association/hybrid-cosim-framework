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
namespace adaptation {

template<class T>
class SemanticAdaptation: public fmi2::Callback {
public:
	SemanticAdaptation(shared_ptr<std::string> resourceLocation,
			shared_ptr<std::list<Rule<T>>>inRules, shared_ptr<std::list<Rule<T>>> outRules,const fmi2CallbackFunctions *functions);
	virtual void initialize()=0;
	virtual ~SemanticAdaptation();

	virtual fmi2Status executeControlFlow(double h, double dt) final;
	fmi2Status executeInRules();
	fmi2Status executeOutRules();

	enum MooreOrMealy
	{	Moore,Mealy};

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
	virtual double getFmiValueDouble(fmi2ValueReference id )=0;

	fmi2Status fmi2SetupExperiment(fmi2Boolean toleranceDefined, fmi2Real tolerance, fmi2Real startTime,
	fmi2Boolean stopTimeDefined, fmi2Real stopTime);
	fmi2Status fmi2EnterInitializationMode();
	fmi2Status fmi2ExitInitializationMode();
	fmi2Status fmi2Terminate();
	/*FMI Calls END*/

protected:
	virtual void executeInternalControlFlow(double h, double dt)=0;
	void do_step(shared_ptr<FmuComponent>,double t,double H);

	const fmi2CallbackFunctions *fmiFunctions=NULL;
	MooreOrMealy machineType;

	fmi2Status setValue(shared_ptr<FmuComponent>,fmi2ValueReference id , int value);
	fmi2Status setValue(shared_ptr<FmuComponent>,fmi2ValueReference id , bool value);
	fmi2Status setValue(shared_ptr<FmuComponent>,fmi2ValueReference id , double value);

	int getValueInteger(shared_ptr<FmuComponent>,fmi2ValueReference id );
	bool getValueBoolean(shared_ptr<FmuComponent>,fmi2ValueReference id );
	double getValueDouble(shared_ptr<FmuComponent>,fmi2ValueReference id );

	virtual T* getRuleThis() =0;

	fmi2Status lastErrorState=fmi2OK;
	shared_ptr<std::string> lastErrorMessage;

	shared_ptr<std::string> resourceLocation;

	shared_ptr<std::list<shared_ptr<FmuComponent>>> instances;
private:
	shared_ptr<std::list<Rule<T>>> inRules;
	shared_ptr<std::list<Rule<T>>> outRules;

	shared_ptr<std::list<Rule<T>>> enablesInRules;
	shared_ptr<std::list<Rule<T>>> enablesOutRules;

	void flushAllEnabledInRules();

};

template<class T>
SemanticAdaptation<T>::SemanticAdaptation(
		shared_ptr<std::string> resourceLocation, shared_ptr<std::list<Rule<T>>>inRules, shared_ptr<std::list<Rule<T>>> outRules,const fmi2CallbackFunctions *functions)
{
	this->resourceLocation = resourceLocation;
	this->machineType = Mealy;
	this->inRules = inRules;
	cout << "added "<< this->inRules->size() <<endl;
	this->outRules = outRules;
	this->fmiFunctions = functions;

	this->enablesInRules = make_shared<std::list<Rule<T>>>();
	this->enablesOutRules = make_shared<std::list<Rule<T>>>();
	this->instances = make_shared<std::list<shared_ptr<FmuComponent>>>();

}

template<class T>
SemanticAdaptation<T>::~SemanticAdaptation() {
	// TODO Auto-generated destructor stub
}

template<class T>
fmi2Status SemanticAdaptation<T>::executeInRules() {
	//http://stackoverflow.com/questions/2898316/using-a-member-function-pointer-within-a-class
	for (auto itr = this->inRules->begin(), end = this->inRules->end();
			itr != end; ++itr) {
		Rule<T> rule = *itr;
		if (((*getRuleThis()).*rule.condition)()) {
			((*getRuleThis()).*rule.body)();
			if (this->machineType == Mealy) {
				((*getRuleThis()).*rule.flush)();
			}
			this->enablesInRules->push_back(*itr);
		}
	}
	return this->lastErrorState;
}

template<class T>
fmi2Status SemanticAdaptation<T>::executeOutRules() {
	if (this->enablesOutRules->size() > 0) {
		//not sure why
		return fmi2OK;
	}

	for (auto itr = this->outRules->begin(), end = this->outRules->end();
			itr != end; ++itr) {
		Rule<T> rule = *itr;
		if (((*getRuleThis()).*rule.condition)()) {
			((*getRuleThis()).*rule.body)();
			if (this->machineType == Mealy) {
				((*getRuleThis()).*rule.flush)();
			}
			this->enablesOutRules->push_back(*itr);
		}
	}

	return this->lastErrorState;
}

template<class T>
void SemanticAdaptation<T>::flushAllEnabledInRules() {
	for (auto itr = this->enablesInRules->begin(), end =
			this->enablesInRules->end(); itr != end; ++itr) {
		Rule<T> rule = *itr;
		((*getRuleThis()).*rule.flush)();

	}
}

template<class T>
fmi2Status SemanticAdaptation<T>::flushAllEnabledOutRules() {
	for (auto itr = this->enablesOutRules->begin(), end =
			this->enablesOutRules->end(); itr != end; ++itr) {
		Rule<T> rule = *itr;
		((*getRuleThis()).*rule.flush)();

	}

	return this->lastErrorState;
}

template<class T>
fmi2Status SemanticAdaptation<T>::executeControlFlow(double h, double dt) {
	this->enablesOutRules->clear();

	//flush all enabled in-rules
	flushAllEnabledInRules();

	executeInternalControlFlow(h, dt);

	//execute the body of all out-rules with a true condition and enable them
	executeOutRules();

	this->enablesInRules->clear();
	return this->lastErrorState;
}

template<class T>
fmi2Status SemanticAdaptation<T>::setValue(shared_ptr<FmuComponent> fmuComp,
		fmi2ValueReference id, int value) {
	const fmi2ValueReference vr[] { id };
	size_t nvr = 1;
	fmi2Integer v[] { value };

	fmi2Status status = fmuComp->fmu->setInteger(fmuComp->component, vr, nvr,
			v);
	if (status != fmi2OK) {
		cerr << "setInteger failed: " << id << " " << status << endl;
		this->lastErrorState = status;
	}
	return status;
}

template<class T>
fmi2Status SemanticAdaptation<T>::setValue(shared_ptr<FmuComponent> fmuComp,
		fmi2ValueReference id, bool value) {
	const fmi2ValueReference vr[] { id };
	size_t nvr = 1;
	fmi2Boolean v[] { value };

	fmi2Status status = fmuComp->fmu->setBoolean(fmuComp->component, vr, nvr,
			v);
	if (status != fmi2OK) {
		cerr << "setBoolean failed: " << id << " " << status << endl;
		this->lastErrorState = status;
	}
	return status;
}

template<class T>
fmi2Status SemanticAdaptation<T>::setValue(shared_ptr<FmuComponent> fmuComp,
		fmi2ValueReference id, double value) {
	const fmi2ValueReference vr[] { id };
	size_t nvr = 1;
	fmi2Real v[] { value };

	fmi2Status status = fmuComp->fmu->setReal(fmuComp->component, vr, nvr, v);
	if (status != fmi2OK) {
		cerr << "setReal failed: " << id << " " << status << endl;
		this->lastErrorState = status;
	}

	return status;
}

template<class T>
void SemanticAdaptation<T>::do_step(shared_ptr<FmuComponent> fmuComp, double t,
		double H) {
	fmi2Status status = fmuComp->fmu->doStep(fmuComp->component, t, H, false);
	if (status != fmi2OK) {
		cerr << "do_step failed: t: " << t << " h: " << H << " " << status
				<< endl;
		this->lastErrorState = status;
	}
}

template<class T>
int SemanticAdaptation<T>::getValueInteger(shared_ptr<FmuComponent> fmuComp,
		fmi2ValueReference id) {
	const fmi2ValueReference vr[] { id };
	size_t nvr = 1;
	fmi2Integer v[1];

	fmi2Status status = fmuComp->fmu->getInteger(fmuComp->component, vr, nvr,
			v);
	if (status != fmi2OK) {
		cerr << "getInteger failed: " << id << " " << status << endl;
		this->lastErrorState = status;
	}

	return v[0];
}
template<class T>
bool SemanticAdaptation<T>::getValueBoolean(shared_ptr<FmuComponent> fmuComp,
		fmi2ValueReference id) {
	const fmi2ValueReference vr[] { id };
	size_t nvr = 1;
	fmi2Boolean v[1];

	fmi2Status status = fmuComp->fmu->getBoolean(fmuComp->component, vr, nvr,
			v);
	if (status != fmi2OK) {
		cerr << "getBoolean failed: " << id << " " << status << endl;
		this->lastErrorState = status;
	}

	return v[0];
}
template<class T>
double SemanticAdaptation<T>::getValueDouble(shared_ptr<FmuComponent> fmuComp,
		fmi2ValueReference id) {
	const fmi2ValueReference vr[] { id };
	size_t nvr = 1;
	fmi2Real v[1];

	fmi2Status status = fmuComp->fmu->getReal(fmuComp->component, vr, nvr, v);
	if (status != fmi2OK) {
		cerr << "getReal failed: " << id << " " << status << endl;
		this->lastErrorState = status;
	}

	return v[0];
}

template<class T>
fmi2Status SemanticAdaptation<T>::getLastErrorState() {
	return this->lastErrorState;
}
template<class T>
shared_ptr<std::string> SemanticAdaptation<T>::getLastErrorMessage() {
	return this->lastErrorMessage;
}

template<class T>
fmi2Status SemanticAdaptation<T>::fmi2SetupExperiment(
		fmi2Boolean toleranceDefined, fmi2Real tolerance, fmi2Real startTime,
		fmi2Boolean stopTimeDefined, fmi2Real stopTime) {

	fmi2Status status = fmi2OK;
	for (auto itr = this->instances->begin(), end = this->instances->end();
			itr != end; ++itr) {
		status = (*itr)->fmu->setupExperiment((*itr)->component,
				toleranceDefined, tolerance, startTime, stopTimeDefined,
				stopTime);

		if (status != fmi2OK) {
			return status;
		}

	}
	return this->lastErrorState;
}

template<class T>
fmi2Status SemanticAdaptation<T>::fmi2EnterInitializationMode() {
	fmi2Status status = fmi2OK;
	for (auto itr = this->instances->begin(), end = this->instances->end();
			itr != end; ++itr) {
		status = (*itr)->fmu->enterInitializationMode((*itr)->component);

		if (status != fmi2OK) {
			return status;
		}

	}
	return this->lastErrorState;
}
template<class T>
fmi2Status SemanticAdaptation<T>::fmi2ExitInitializationMode() {
	fmi2Status status = fmi2OK;
	for (auto itr = this->instances->begin(), end = this->instances->end();
			itr != end; ++itr) {
		status = (*itr)->fmu->exitInitializationMode((*itr)->component);

		if (status != fmi2OK) {
			return status;
		}

	}
	return this->lastErrorState;
}

template<class T>
fmi2Status SemanticAdaptation<T>::fmi2Terminate() {
	fmi2Status status = fmi2OK;
	for (auto itr = this->instances->begin(), end = this->instances->end();
			itr != end; ++itr) {
		status = (*itr)->fmu->terminate((*itr)->component);

		if (status != fmi2OK) {
			return status;
		}

	}
	return this->lastErrorState;
}


template<class T>
fmi2Component SemanticAdaptation<T>::getComponent(){
	return (fmi2Component)1;
}

template<class T>
void SemanticAdaptation<T>::log(fmi2String instanceName, fmi2Status status,
		fmi2String category, fmi2String message) {

	// (fmi2ComponentEnvironment, fmi2String, fmi2Status, fmi2String, fmi2String, ...);
//	void *componentEnvironment, fmi2String instanceName, fmi2Status status, fmi2String category,
//			fmi2String message, .
	string name (instanceName);
	name.insert(0,string("adapted-"));
	if (this->fmiFunctions != NULL && this->fmiFunctions->logger != NULL) {

		this->fmiFunctions->logger(getComponent(),name.c_str(),status,category,message);
	} else {
		cout << "-Adapted-" << Fmu::fmi2StatusToString(status)->c_str() << " "
				<< instanceName << " - " << category << " " << message << endl;

	}

}

}
/* namespace fmi2 */

#endif /* SRC_SEMANTICADAPTATION_H_ */
