/*
 * InRule.h
 *
 *  Created on: Mar 3, 2017
 *      Author: kel
 */

#ifndef SRC_RULE_H_
#define SRC_RULE_H_

#ifdef __cplusplus
extern "C"
{
#endif

struct Rule
{
	bool *(condition)();
	void *(body)();
	void *(flush)();
} ;

#ifdef __cplusplus
}
#endif

#endif /* SRC_RULE_H_ */
