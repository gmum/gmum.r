#ifndef SVM_CLIENT_H
#define SVM_CLIENT_H

#include "svm_handler.h"
#include "svm_basic.h"
#include "norm.h"
#include <vector>

class SVMClient {
private:
	std::vector<SVMHandler*> SVMHandlers;
	SVMConfiguration config;
	void createFlow();
public:
	SVMClient(SVMConfiguration*);

	void setX( arma::mat );
	void setY( arma::vec );

	arma::mat getX();
	arma::vec getY();
	arma::vec getPrediction();

	std::string getLibrary();
	std::string getKernel();
	std::string getPreprocess();

	double getCacheSize();
	int getDegree();
	double getGamma();
	double getCoef0();
	double getC();
	double getEps();
	bool isShrinking();
	bool isProbability();

	void run();
	void predict( arma::mat );
};

#endif
