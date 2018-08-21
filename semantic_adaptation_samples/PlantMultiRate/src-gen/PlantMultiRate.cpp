#include "PlantMultiRate.h"

namespace adaptation 
{
	PlantMultiRate::PlantMultiRate(shared_ptr<std::string> fmiInstanceName,shared_ptr<string> resourceLocation, const fmi2CallbackFunctions* functions) : 
	SemanticAdaptation(fmiInstanceName, resourceLocation, createInputRules(), createOutputRules(), functions)
	{			
		this->internalState.RATE = 1.0;
		this->internalState.INIT_TORQUE = 0.0;
		this->internalState.INIT_PSU = 0.0;
		this->internalState.INIT_V = 0.0;
		this->internalState.INIT_X = 0.0;
		this->internalState.INIT_W = 0.0;
		this->internalState.INIT_F = 0.0;
		this->internalState.stored__psu = this->internalState.INIT_PSU;
		this->internalState.stored__x = this->internalState.INIT_X;
		this->internalState.stored__v = this->internalState.INIT_V;
		this->internalState.stored__torque = this->internalState.INIT_TORQUE;
		this->internalState.stored__w = this->internalState.INIT_W;
		this->internalState.stored__f = this->internalState.INIT_F;
	}
	
	void PlantMultiRate::initialize(bool loggingOn)
	{
		this->reactiveness = ReactiveOrDelayed::Reactive;
		this->machineType = MooreOrMealy::Moore;
		auto path1 = make_shared<string>(*resourceLocation);
		path1->append(string("./EMAPlantNoLoad_FixedEuler_1Em6.fmu"));
		auto plantFmu = make_shared<fmi2::Fmu>(*path1);
		plantFmu->initialize();
		this->plant = plantFmu->instantiate("plant",fmi2CoSimulation, "698567049", true, loggingOn, shared_from_this());
		
		if(this->plant->component == NULL)
			this->lastErrorState = fmi2Fatal;
		this->instances->push_back(this->plant);
	}
	
	PlantMultiRate::~PlantMultiRate()
	{
	}
	
	PlantMultiRate* PlantMultiRate::getRuleThis()
	{
		return this;
	}
	
	double PlantMultiRate::getFmiValueReal(fmi2ValueReference id)
	{
		#ifdef SA_DEBUG
			printf("Invoking double PlantMultiRate::getFmiValueReal(fmi2ValueReference id)");
			printf("\n");
		#endif	
		switch (id)
		{
			case PLANTMULTIRATEW:
			{
				return this->internalState.w;
			}
			case PLANTMULTIRATEF:
			{
				return this->internalState.f;
			}
			default:
			{
				return 0.0;
			}
		}
	}
	
	string PlantMultiRate::getFmiValueString(fmi2ValueReference id)
	{
		#ifdef SA_DEBUG
			printf("Invoking string PlantMultiRate::getFmiValueString(fmi2ValueReference id)");
			printf("\n");
		#endif	
		return "";
	}
	
	int PlantMultiRate::getFmiValueInteger(fmi2ValueReference id)
	{
		#ifdef SA_DEBUG
			printf("Invoking int PlantMultiRate::getFmiValueInteger(fmi2ValueReference id)");
			printf("\n");
		#endif	
		return 0;
	}
	
	bool PlantMultiRate::getFmiValueBoolean(fmi2ValueReference id)
	{
		#ifdef SA_DEBUG
			printf("Invoking bool PlantMultiRate::getFmiValueBoolean(fmi2ValueReference id)");
			printf("\n");
		#endif	
		return false;
	}
	
	void PlantMultiRate::setFmiValue(fmi2ValueReference id, double value)
	{
		#ifdef SA_DEBUG
			printf("PlantMultiRate::setFmiValue(%d,?)",id);
			printf("\n");
		#endif	
		switch (id)	
			{
				case PLANTMULTIRATETORQUE:
				{
					this->internalState.torque = value;
					this->internalState.isSettorque = true;
					break;
				}
				case PLANTMULTIRATEPSU:
				{
					this->internalState.psu = value;
					this->internalState.isSetpsu = true;
					break;
				}
				case PLANTMULTIRATEV:
				{
					this->internalState.v = value;
					this->internalState.isSetv = true;
					break;
				}
				case PLANTMULTIRATEX:
				{
					this->internalState.x = value;
					this->internalState.isSetx = true;
					break;
				}
				default:
				{
				}
			}
	}
	
	void PlantMultiRate::setFmiValue(fmi2ValueReference id, string value)
	{
		#ifdef SA_DEBUG
			printf("PlantMultiRate::setFmiValue(%d,?)",id);
			printf("\n");
		#endif	
	}
	
	void PlantMultiRate::setFmiValue(fmi2ValueReference id, int value)
	{
		#ifdef SA_DEBUG
			printf("PlantMultiRate::setFmiValue(%d,?)",id);
			printf("\n");
		#endif	
	}
	
	void PlantMultiRate::setFmiValue(fmi2ValueReference id, bool value)
	{
		#ifdef SA_DEBUG
			printf("PlantMultiRate::setFmiValue(%d,?)",id);
			printf("\n");
		#endif	
	}
	
	bool PlantMultiRate::in_rule_condition1(double dt, double H, double h){
		#ifdef SA_DEBUG
			printf("Invoking bool PlantMultiRate::in_rule_condition1(double dt, double H, double h)");
			printf("\n");
		#endif	
		return true;
	}
	void PlantMultiRate::in_rule_body1(double dt, double H, double h){
		#ifdef SA_DEBUG
			printf("Invoking void PlantMultiRate::in_rule_body1(double dt, double H, double h)");
			printf("\n");
		#endif	
		this->internalState.stored__torque = this->internalState.torque;
		this->internalState.stored__v = this->internalState.v;
		this->internalState.stored__x = this->internalState.x;
		this->internalState.stored__psu = this->internalState.psu;
	}
	void PlantMultiRate::in_rule_flush1(double dt, double H, double h){
		#ifdef SA_DEBUG
			printf("Invoking void PlantMultiRate::in_rule_flush1(double dt, double H, double h)");
			printf("\n");
		#endif	
		setValue(plant,PLANTV,this->internalState.stored__v);
		;
		setValue(plant,PLANTPSU,this->internalState.stored__psu);
		;
		setValue(plant,PLANTX,this->internalState.stored__x);
		;
		setValue(plant,PLANTTORQUE,this->internalState.stored__torque);
		;
	}
	shared_ptr<list<Rule<PlantMultiRate>>> PlantMultiRate::createInputRules()
	{
		auto list = make_shared<std::list<Rule<PlantMultiRate>>>();
		
		list->push_back(
			(Rule<PlantMultiRate>){
				&PlantMultiRate::in_rule_condition1,
				&PlantMultiRate::in_rule_body1,
				&PlantMultiRate::in_rule_flush1
			});
		
		
		return list;
		
	}
	
	
	double PlantMultiRate::executeInternalControlFlow(double t, double H) {
		#ifdef SA_DEBUG
			printf("Invoking double PlantMultiRate::executeInternalControlFlow(double t, double H)");
			printf("\n");
		#endif	
		double micro_step = 0.0;
		double inner_time = 0.0;
		inner_time = t;
		micro_step = H / this->internalState.RATE;
		for (int iter = 0; iter<this->internalState.RATE; iter++){
			this->do_step(plant,inner_time,inner_time-t,micro_step);;
			inner_time = inner_time + micro_step;
		}
		
		return H;
	}
	
	bool PlantMultiRate::out_rule_condition1(double dt, double H, double h){
		#ifdef SA_DEBUG
			printf("Invoking bool PlantMultiRate::out_rule_condition1(double dt, double H, double h)");
			printf("\n");
		#endif	
		return true;
	}
	void PlantMultiRate::out_rule_body1(double dt, double H, double h){
		#ifdef SA_DEBUG
			printf("Invoking void PlantMultiRate::out_rule_body1(double dt, double H, double h)");
			printf("\n");
		#endif	
		this->internalState.stored__w = getValueDouble(plant,PLANTW);
		this->internalState.stored__f = getValueDouble(plant,PLANTF);
	}
	void PlantMultiRate::out_rule_flush1(double dt, double H, double h){
		#ifdef SA_DEBUG
			printf("Invoking void PlantMultiRate::out_rule_flush1(double dt, double H, double h)");
			printf("\n");
		#endif	
		this->internalState.w = this->internalState.stored__w;
		this->internalState.f = this->internalState.stored__f;
	}
	shared_ptr<list<Rule<PlantMultiRate>>> PlantMultiRate::createOutputRules()
	{
		auto list = make_shared<std::list<Rule<PlantMultiRate>>>();
		
		list->push_back(
			(Rule<PlantMultiRate>){
				&PlantMultiRate::out_rule_condition1,
				&PlantMultiRate::out_rule_body1,
				&PlantMultiRate::out_rule_flush1
			});
		
		
		return list;
		
	}
	
	
	fmi2FMUstate PlantMultiRate::getInternalFMUState()
				
	{
		InternalState* s = new InternalState();
		*s = this->internalState;
		return s;
	}
	
	void PlantMultiRate::setInternalFMUState(fmi2FMUstate state)
	{
		this->internalState = *(InternalState*)state;
	}
	
	void PlantMultiRate::freeInternalFMUState(fmi2FMUstate state)
	{
		delete (InternalState*)state;
	}

}

