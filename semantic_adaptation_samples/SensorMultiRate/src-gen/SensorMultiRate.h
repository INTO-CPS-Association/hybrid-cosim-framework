#ifndef SRC_SENSORMULTIRATE_H
#define SRC_SENSORMULTIRATE_H

#include "SemanticAdaptation.h"
#include "HyfMath.h"
#include <memory>
#include "Fmu.h"

using namespace std;
using namespace fmi2;

namespace adaptation
{
	#define SENSORF 134217728
	#define SENSORX 268435456
	#define SENSORV 268435457
	#define SENSORX_AFT 268435458

	#define SENSORMULTIRATEF 0
	#define SENSORMULTIRATEX_AFT 1
	#define SENSORMULTIRATEV 2
	#define SENSORMULTIRATEX 3

	class SensorMultiRate : public SemanticAdaptation<SensorMultiRate>, public enable_shared_from_this<SensorMultiRate>
	{
		public:
			SensorMultiRate(shared_ptr<std::string> fmiInstanceName, shared_ptr<string> resourceLocation, const fmi2CallbackFunctions* functions);
			void initialize(bool loggingOn);
			virtual ~SensorMultiRate();
			
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
			
			SensorMultiRate* getRuleThis();
			
			/*in rules*/
			bool in_rule_condition1(double dt, double H, double h);
			void in_rule_body1(double dt, double H, double h);
			void in_rule_flush1(double dt, double H, double h);
			shared_ptr<list<Rule<SensorMultiRate>>> createInputRules();
			
			/*out rules*/
			bool out_rule_condition1(double dt, double H, double h);
			void out_rule_body1(double dt, double H, double h);
			void out_rule_flush1(double dt, double H, double h);
			shared_ptr<list<Rule<SensorMultiRate>>> createOutputRules();
			
			double executeInternalControlFlow(double t, double H);
			
			shared_ptr<FmuComponent> sensor;
			
			struct InternalState {
				double f;
				bool isSetf;
				double x_aft;
				double v;
				double x;
				
				double RATE;
				double INIT_F;
				double INIT_X_AFT;
				double INIT_V;
				double INIT_X;
				double stored__f;
				double stored__v;
				double stored__x;
				double stored__x_aft;
			};
			
			InternalState internalState;
	};
}

#endif
