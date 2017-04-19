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

#define ID_Window_SA_IN_reaction_force 0
#define ID_Window_SA_IN_displacement 1
#define ID_Window_SA_IN_speed 2

#define ID_Window_SA_OUT_disp 3
#define ID_Window_SA_OUT_tau 4


class Window_SA: public SemanticAdaptation<Window_SA>, public std::enable_shared_from_this<Window_SA>
{
public:
	Window_SA(shared_ptr<std::string> resourceLocation,const fmi2CallbackFunctions *functions);
	void initialize();
	virtual ~Window_SA();

	void setFmiValue(fmi2ValueReference id, int value);
	void setFmiValue(fmi2ValueReference id, bool value);
	void setFmiValue(fmi2ValueReference id, double value);

	int getFmiValueInteger(fmi2ValueReference id);
	bool getFmiValueBoolean(fmi2ValueReference id);
	double getFmiValueReal(fmi2ValueReference id);

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

	double executeInternalControlFlow(double h, double dt);

	shared_ptr<FmuComponent> window;

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
