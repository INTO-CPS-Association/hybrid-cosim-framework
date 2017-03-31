#include "gtest/gtest.h"

#include "Fmu.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
using namespace fmi2;

string FMU_PATH=string("../window-sa/Window.fmu");

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

TEST(Window, loadTest) {

	auto fmu = make_shared<fmi2::Fmu>(FMU_PATH);

	auto initialized = fmu->initialize();
	EXPECT_EQ(true, initialized);
	auto version = fmu->getVersion();
	EXPECT_STREQ("2.0", version);
}

TEST(Window, simulationTest) {

	auto fmu = make_shared<fmi2::Fmu>(FMU_PATH);

	auto initialized = fmu->initialize();
	EXPECT_EQ(true, initialized);
	auto version = fmu->getVersion();
	EXPECT_STREQ("2.0", version);

	auto callback = make_shared<fmi2::Callback>();
	auto instance = fmu->instantiate("k", fmi2CoSimulation,
			"{29e3eae4-7ed5-4ccc-a0e7-7d8198e20bc0}", true, true, callback);

	cout << "instance: " << instance->component << endl;

	EXPECT_TRUE(instance->component != NULL);

	EXPECT_EQ(fmi2OK,
			instance->fmu->setupExperiment(instance->component, false, 0.0, 0,
					true, 100));

	EXPECT_EQ(fmi2OK,instance->fmu->enterInitializationMode(instance->component) );

	EXPECT_EQ(fmi2OK,instance->fmu->exitInitializationMode(instance->component) );

#define ID_Window_SA_IN_reaction_force 3
#define ID_Window_SA_IN_displacement 1
#define ID_Window_SA_IN_speed 4

#define ID_Window_SA_OUT_disp 0
#define ID_Window_SA_OUT_tau 5

	double time = 0.0;
	double stepSize = 0.01;

	for (double time = 0.0; time < 0.1;) {
		EXPECT_EQ(fmi2OK,setReal(*instance, ID_Window_SA_IN_reaction_force, -1.0));

		EXPECT_EQ(fmi2OK,setReal(*instance, ID_Window_SA_IN_displacement, time));
		EXPECT_EQ(fmi2OK,setReal(*instance, ID_Window_SA_IN_speed, 1.0));

		EXPECT_EQ(fmi2OK,instance->fmu->doStep(instance->component, time, stepSize, false));
		time += stepSize;

		auto disp = getReal(*instance, ID_Window_SA_OUT_disp);
		auto tau = getReal(*instance, ID_Window_SA_OUT_tau);

		printf("disp %lf, tau %lf\n", disp, tau);
		//		cout << "disp: " << getReal(*instance, ID_Window_SA_OUT_disp) << " tau: "
		//				<< getReal(*instance, ID_Window_SA_OUT_tau) << endl;

	}
}
