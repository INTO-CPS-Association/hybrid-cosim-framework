/*
 * WindowSA.cpp
 *
 *  Created on: Mar 3, 2017
 *      Author: kel
 */

#include "WindowSA.h"

namespace adaptation
{

void Window_SA::setFmiValue(int id, int value)
{

}
void Window_SA::setFmiValue(int id, bool value)
{
}
void Window_SA::setFmiValue(int id, double value)
{
	switch (id)
	{
	case 0:
		this->reaction_force = value;
		this->isSetReaction_force = true;
		break;
	}

}

int Window_SA::getFmiValueInteger(int id)
{
	return 0;
}
bool Window_SA::getFmiValueBoolean(int id)
{
	return false;
}
double Window_SA::getFmiValueDouble(int id)
{
	switch (id)
	{
	case 0:
		return this->reaction_force;
	}
	return 0.0;
}

//in rules {
//	true -> {
//		/*
//		is_set checks whether the input value is given in the setValues call of the adapted FMU.
//		Notice that in the canonical version, all ports are prefixed.
//		*/
//		if (is_set(window_sa.reaction_force))
//			stored_windowsa_reaction_force := window_sa.reaction_force;
//	} --> {
//		window.reaction_force := stored_windowsa_reaction_force;
//	};
//    true -> {
//		if (is_set(window_sa.displacement))
//			stored_windowsa_displacement := window_sa.displacement;
//	} --> {
//		window.displacement := stored_windowsa_displacement;
//	};
//    true -> {
//		if (is_set(window_sa.speed))
//			stored_windowsa_speed := window_sa.speed;
//	} --> {
//		window.speed := stored_windowsa_speed;
//	};
//}

bool Window_SA::in_rule_condition_1()
{
	return true;
}

void Window_SA::in_rule_body1()
{
//	is_set checks whether the input value is given in the setValues call of the adapted FMU.
	//		Notice that in the canonical version, all ports are prefixed.
	//		*/
	if (isSetReaction_force)
		stored_windowsa_reaction_force = this->reaction_force;
}
void Window_SA::in_rule_flush1()
{
//window.reaction_force = stored_windowsa_reaction_force;
	setValue(window, 1/*reaction_force*/, stored_windowsa_reaction_force);
}

bool Window_SA::in_rule_condition_2()
{
	return true;
}

void Window_SA::in_rule_body2()
{
//	if (is_set(window_sa.displacement))
//				stored_windowsa_displacement := window_sa.displacement;
	if (this->isSetDisplacement)
		this->stored_windowsa_displacement = this->displacement;
}
void Window_SA::in_rule_flush2()
{
//	window.displacement := stored_windowsa_displacement;
	setValue(window, 2/*displacement*/, this->stored_windowsa_displacement);
}

bool Window_SA::in_rule_condition_3()
{
	return true;
}

void Window_SA::in_rule_body3()
{
//	if (is_set(window_sa.speed))
//				stored_windowsa_speed := window_sa.speed;
	if (this->isSetSpeed)
		this->stored_windowsa_speed = this->speed;
}
void Window_SA::in_rule_flush3()
{
//	window.speed := stored_windowsa_speed;
	setValue(window, 1/*reaction_force*/, this->speed);
}

shared_ptr<std::list<Rule<Window_SA>>>Window_SA::createInputRules()
{
	auto list = make_shared<std::list<Rule<Window_SA>>>();

	list->push_back(
			(Rule<Window_SA>)
			{	&Window_SA::in_rule_condition_1, &Window_SA::in_rule_body1, &Window_SA::in_rule_flush1});

	list->push_back(
			(Rule<Window_SA>)
			{	&Window_SA::in_rule_condition_2, &Window_SA::in_rule_body2, &Window_SA::in_rule_flush2});

	list->push_back(
			(Rule<Window_SA>)
			{	&Window_SA::in_rule_condition_3, &Window_SA::in_rule_body3, &Window_SA::in_rule_flush3});

	return list;
}

/*


 out rules {
 true => {
 stored_window_reaction_torque := window.reaction_torque;
 } -> {
 window_sa.tau := - stored_window_reaction_torque;
 }
 true => {
 stored_window_height := window.height;
 } -> {
 window_sa.disp := stored_window_height * 100;
 }
 }

 */

bool Window_SA::out_rule_condition_1()
{
	return true;
}

void Window_SA::out_rule_body1()
{
//	stored_window_reaction_torque := window.reaction_torque;
	this->stored_window_reaction_torque = getValueDouble(window, 3/*reaction_torque*/);
}
void Window_SA::out_rule_flush1()
{
//	window_sa.tau := - stored_window_reaction_torque;
	this->tau = this->stored_window_reaction_torque;
}

bool Window_SA::out_rule_condition_2()
{
	return true;
}

void Window_SA::out_rule_body2()
{
//	stored_window_height := window.height;
	this->stored_window_height = getValueDouble(window, 4/*height*/);
}
void Window_SA::out_rule_flush2()
{
//	window_sa.disp := stored_window_height * 100;
	this->disp = this->stored_window_height * 100;
}

shared_ptr<std::list<Rule<Window_SA>>>Window_SA::createOutputRules()
{
	auto list = make_shared<std::list<Rule<Window_SA>>>();

	list->push_back(
			(Rule<Window_SA>)
			{	&Window_SA::out_rule_condition_1, &Window_SA::out_rule_body1, &Window_SA::out_rule_flush1});

	list->push_back(
			(Rule<Window_SA>)
			{	&Window_SA::out_rule_condition_2, &Window_SA::out_rule_body2, &Window_SA::out_rule_flush2});

	return list;
}

Window_SA::Window_SA() :
		SemanticAdaptation(createInputRules(), createOutputRules())
{
	reaction_force = displacement = speed = disp = tau = stored_windowsa_reaction_force = stored_windowsa_displacement =
			stored_windowsa_speed = stored_window_reaction_torque = stored_window_height = 0;

	//FIXME
//	this->window =
}

Window_SA::~Window_SA()
{
}

void Window_SA::executeInternalControlFlow(double h, double dt)
{
	this->do_step(window, h, dt); // includes update_in rules and update_out (update-in rules do not update state)
}

} /* namespace fmi2 */
