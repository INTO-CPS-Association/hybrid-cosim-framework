#include <iostream>
#include "fmi2TypesPlatform.h"
#include "fmi2Functions.h"
#include <string>

#include <memory>
#include <iostream>

#include <cstdio>
#include <stdarg.h>     /* va_list, va_start, va_arg, va_end */
#include <fstream>

using namespace std;

void fmuLoggerCache(void *componentEnvironment, fmi2String instanceName,
		fmi2Status status, fmi2String category, fmi2String message, ...) {
	va_list argp;

	// replace C format strings
	va_start(argp, message);
	size_t size = vsnprintf(nullptr, 0, message, argp) + 1; // Extra space for '\0'
	va_end(argp);

	va_start(argp, message);
	unique_ptr<char[]> buf(new char[size]);
	vsnprintf(buf.get(), size, message, argp);
	va_end(argp);

	string completeMessage = string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside

	cout << "FROM MAIN: Name: " << instanceName << " Status: " << status << " Category: "
			<< category << " Msg: " << completeMessage << endl;
	
}

fmi2Status setReal(fmi2Component comp, fmi2ValueReference id, fmi2Real val) {
	const fmi2ValueReference vr[] { id };
	size_t nvr = 1;
	fmi2Real value[] { val };
	return fmi2SetReal(comp, vr, nvr, value);
}

fmi2Real getReal(fmi2Component comp, fmi2ValueReference id) {
	const fmi2ValueReference vr[] { id };
	size_t nvr = 1;
	fmi2Real value[1];
	fmi2GetReal(comp, vr, nvr, value);

	return value[0];
}

int main(int argc, char *argv[]) {
	cout << "hello" << endl;

	fmi2CallbackFunctions callback = { &fmuLoggerCache, NULL, NULL, NULL, NULL };
	fmi2Component comp =
			fmi2Instantiate("this system", fmi2CoSimulation, "{1234}",
					"D:\\srcctrl\\into-cps-organization\\hybridCosimulation-framework\\semantic_adaptation_samples\\PlantMultiRate",
					&callback, fmi2True,
					fmi2False); // logging fmi2False

	if (comp == NULL) {
		cout << "init failed" << endl;
		return -1;
	}
	
	if (fmi2SetupExperiment(comp, false, 0.0, 0, true, 100) != fmi2OK) {
		printf("Error fmi2SetupExperiment");
		return 1;
	}

	if (fmi2EnterInitializationMode(comp) != fmi2OK) {
		printf("Error fmi2SetupExperiment");
		return 1;
	}
	
	if (fmi2ExitInitializationMode(comp) != fmi2OK) {
		printf("Error fmi2SetupExperiment");
		return 1;
	}

	std::fstream fs;

	double time = 0.0;
	double stepSize = 1e-2;

	
#define ID_T_IN 0
#define ID_P_IN 1
#define ID_V_IN 2
#define ID_X_IN 3

#define ID_W_OUT 4
#define ID_F_OUT 5
	
	
	auto wout = getReal(comp, ID_W_OUT);
	auto fout = getReal(comp, ID_F_OUT);

	cout << "time: " << 0.0 << " w: " << wout << " fout: " << fout
			<< endl;
	
	for (double time = 0.0; time < 2*stepSize; time += stepSize) {
		
		if (setReal(comp, ID_T_IN, 1.0) != fmi2OK) {
			printf("Error setReal");
			return 1;
		}
		if (setReal(comp, ID_P_IN, 1.0) != fmi2OK) {
			printf("Error setReal");
			return 1;
		}
		if (setReal(comp, ID_V_IN, 1.0) != fmi2OK) {
			printf("Error setReal");
			return 1;
		}
		if (setReal(comp, ID_X_IN, 1.0) != fmi2OK) {
			printf("Error setReal");
			return 1;
		}
		
		if (fmi2DoStep(comp, time, stepSize, false)!= fmi2OK) {
			printf("Errorin do step");
			return 1;
		}

		auto wout = getReal(comp, ID_W_OUT);
		auto fout = getReal(comp, ID_F_OUT);

		cout << "time: " << time + stepSize << " w: " << wout << " fout: " << fout
				<< endl;
	}

	return 0;
}
