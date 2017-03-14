/*
 * main.cpp
 *
 *  Created on: Mar 14, 2017
 *      Author: kel
 */
#include <iostream>
#include "fmi2Functions.h"
#include <string>

#include <memory>
#include <iostream>
#include <string>
#include <cstdio>
using namespace std;

void fmuLoggerCache(void *componentEnvironment, fmi2String instanceName, fmi2Status status, fmi2String category,
		fmi2String message, ...)
{
	va_list argp;

	// replace C format strings
	va_start(argp, message);
	size_t size = vsnprintf( nullptr, 0, message, argp ) + 1; // Extra space for '\0'
	va_end(argp);

	va_start(argp, message);
	unique_ptr<char[]> buf(new char[size]);
	vsnprintf(buf.get(), size, message, argp);
	va_end(argp);

	string completeMessage = string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside

	cout << "Name: " << instanceName << " Status: " << status << " Category: " << category << " Msg: "
			<< completeMessage << endl;
}

int main()
{
	cout << "hello" << endl;

	fmi2CallbackFunctions callback =
	{ &fmuLoggerCache, NULL, NULL, NULL, NULL };
	fmi2Component comp = fmi2Instantiate("this system", fmi2CoSimulation, "", "", &callback, fmi2True,
	fmi2True);

	if (comp == NULL)
	{
		cout << "init faild" << endl;
		return -1;
	}

	const fmi2ValueReference vr[]
	{ 1 };
	size_t nvr = 1;
	fmi2Real value[]
	{ 1.0 };
	fmi2SetReal(comp, vr, nvr, value);

	return 0;
}
