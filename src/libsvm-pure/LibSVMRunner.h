/*
 * LibSVMRunner.h
 *
 *  Created on: Apr 7, 2014
 *      Author: sacherus
 */
#ifndef LIBSVMRUNNER_H_
#define LIBSVMRUNNER_H_

#include <string>
#include <sstream>
#include <cstdlib>
#include <time.h>
#include "svm.h"

class LibSVMRunner {
public:
	LibSVMRunner();
	virtual ~LibSVMRunner();
	bool canHandle();
	void handleRequest(const char*);
	void handleRequest(svm_parameter&, svm_problem&);
	/*
	virtual string printTicket();
protected:
	int ticketSize, maxNum;
	int* generateNumbers();
public:
	//Lottery(int, int);virtual ~Lottery();
*/

};

#endif /* LIBSVMRUNNER_H_ */
