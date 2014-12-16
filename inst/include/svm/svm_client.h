#ifndef SVM_CLIENT_H
#define SVM_CLIENT_H

#include "svm_handler.h"
#include "libsvm_runner.h"
#include "svm_basic.h"
#include "norm.h"
#include <vector>

class SVMClient {
private:
	std::vector<SVMHandler*> SVMHandlers;
	SVMConfiguration config;
	void createFlow();

public:
	//constructors
	SVMClient(SVMConfiguration*);
	bool trained;

	// data setters
	void setX( arma::mat );
	void setY( arma::vec );

	// params setter
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

	// data getters
	arma::mat getX();
	arma::vec getY();
	arma::vec getPrediction();

	// params getters
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

	// runners
	void run();
    void predict( arma::mat );
    /// Process a request of prediction with a SVMHandlers implementations
	void requestPredict( arma::mat );
	void train();

	// model getters
 // double** getSV(); // double**, std::vector, arma:mat ?
	double* getAlpha();
	double getBias();
	double* getW();
};

#endif
