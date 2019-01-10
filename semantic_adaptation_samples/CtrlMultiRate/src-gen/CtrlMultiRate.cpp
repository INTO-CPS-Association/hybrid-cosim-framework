#include "CtrlMultiRate.h"
namespace adaptation 
{
	CtrlMultiRate::CtrlMultiRate(shared_ptr<std::string> fmiInstanceName,shared_ptr<string> resourceLocation, const fmi2CallbackFunctions* functions) : 
	SemanticAdaptation(fmiInstanceName, resourceLocation, createInputRules(), createOutputRules(), functions)
	{			
		this->internalState.RATE = 10.0;
		this->internalState.INIT_V = 0.0;
		this->internalState.INIT_X = 0.0;
		this->internalState.INIT_REF = 0.0;
		this->internalState.INIT_O = 0.0;
		this->internalState.current_v = 0.0;
		this->internalState.current_x = 0.0;
		this->internalState.stored__v = this->internalState.INIT_V;
		this->internalState.stored__x = this->internalState.INIT_X;
		this->internalState.stored__ref = this->internalState.INIT_REF;
		this->internalState.stored__o = this->internalState.INIT_O;
		this->internalState.previous_v = 0.0;
		this->internalState.previous_x = 0.0;
	}
	
	void CtrlMultiRate::initialize(bool loggingOn)
	{
		this->reactiveness = ReactiveOrDelayed::Reactive;
		this->machineType = MooreOrMealy::Moore;
		auto path1 = make_shared<string>(*resourceLocation);
		path1->append(string("./DLoopController_FixedEuler_1Em6.fmu"));
		auto ctrlFmu = make_shared<fmi2::Fmu>(*path1);
		ctrlFmu->initialize();
		this->ctrl = ctrlFmu->instantiate("ctrl",fmi2CoSimulation, "2761528305", true, loggingOn, shared_from_this());
		
		if(this->ctrl->component == NULL)
			this->lastErrorState = fmi2Fatal;
		this->instances->push_back(this->ctrl);
	}
	
	CtrlMultiRate::~CtrlMultiRate()
	{
	}
	
	CtrlMultiRate* CtrlMultiRate::getRuleThis()
	{
		return this;
	}
	
	double CtrlMultiRate::getFmiValueReal(fmi2ValueReference id)
	{
		#ifdef SA_DEBUG
			printf("Invoking double CtrlMultiRate::getFmiValueReal(fmi2ValueReference id)");
			printf("\n");
		#endif	
		switch (id)
		{
			case CTRLMULTIRATEO:
			{
				return this->internalState.o;
			}
			default:
			{
				return 0.0;
			}
		}
	}
	
	string CtrlMultiRate::getFmiValueString(fmi2ValueReference id)
	{
		#ifdef SA_DEBUG
			printf("Invoking string CtrlMultiRate::getFmiValueString(fmi2ValueReference id)");
			printf("\n");
		#endif	
		return "";
	}
	
	int CtrlMultiRate::getFmiValueInteger(fmi2ValueReference id)
	{
		#ifdef SA_DEBUG
			printf("Invoking int CtrlMultiRate::getFmiValueInteger(fmi2ValueReference id)");
			printf("\n");
		#endif	
		return 0;
	}
	
	bool CtrlMultiRate::getFmiValueBoolean(fmi2ValueReference id)
	{
		#ifdef SA_DEBUG
			printf("Invoking bool CtrlMultiRate::getFmiValueBoolean(fmi2ValueReference id)");
			printf("\n");
		#endif	
		return false;
	}
	
	void CtrlMultiRate::setFmiValue(fmi2ValueReference id, double value)
	{
		#ifdef SA_DEBUG
			printf("CtrlMultiRate::setFmiValue(%d,?)",id);
			printf("\n");
		#endif	
		switch (id)	
			{
				case CTRLMULTIRATEV:
				{
					this->internalState.v = value;
					this->internalState.isSetv = true;
					break;
				}
				case CTRLMULTIRATEX:
				{
					this->internalState.x = value;
					this->internalState.isSetx = true;
					break;
				}
				case CTRLMULTIRATEREF:
				{
					this->internalState.ref = value;
					this->internalState.isSetref = true;
					break;
				}
				default:
				{
				}
			}
	}
	
	void CtrlMultiRate::setFmiValue(fmi2ValueReference id, string value)
	{
		#ifdef SA_DEBUG
			printf("CtrlMultiRate::setFmiValue(%d,?)",id);
			printf("\n");
		#endif	
	}
	
	void CtrlMultiRate::setFmiValue(fmi2ValueReference id, int value)
	{
		#ifdef SA_DEBUG
			printf("CtrlMultiRate::setFmiValue(%d,?)",id);
			printf("\n");
		#endif	
	}
	
	void CtrlMultiRate::setFmiValue(fmi2ValueReference id, bool value)
	{
		#ifdef SA_DEBUG
			printf("CtrlMultiRate::setFmiValue(%d,?)",id);
			printf("\n");
		#endif	
	}
	
	bool CtrlMultiRate::in_rule_condition1(double dt, double H, double h){
		#ifdef SA_DEBUG
			printf("Invoking bool CtrlMultiRate::in_rule_condition1(double dt, double H, double h)");
			printf("\n");
		#endif	
		return true;
	}
	void CtrlMultiRate::in_rule_body1(double dt, double H, double h){
		#ifdef SA_DEBUG
			printf("Invoking void CtrlMultiRate::in_rule_body1(double dt, double H, double h)");
			printf("\n");
		#endif	
		this->internalState.stored__ref = this->internalState.ref;
		this->internalState.stored__x = this->internalState.x;
		this->internalState.stored__v = this->internalState.v;
		this->internalState.current_x = this->internalState.x;
		this->internalState.current_v = this->internalState.v;
	}
	void CtrlMultiRate::in_rule_flush1(double dt, double H, double h){
		#ifdef SA_DEBUG
			printf("Invoking void CtrlMultiRate::in_rule_flush1(double dt, double H, double h)");
			printf("\n");
		#endif	
		setValue(ctrl,CTRLREF,this->internalState.stored__ref);
		;
		setValue(ctrl,CTRLV,this->internalState.stored__v);
		;
		setValue(ctrl,CTRLX,this->internalState.stored__x);
		;
		setValue(ctrl,CTRLX,this->internalState.previous_x + (((this->internalState.current_x - this->internalState.previous_x) / H) * ((H + dt) + h)));
		;
		setValue(ctrl,CTRLV,this->internalState.previous_v + (((this->internalState.current_v - this->internalState.previous_v) / H) * ((H + dt) + h)));
		;
	}
	shared_ptr<list<Rule<CtrlMultiRate>>> CtrlMultiRate::createInputRules()
	{
		auto list = make_shared<std::list<Rule<CtrlMultiRate>>>();
		
		list->push_back(
			(Rule<CtrlMultiRate>){
				&CtrlMultiRate::in_rule_condition1,
				&CtrlMultiRate::in_rule_body1,
				&CtrlMultiRate::in_rule_flush1
			});
		
		
		return list;
		
	}
	
	
	double CtrlMultiRate::executeInternalControlFlow(double t, double H) {
		#ifdef SA_DEBUG
			printf("Invoking double CtrlMultiRate::executeInternalControlFlow(double t, double H)");
			printf("\n");
		#endif	
		double micro_step = 0.0;
		double inner_time = 0.0;
		inner_time = t;
		micro_step = H / this->internalState.RATE;
		for (int iter = 0; iter<this->internalState.RATE; iter++){
			this->do_step(ctrl,inner_time,inner_time-t,micro_step);;
			inner_time = inner_time + micro_step;
		}
		this->internalState.previous_v = this->internalState.current_v;
		this->internalState.previous_x = this->internalState.current_x;
		
		return H;
	}
	
	bool CtrlMultiRate::out_rule_condition1(double dt, double H, double h){
		#ifdef SA_DEBUG
			printf("Invoking bool CtrlMultiRate::out_rule_condition1(double dt, double H, double h)");
			printf("\n");
		#endif	
		return true;
	}
	void CtrlMultiRate::out_rule_body1(double dt, double H, double h){
		#ifdef SA_DEBUG
			printf("Invoking void CtrlMultiRate::out_rule_body1(double dt, double H, double h)");
			printf("\n");
		#endif	
		this->internalState.stored__o = getValueDouble(ctrl,CTRLO);
	}
	void CtrlMultiRate::out_rule_flush1(double dt, double H, double h){
		#ifdef SA_DEBUG
			printf("Invoking void CtrlMultiRate::out_rule_flush1(double dt, double H, double h)");
			printf("\n");
		#endif	
		this->internalState.o = this->internalState.stored__o;
	}
	shared_ptr<list<Rule<CtrlMultiRate>>> CtrlMultiRate::createOutputRules()
	{
		auto list = make_shared<std::list<Rule<CtrlMultiRate>>>();
		
		list->push_back(
			(Rule<CtrlMultiRate>){
				&CtrlMultiRate::out_rule_condition1,
				&CtrlMultiRate::out_rule_body1,
				&CtrlMultiRate::out_rule_flush1
			});
		
		
		return list;
		
	}
	
	
	fmi2FMUstate CtrlMultiRate::getInternalFMUState()
				
	{
		InternalState* s = new InternalState();
		*s = this->internalState;
		return s;
	}
	
	void CtrlMultiRate::setInternalFMUState(fmi2FMUstate state)
	{
		this->internalState = *(InternalState*)state;
	}
	
	void CtrlMultiRate::freeInternalFMUState(fmi2FMUstate state)
	{
		delete (InternalState*)state;
	}

}

