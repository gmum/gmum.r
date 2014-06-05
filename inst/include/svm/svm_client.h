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
	bool trained;

	void setX( arma::mat );
	void setY( arma::vec );

	void setLibrary(std::string);
	void setKernel(std::string);
	void setPreprocess(std::string);

	void setCacheSize(double);
	void setDegree(int);
	void setGamma(double);
	void setCoef0(double);
	void setC(double);
	void setEps(double);
	void setShrinking(int);
	void setProbability(int);

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
	void train();
};

#endif