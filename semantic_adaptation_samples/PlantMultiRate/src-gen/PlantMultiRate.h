#ifndef SRC_PLANTMULTIRATE_H
#define SRC_PLANTMULTIRATE_H

#include "SemanticAdaptation.h"
#include "HyfMath.h"
#include <memory>
#include "Fmu.h"

using namespace std;
using namespace fmi2;

namespace adaptation
{
	#define PLANTTORQUE 134217728
	#define PLANTPSU 134217729
	#define PLANTV 134217730
	#define PLANTX 134217731
	#define PLANTW 268435456
	#define PLANTF 268435457

	#define PLANTMULTIRATETORQUE 0
	#define PLANTMULTIRATEPSU 1
	#define PLANTMULTIRATEV 2
	#define PLANTMULTIRATEX 3
	#define PLANTMULTIRATEW 4
	#define PLANTMULTIRATEF 5

	class PlantMultiRate : public SemanticAdaptation<PlantMultiRate>, public enable_shared_from_this<PlantMultiRate>
	{
		public:
			PlantMultiRate(shared_ptr<std::string> fmiInstanceName, shared_ptr<string> resourceLocation, const fmi2CallbackFunctions* functions);
			void initialize(bool loggingOn);
			virtual ~PlantMultiRate();
			
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
			
			PlantMultiRate* getRuleThis();
			
			/*in rules*/
			bool in_rule_condition1(double dt, double H, double h);
			void in_rule_body1(double dt, double H, double h);
			void in_rule_flush1(double dt, double H, double h);
			shared_ptr<list<Rule<PlantMultiRate>>> createInputRules();
			
			/*out rules*/
			bool out_rule_condition1(double dt, double H, double h);
			void out_rule_body1(double dt, double H, double h);
			void out_rule_flush1(double dt, double H, double h);
			shared_ptr<list<Rule<PlantMultiRate>>> createOutputRules();
			
			double executeInternalControlFlow(double t, double H);
			
			shared_ptr<FmuComponent> plant;
			
			struct InternalState {
				double torque;
				bool isSettorque;
				double psu;
				bool isSetpsu;
				double v;
				bool isSetv;
				double x;
				bool isSetx;
				double w;
				double f;
				
				int RATE;
				double INIT_TORQUE;
				double INIT_PSU;
				double INIT_V;
				double INIT_X;
				double INIT_W;
				double INIT_F;
				double current_torque;
				double current_psu;
				double current_v;
				double current_x;
				double stored__psu;
				double stored__x;
				double stored__v;
				double stored__torque;
				double stored__w;
				double stored__f;
				double previous_torque;
				double previous_psu;
				double previous_v;
				double previous_x;
			};
			
			InternalState internalState;
	};
}

#endif
