#include "RateConstants.h"

namespace adaptation 
{
	RateConstants::RateConstants(shared_ptr<std::string> fmiInstanceName,shared_ptr<string> resourceLocation, const fmi2CallbackFunctions* functions) : 
		SemanticAdaptation(fmiInstanceName, resourceLocation, createInputRules(),createOutputRules(), functions)
	{
		
		this->RATE = 10;
		this->INIT_PSUVOLT = 0.0;
		this->INIT_REF = 0.0;
		this->stored__psuvolt = this->INIT_PSUVOLT;this->stored__ref = this->INIT_REF;
	}
	
	void RateConstants::initialize(bool loggingOn)
	{				
		auto path = make_shared<string>(*resourceLocation);
		path->append(string("Constants.fmu"));
		auto constantsFmu = make_shared<fmi2::Fmu>(*path);
		constantsFmu->initialize();
		this->constants = constantsFmu->instantiate("constants",fmi2CoSimulation, "{bb398d58-c0ba-4011-8fc2-a09893a75244}", true, loggingOn, shared_from_this());
		
		if(this->constants->component == NULL)
			this->lastErrorState = fmi2Fatal;
		this->instances->push_back(this->constants);
	}
	
	RateConstants::~RateConstants()
	{
	}
	
	RateConstants* RateConstants::getRuleThis()
	{
		return this;
	}
	
	double RateConstants::getFmiValueReal(fmi2ValueReference id)
	{
		switch (id)
		{
			case RATECONSTANTSPSUVOLT:
			{
				return this->psuvolt;
			}
			case RATECONSTANTSREF:
			{
				return this->ref;
			}
			default:
			{
				return 0.0;
			}
		}
		
	}
	
	string RateConstants::getFmiValueString(fmi2ValueReference id)
	{
		return "";
	}
	
	int RateConstants::getFmiValueInteger(fmi2ValueReference id)
	{
		return 0;
	}
	
	bool RateConstants::getFmiValueBoolean(fmi2ValueReference id)
	{
		return false;
	}
	
    void RateConstants::setFmiValue(fmi2ValueReference id, double value)
    {
    }
    
    void RateConstants::setFmiValue(fmi2ValueReference id, string value)
    {
    }
    
    void RateConstants::setFmiValue(fmi2ValueReference id, int value)
    {
    }
    
    void RateConstants::setFmiValue(fmi2ValueReference id, bool value)
    {
    }
					
    bool RateConstants::out_rule_condition1(double dt, double h){
    	return true;
    }
    void RateConstants::out_rule_body1(double dt, double h){
    	this->stored__psuvolt = getValueDouble(constants,CONSTANTSPSUVOLT);
    	this->stored__ref = getValueDouble(constants,CONSTANTSREF);
    }
    void RateConstants::out_rule_flush1(double dt, double h){
    	this->psuvolt = this->stored__psuvolt;
    	this->ref = this->stored__ref;
    }
    shared_ptr<list<Rule<RateConstants>>> RateConstants::createOutputRules()
    {
    	auto list = make_shared<std::list<Rule<RateConstants>>>();
    	
    	list->push_back(
    		(Rule<RateConstants>){
    			&RateConstants::out_rule_condition1,
    			&RateConstants::out_rule_body1,
    			&RateConstants::out_rule_flush1
    		});
    	
    	
    	return list;
    	
    }
    
									
	fmi2FMUstate RateConstants::getInternalFMUState()
	{
	}
	
	void RateConstants::setInternalFMUState(fmi2FMUstate)
	{
	}
	
	void RateConstants::freeInternalFMUState(fmi2FMUstate)
	{
	}
	
	double RateConstants::executeInternalControlFlow(double H, double t)
	{
		double micro_step = 0.0;
		double inner_time = 0.0;
		inner_time = t;
		micro_step = H / this->RATE;
		for(int iter=0; iter<this->RATE; iter++){
			this->do_step(constants,micro_step,inner_time);
			inner_time = inner_time + micro_step;
		}
		
		return H;
	}
	
	bool RateConstants::in_rule_condition1(double dt, double h){
		return true;
	}
	void RateConstants::in_rule_body1(double dt, double h){
	}
	void RateConstants::in_rule_flush1(double dt, double h){
	}
	shared_ptr<list<Rule<RateConstants>>> RateConstants::createInputRules()
	{
		auto list = make_shared<std::list<Rule<RateConstants>>>();
		
		list->push_back(
			(Rule<RateConstants>){
				&RateConstants::in_rule_condition1,
				&RateConstants::in_rule_body1,
				&RateConstants::in_rule_flush1
			});
		
		
		return list;
		
	}
	

}

