

			
				#ifndef SRC_RATECONSTANTS_H
				#define SRC_RATECONSTANTS_H
		   
			

   
												 
	
												   
								  
													
							 
																																											 
			
				#include "SemanticAdaptation.h"
				#include "HyfMath.h"
				#include <memory>
				#include "Fmu.h"
				
				using namespace std;
				using namespace fmi2;
				
				namespace adaptation
				{
		 
																																																																	  
					 
								
   
																				   
																					
																					   
																					   
					
					#define CONSTANTSPSUVOLT 0
					#define CONSTANTSREF 1
					
					#define RATECONSTANTSPSUVOLT 0
					#define RATECONSTANTSREF 1
				
					class RateConstants : public SemanticAdaptation<RateConstants>, public enable_shared_from_this<RateConstants>
					{
						public:
							RateConstants(shared_ptr<std::string> fmiInstanceName, shared_ptr<string> resourceLocation, const fmi2CallbackFunctions* functions);
							void initialize(bool loggingOn);
							virtual ~RateConstants();
							
							void setFmiValue(fmi2ValueReference id, int value);
							void setFmiValue(fmi2ValueReference id, bool value);
							void setFmiValue(fmi2ValueReference id, double value);
							void setFmiValue(fmi2ValueReference id, string value);
				
												
   
												 
   
					 
										  
			 
						
							int getFmiValueInteger(fmi2ValueReference id);
							bool getFmiValueBoolean(fmi2ValueReference id);
							double getFmiValueReal(fmi2ValueReference id);
							string getFmiValueString(fmi2ValueReference id);
							
			  
			  
							
			  
			  
							
						protected:
							fmi2FMUstate getInternalFMUState();
							void setInternalFMUState(fmi2FMUstate state);
							void freeInternalFMUState(fmi2FMUstate state);			
							
						private:
							
							RateConstants* getRuleThis();
							
							/*in rules*/
							bool in_rule_condition1(double dt, double H, double h);
							void in_rule_body1(double dt, double H, double h);
							void in_rule_flush1(double dt, double H, double h);
							shared_ptr<list<Rule<RateConstants>>> createInputRules();
							
							/*out rules*/
							bool out_rule_condition1(double dt, double H, double h);
							void out_rule_body1(double dt, double H, double h);
							void out_rule_flush1(double dt, double H, double h);
							shared_ptr<list<Rule<RateConstants>>> createOutputRules();
							
							double executeInternalControlFlow(double t, double H);
							
							shared_ptr<FmuComponent> constants;
							
							struct InternalState {
								double psuvolt;
								double ref;
								
								int RATE;
								double INIT_PSUVOLT;
								double INIT_REF;
								double stored__ref;
								double stored__psuvolt;
							};
							
							InternalState internalState;
					};
				}
				
				#endif
