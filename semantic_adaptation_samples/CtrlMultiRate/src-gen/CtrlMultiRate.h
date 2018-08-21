#ifndef SRC_CTRLMULTIRATE_H
#define SRC_CTRLMULTIRATE_H

#include "SemanticAdaptation.h"
#include "HyfMath.h"
#include <memory>
#include "Fmu.h"

using namespace std;
using namespace fmi2;

namespace adaptation
{
	#define CTRLV 134217728
	#define CTRLX 134217729
	#define CTRLREF 134217730
	#define CTRLO 268435456

	#define CTRLMULTIRATEV 0
	#define CTRLMULTIRATEX 1
	#define CTRLMULTIRATEREF 2
	#define CTRLMULTIRATEO 3

	class CtrlMultiRate : public SemanticAdaptation<CtrlMultiRate>, public enable_shared_from_this<CtrlMultiRate>
	{
		public:
			CtrlMultiRate(shared_ptr<std::string> fmiInstanceName, shared_ptr<string> resourceLocation, const fmi2CallbackFunctions* functions);
			void initialize(bool loggingOn);
			virtual ~CtrlMultiRate();
			
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
			
			CtrlMultiRate* getRuleThis();
			
			/*in rules*/
			bool in_rule_condition1(double dt, double H, double h);
			void in_rule_body1(double dt, double H, double h);
			void in_rule_flush1(double dt, double H, double h);
			shared_ptr<list<Rule<CtrlMultiRate>>> createInputRules();
			
			/*out rules*/
			bool out_rule_condition1(double dt, double H, double h);
			void out_rule_body1(double dt, double H, double h);
			void out_rule_flush1(double dt, double H, double h);
			shared_ptr<list<Rule<CtrlMultiRate>>> createOutputRules();
			
			double executeInternalControlFlow(double t, double H);
			
			shared_ptr<FmuComponent> ctrl;
			
			struct InternalState {
				double v;
				bool isSetv;
				double x;
				bool isSetx;
				double ref;
				bool isSetref;
				double o;
				
				double RATE;
				double INIT_V;
				double INIT_X;
				double INIT_REF;
				double INIT_O;
				double current_v;
				double current_x;
				double stored__x;
				double stored__ref;
				double stored__v;
				double stored__o;
				double previous_v;
				double previous_x;
			};
			
			InternalState internalState;
	};
}

#endif
