/*
 * InRule.h
 *
 *  Created on: Mar 3, 2017
 *      Author: kel
 */

#ifndef SRC_RULE_H_
#define SRC_RULE_H_



template<class T>
struct Rule
{
	bool (T::*condition)();
	void (T::*body)();
	void (T::*flush)();
} ;



#endif /* SRC_RULE_H_ */
