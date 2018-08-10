			#include "RateConstants.h"
										   
   
							   
					
				 
				

					
					 
			
			namespace adaptation 
			{
				RateConstants::RateConstants(shared_ptr<std::string> fmiInstanceName,shared_ptr<string> resourceLocation, const fmi2CallbackFunctions* functions) : 
				SemanticAdaptation(fmiInstanceName, resourceLocation, createInputRules(), createOutputRules(), functions)
				{			
					this->internalState.RATE = 10;
					this->internalState.INIT_PSUVOLT = 0.0;
					this->internalState.INIT_REF = 0.0;
					this->internalState.stored__psuvolt = this->internalState.INIT_PSUVOLT;
					this->internalState.stored__ref = this->internalState.INIT_REF;
				}
				
				void RateConstants::initialize(bool loggingOn)
				{
					this->reactiveness = ReactiveOrDelayed::Reactive;
					this->machineType = MooreOrMealy::Moore;
					auto path1 = make_shared<string>(*resourceLocation);
					path1->append(string("./Constants.fmu"));
					auto constantsFmu = make_shared<fmi2::Fmu>(*path1);
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
												#ifdef SA_DEBUG
													printf("Invoking double RateConstants::getFmiValueReal(fmi2ValueReference id)");
													printf("\n");
												#endif	
												switch (id)
												{
													case RATECONSTANTSPSUVOLT:
													{
														return this->internalState.psuvolt;
													}
													case RATECONSTANTSREF:
													{
														return this->internalState.ref;
													}
													default:
													{
														return 0.0;
													}
								  
								
						 
											
												}
												
											}
				
				string RateConstants::getFmiValueString(fmi2ValueReference id)
				{
					#ifdef SA_DEBUG
						printf("Invoking string RateConstants::getFmiValueString(fmi2ValueReference id)");
						printf("\n");
					#endif	
					return "";
				}
				
				int RateConstants::getFmiValueInteger(fmi2ValueReference id)
				{
					#ifdef SA_DEBUG
						printf("Invoking int RateConstants::getFmiValueInteger(fmi2ValueReference id)");
						printf("\n");
					#endif	
					return 0;
				}
				
				bool RateConstants::getFmiValueBoolean(fmi2ValueReference id)
				{
					#ifdef SA_DEBUG
						printf("Invoking bool RateConstants::getFmiValueBoolean(fmi2ValueReference id)");
						printf("\n");
					#endif	
					return false;
				}
				
				void RateConstants::setFmiValue(fmi2ValueReference id, double value)
				{
					#ifdef SA_DEBUG
						printf("RateConstants::setFmiValue(%d,?)",id);
						printf("\n");
					#endif	
				}
				
				void RateConstants::setFmiValue(fmi2ValueReference id, string value)
				{
					#ifdef SA_DEBUG
						printf("RateConstants::setFmiValue(%d,?)",id);
						printf("\n");
					#endif	
				}
				
				void RateConstants::setFmiValue(fmi2ValueReference id, int value)
				{
					#ifdef SA_DEBUG
						printf("RateConstants::setFmiValue(%d,?)",id);
						printf("\n");
					#endif	
				}
				
				void RateConstants::setFmiValue(fmi2ValueReference id, bool value)
				{
					#ifdef SA_DEBUG
						printf("RateConstants::setFmiValue(%d,?)",id);
						printf("\n");
					#endif	
				}
				
				bool RateConstants::in_rule_condition1(double dt, double h){
					#ifdef SA_DEBUG
						printf("Invoking bool RateConstants::in_rule_condition1(double dt, double h)");
						printf("\n");
					#endif	
					return true;
				}
				void RateConstants::in_rule_body1(double dt, double h){
					#ifdef SA_DEBUG
						printf("Invoking void RateConstants::in_rule_body1(double dt, double h)");
						printf("\n");
					#endif	
				}
				void RateConstants::in_rule_flush1(double dt, double h){
					#ifdef SA_DEBUG
						printf("Invoking void RateConstants::in_rule_flush1(double dt, double h)");
						printf("\n");
					#endif	
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
				
				
				double RateConstants::executeInternalControlFlow(double t, double H) {
					#ifdef SA_DEBUG
						printf("Invoking double RateConstants::executeInternalControlFlow(double t, double H)");
						printf("\n");
					#endif	
					double micro_step = 0.0;
					double inner_time = 0.0;
					inner_time = t;
					micro_step = H / this->internalState.RATE;
					for (int iter = 0; iter<=this->internalState.RATE; iter++){
						this->do_step(constants,micro_step,inner_time);
						inner_time = inner_time + micro_step;
					}
					
					return H;
				}
				
				bool RateConstants::out_rule_condition1(double dt, double h){
					#ifdef SA_DEBUG
						printf("Invoking bool RateConstants::out_rule_condition1(double dt, double h)");
						printf("\n");
					#endif	
					return true;
				}
				void RateConstants::out_rule_body1(double dt, double h){
					#ifdef SA_DEBUG
						printf("Invoking void RateConstants::out_rule_body1(double dt, double h)");
						printf("\n");
					#endif	
					this->internalState.stored__psuvolt = getValueDouble(constants,CONSTANTSPSUVOLT);
					this->internalState.stored__ref = getValueDouble(constants,CONSTANTSREF);
				}
				void RateConstants::out_rule_flush1(double dt, double h){
					#ifdef SA_DEBUG
						printf("Invoking void RateConstants::out_rule_flush1(double dt, double h)");
						printf("\n");
					#endif	
					this->internalState.psuvolt = this->internalState.stored__psuvolt;
					this->internalState.ref = this->internalState.stored__ref;
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
					InternalState* s = new InternalState();
					*s = this->internalState;
					return s;
				}
						
					
						
					
					   
					
						   
					
						
					
						  
				
				void RateConstants::setInternalFMUState(fmi2FMUstate state)
				{
					this->internalState = *(InternalState*)state;
				}
				
				void RateConstants::freeInternalFMUState(fmi2FMUstate state)
				{
					delete (InternalState*)state;
				}
			
			}
			
