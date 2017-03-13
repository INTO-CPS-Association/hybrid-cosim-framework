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
using namespace std;
namespace adaptation
{

//FIXME: temp replace with full version
class Fmu
{
};

template<class T>
class SemanticAdaptation
{
public:
	SemanticAdaptation(shared_ptr<std::list<Rule<T>>>inRules, shared_ptr<std::list<Rule<T>>> outRules);
	virtual ~SemanticAdaptation();

	void executeControlFlow(double h, double dt);
	void executeInRules();
	void executeOutRules();

	enum MooreOrMealy
	{	Moore,Mealy};

	virtual	void setFmiValue(int id , int value)=0;
	virtual	void setFmiValue(int id , bool value)=0;
	virtual	void setFmiValue(int id , double value)=0;

	virtual	int getFmiValueInteger(int id )=0;
	virtual	bool getFmiValueBoolean(int id )=0;
	virtual	double getFmiValueDouble(int id )=0;

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
private:
	shared_ptr<std::list<Rule<T>>> inRules;
	shared_ptr<std::list<Rule<T>>> outRules;

	shared_ptr<std::list<Rule<T>>> enablesInRules;
	shared_ptr<std::list<Rule<T>>> enablesOutRules;

};

}
/* namespace fmi2 */

#endif /* SRC_SEMANTICADAPTATION_H_ */
