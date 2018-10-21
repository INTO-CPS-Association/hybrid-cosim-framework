#include "SensorMultiRate.h"
namespace adaptation 
{
	SensorMultiRate::SensorMultiRate(shared_ptr<std::string> fmiInstanceName,shared_ptr<string> resourceLocation, const fmi2CallbackFunctions* functions) : 
	SemanticAdaptation(fmiInstanceName, resourceLocation, createInputRules(), createOutputRules(), functions)
	{			
		this->internalState.RATE = 10.0;
		this->internalState.INIT_F = 0.0;
		this->internalState.INIT_X_AFT = 0.0;
		this->internalState.INIT_V = 0.0;
		this->internalState.INIT_X = 0.0;
		this->internalState.current_f = 0.0;
		this->internalState.stored__f = this->internalState.INIT_F;
		this->internalState.stored__x_aft = this->internalState.INIT_X_AFT;
		this->internalState.stored__v = this->internalState.INIT_V;
		this->internalState.stored__x = this->internalState.INIT_X;
		this->internalState.previous_f = 0.0;
	}
	
	void SensorMultiRate::initialize(bool loggingOn)
	{
		this->reactiveness = ReactiveOrDelayed::Reactive;
		this->machineType = MooreOrMealy::Moore;
		auto path1 = make_shared<string>(*resourceLocation);
		path1->append(string("./LoadNSensor_FixedEuler_1Em6.fmu"));
		auto sensorFmu = make_shared<fmi2::Fmu>(*path1);
		sensorFmu->initialize();
		this->sensor = sensorFmu->instantiate("sensor",fmi2CoSimulation, "2810456241", true, loggingOn, shared_from_this());
		
		if(this->sensor->component == NULL)
			this->lastErrorState = fmi2Fatal;
		this->instances->push_back(this->sensor);
	}
	
	SensorMultiRate::~SensorMultiRate()
	{
	}
	
	SensorMultiRate* SensorMultiRate::getRuleThis()
	{
		return this;
	}
	
	double SensorMultiRate::getFmiValueReal(fmi2ValueReference id)
	{
		#ifdef SA_DEBUG
			printf("Invoking double SensorMultiRate::getFmiValueReal(fmi2ValueReference id)");
			printf("\n");
		#endif	
		switch (id)
		{
			case SENSORMULTIRATEX_AFT:
			{
				return this->internalState.x_aft;
			}
			case SENSORMULTIRATEV:
			{
				return this->internalState.v;
			}
			case SENSORMULTIRATEX:
			{
				return this->internalState.x;
			}
			default:
			{
				return 0.0;
			}
		}
	}
	
	string SensorMultiRate::getFmiValueString(fmi2ValueReference id)
	{
		#ifdef SA_DEBUG
			printf("Invoking string SensorMultiRate::getFmiValueString(fmi2ValueReference id)");
			printf("\n");
		#endif	
		return "";
	}
	
	int SensorMultiRate::getFmiValueInteger(fmi2ValueReference id)
	{
		#ifdef SA_DEBUG
			printf("Invoking int SensorMultiRate::getFmiValueInteger(fmi2ValueReference id)");
			printf("\n");
		#endif	
		return 0;
	}
	
	bool SensorMultiRate::getFmiValueBoolean(fmi2ValueReference id)
	{
		#ifdef SA_DEBUG
			printf("Invoking bool SensorMultiRate::getFmiValueBoolean(fmi2ValueReference id)");
			printf("\n");
		#endif	
		return false;
	}
	
	void SensorMultiRate::setFmiValue(fmi2ValueReference id, double value)
	{
		#ifdef SA_DEBUG
			printf("SensorMultiRate::setFmiValue(%d,?)",id);
			printf("\n");
		#endif	
		switch (id)	
			{
				case SENSORMULTIRATEF:
				{
					this->internalState.f = value;
					this->internalState.isSetf = true;
					break;
				}
				default:
				{
				}
			}
	}
	
	void SensorMultiRate::setFmiValue(fmi2ValueReference id, string value)
	{
		#ifdef SA_DEBUG
			printf("SensorMultiRate::setFmiValue(%d,?)",id);
			printf("\n");
		#endif	
	}
	
	void SensorMultiRate::setFmiValue(fmi2ValueReference id, int value)
	{
		#ifdef SA_DEBUG
			printf("SensorMultiRate::setFmiValue(%d,?)",id);
			printf("\n");
		#endif	
	}
	
	void SensorMultiRate::setFmiValue(fmi2ValueReference id, bool value)
	{
		#ifdef SA_DEBUG
			printf("SensorMultiRate::setFmiValue(%d,?)",id);
			printf("\n");
		#endif	
	}
	
	bool SensorMultiRate::in_rule_condition1(double dt, double H, double h){
		#ifdef SA_DEBUG
			printf("Invoking bool SensorMultiRate::in_rule_condition1(double dt, double H, double h)");
			printf("\n");
		#endif	
		return true;
	}
	void SensorMultiRate::in_rule_body1(double dt, double H, double h){
		#ifdef SA_DEBUG
			printf("Invoking void SensorMultiRate::in_rule_body1(double dt, double H, double h)");
			printf("\n");
		#endif	
		this->internalState.stored__f = this->internalState.f;
		this->internalState.current_f = this->internalState.f;
	}
	void SensorMultiRate::in_rule_flush1(double dt, double H, double h){
		#ifdef SA_DEBUG
			printf("Invoking void SensorMultiRate::in_rule_flush1(double dt, double H, double h)");
			printf("\n");
		#endif	
		setValue(sensor,SENSORF,this->internalState.stored__f);
		;
		setValue(sensor,SENSORF,this->internalState.previous_f + (((this->internalState.current_f - this->internalState.previous_f) / H) * (dt + h)));
		;
	}
	shared_ptr<list<Rule<SensorMultiRate>>> SensorMultiRate::createInputRules()
	{
		auto list = make_shared<std::list<Rule<SensorMultiRate>>>();
		
		list->push_back(
			(Rule<SensorMultiRate>){
				&SensorMultiRate::in_rule_condition1,
				&SensorMultiRate::in_rule_body1,
				&SensorMultiRate::in_rule_flush1
			});
		
		
		return list;
		
	}
	
	
	double SensorMultiRate::executeInternalControlFlow(double t, double H) {
		#ifdef SA_DEBUG
			printf("Invoking double SensorMultiRate::executeInternalControlFlow(double t, double H)");
			printf("\n");
		#endif	
		double micro_step = 0.0;
		double inner_time = 0.0;
		inner_time = t;
		micro_step = H / this->internalState.RATE;
		for (int iter = 0; iter<this->internalState.RATE; iter++){
			this->do_step(sensor,inner_time,inner_time-t,micro_step);;
			inner_time = inner_time + micro_step;
		}
		this->internalState.previous_f = this->internalState.current_f;
		
		return H;
	}
	
	bool SensorMultiRate::out_rule_condition1(double dt, double H, double h){
		#ifdef SA_DEBUG
			printf("Invoking bool SensorMultiRate::out_rule_condition1(double dt, double H, double h)");
			printf("\n");
		#endif	
		return true;
	}
	void SensorMultiRate::out_rule_body1(double dt, double H, double h){
		#ifdef SA_DEBUG
			printf("Invoking void SensorMultiRate::out_rule_body1(double dt, double H, double h)");
			printf("\n");
		#endif	
		this->internalState.stored__x = getValueDouble(sensor,SENSORX);
		this->internalState.stored__x_aft = getValueDouble(sensor,SENSORX_AFT);
		this->internalState.stored__v = getValueDouble(sensor,SENSORV);
	}
	void SensorMultiRate::out_rule_flush1(double dt, double H, double h){
		#ifdef SA_DEBUG
			printf("Invoking void SensorMultiRate::out_rule_flush1(double dt, double H, double h)");
			printf("\n");
		#endif	
		this->internalState.x = this->internalState.stored__x;
		this->internalState.x_aft = this->internalState.stored__x_aft;
		this->internalState.v = this->internalState.stored__v;
	}
	shared_ptr<list<Rule<SensorMultiRate>>> SensorMultiRate::createOutputRules()
	{
		auto list = make_shared<std::list<Rule<SensorMultiRate>>>();
		
		list->push_back(
			(Rule<SensorMultiRate>){
				&SensorMultiRate::out_rule_condition1,
				&SensorMultiRate::out_rule_body1,
				&SensorMultiRate::out_rule_flush1
			});
		
		
		return list;
		
	}
	
	
	fmi2FMUstate SensorMultiRate::getInternalFMUState()
				
	{
		InternalState* s = new InternalState();
		*s = this->internalState;
		return s;
	}
	
	void SensorMultiRate::setInternalFMUState(fmi2FMUstate state)
	{
		this->internalState = *(InternalState*)state;
	}
	
	void SensorMultiRate::freeInternalFMUState(fmi2FMUstate state)
	{
		delete (InternalState*)state;
	}

}

