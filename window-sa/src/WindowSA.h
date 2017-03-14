/*
 * WindowSA.h
 *
 *  Created on: Mar 3, 2017
 *      Author: kel
 */

#ifndef SRC_WINDOWSA_H_
#define SRC_WINDOWSA_H_

#include <memory>
#include "SemanticAdaptation.h"
#include "Fmu.h"

using namespace std;
using namespace fmi2;

namespace adaptation
{

class Window_SA: public SemanticAdaptation<Window_SA>
{
public:
	Window_SA(shared_ptr<std::string> resourceLocation);
	virtual ~Window_SA();

	void setFmiValue(int id, int value);
	void setFmiValue(int id, bool value);
	void setFmiValue(int id, double value);

	int getFmiValueInteger(int id);
	bool getFmiValueBoolean(int id);
	double getFmiValueDouble(int id);

private:

Window_SA*	getRuleThis();

	shared_ptr<std::list<Rule<Window_SA>>> createInputRules();
	shared_ptr<std::list<Rule<Window_SA>>> createOutputRules();

	/*in rules*/
	bool in_rule_condition_1();
	void in_rule_body1();
	void in_rule_flush1();

	bool in_rule_condition_2();
	void in_rule_body2();
	void in_rule_flush2();

	bool in_rule_condition_3();
	void in_rule_body3();
	void in_rule_flush3();

	/*out rules*/
	bool out_rule_condition_1();
	void out_rule_body1();
	void out_rule_flush1();

	bool out_rule_condition_2();
	void out_rule_body2();
	void out_rule_flush2();

	void executeInternalControlFlow(double h, double dt);

	shared_ptr<Fmu> window;

	//inputs
	double reaction_force;
	double displacement;
	double speed;

	bool isSetReaction_force;
	bool isSetDisplacement;
	bool isSetSpeed;

	//outputs
	double disp;
	double tau;

	//local
	double stored_windowsa_reaction_force,
	stored_windowsa_displacement,
	stored_windowsa_speed;

	double stored_window_reaction_torque,
	stored_window_height;
};

}
/* namespace fmi2 */

#endif /* SRC_WINDOWSA_H_ */
