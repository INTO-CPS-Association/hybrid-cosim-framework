#include "Fmu.h"
#include <iostream>
#include <fstream>

using namespace std;
using namespace fmi2;

fmi2Status setReal(FmuComponent comp, fmi2ValueReference id, fmi2Real val) {
	const fmi2ValueReference vr[] { id };
	size_t nvr = 1;
	fmi2Real value[] { val };

	return comp.fmu->setReal(comp.component, vr, nvr, value);

}

fmi2Real getReal(FmuComponent comp, fmi2ValueReference id) {
	const fmi2ValueReference vr[] { id };
	size_t nvr = 1;
	fmi2Real value[1];
	//fmi2SetReal(comp, vr, nvr, value);
	 comp.fmu->getReal(comp.component, vr, nvr, value);

	return value[0];

}

int main(int argc, char *argv[]) {
	//cout.precision(17);
	cout << "hello" << endl;
	auto fmu =
			make_shared<fmi2::Fmu>(
					string(
							"C:\\Users\\kel\\Downloads\\hybridCosimulation-framework.git\\window-sa\\Window.fmu"));
	cout << fmu->initialize() << endl;
	cout << fmu->getVersion() << endl;

	auto callback = make_shared<fmi2::Callback>();
	auto instance = fmu->instantiate("k", fmi2CoSimulation,
			"{29e3eae4-7ed5-4ccc-a0e7-7d8198e20bc0}", true, true, callback);

	cout << "instance: " << instance->component << endl;

	if (instance->component == NULL) {
		cout << "init failed" << endl;
		return -1;
	}

	if (instance->fmu->setupExperiment(instance->component, false, 0.0, 0, true,
			100) != fmi2OK) {
		printf("Error fmi2SetupExperiment");
		return 1;
	}

	if (instance->fmu->enterInitializationMode(instance->component) != fmi2OK) {
		printf("Error fmi2SetupExperiment");
		return 1;
	}

	if (instance->fmu->exitInitializationMode(instance->component) != fmi2OK) {
		printf("Error fmi2SetupExperiment");
		return 1;
	}

#define ID_Window_SA_IN_reaction_force 3
#define ID_Window_SA_IN_displacement 1
#define ID_Window_SA_IN_speed 4

#define ID_Window_SA_OUT_disp 0
#define ID_Window_SA_OUT_tau 5

	std::fstream fs;
	fs.open("result.csv",
			std::fstream::in | std::fstream::out | std::fstream::trunc);

	//cout << "------------ Header ---------" << endl;
	fs << "\"time\",\"ID_Window_SA_OUT_disp\",\"ID_Window_SA_OUT_tau\"" << endl;
	double time = 0.0;
	double stepSize = 0.01;

	for (double time = 0.0; time < 10;) {
		if (setReal(*instance, ID_Window_SA_IN_reaction_force, -1.0)
				!= fmi2OK) {
			printf("Error setting real");
			return 1;
		}
		if (setReal(*instance, ID_Window_SA_IN_displacement, time) != fmi2OK) {
			printf("Error setting real");
			return 1;
		}
		if (setReal(*instance, ID_Window_SA_IN_speed, 1.0) != fmi2OK) {
			printf("Error setting real");
			return 1;
		}

		if (instance->fmu->doStep(instance->component, time, stepSize, false)
				!= fmi2OK) {
			printf("Errorin do step");
			return 1;
		}
		time += stepSize;

		auto disp = getReal(*instance, ID_Window_SA_OUT_disp);
		auto tau = getReal(*instance, ID_Window_SA_OUT_tau);

		printf("disp %lf, tau %lf\n", disp, tau);
//		cout << "disp: " << getReal(*instance, ID_Window_SA_OUT_disp) << " tau: "
//				<< getReal(*instance, ID_Window_SA_OUT_tau) << endl;
		fs << time << "," << disp << "," << tau << endl;
	}

	return 0;
}
