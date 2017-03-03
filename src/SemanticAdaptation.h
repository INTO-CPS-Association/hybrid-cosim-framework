/*
 * SemanticAdaptation.h
 *
 *  Created on: Mar 3, 2017
 *      Author: kel
 */

#ifndef SRC_SEMANTICADAPTATION_H_
#define SRC_SEMANTICADAPTATION_H_

#include <iostream>
#include <list>
#include "Rule.h"
using namespace std;
namespace fmi2
{

class SemanticAdaptation
{
public:
	SemanticAdaptation();
	virtual ~SemanticAdaptation();

private:
	shared_ptr<Rule*> inRules;
	shared_ptr<Rule*> outRules;

	std::list<Rule> enablesInRules;
};

} /* namespace fmi2 */

#endif /* SRC_SEMANTICADAPTATION_H_ */
