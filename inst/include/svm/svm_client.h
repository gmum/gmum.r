#ifndef SVM_CLIENT_H
#define SVM_CLIENT_H

#include "svm_handler.h"
#include "svm_basic.h"
#include <vector>

class SVMClient {
private:
	std::vector<SVMHandler*> svm_handlers_;
	SVMConfiguration config_;
	void CreateFlow();

public:
	//constructors
	SVMClient(SVMConfiguration*);

	// data setters
	void set_x( arma::mat );
	void set_y( arma::vec );

	// params setter
	void set_library(std::string);
	void set_kernel(std::string);
	void set_preprocess(std::string);

	void set_cache_size(double);
	void set_degree(int);
	void set_gamma(double);
	void set_coef0(double);
	void set_c(double);
	void set_eps(double);
	void set_shrinking(int);
	void set_probability(int);
    void set_bias(double);
    // void set_alpha(double*);

	// data getters
	arma::mat x();
	arma::vec y();
	arma::vec prediction();

	// params getters
	std::string library();
	std::string kernel();
	std::string preprocess();

	double cache_size();
	int degree();
	double gamma();
	double coef0();
	double c();
	double eps();
	bool shrinking();
	bool probability();

	// runners
	void Run();
	void Predict( arma::mat );
	void Train();

	// model getters
    // double** getSV(); // double**, std::vector, arma:mat ?
    int number_sv();
    int number_class();
	arma::vec alpha();
	double bias();
	arma::vec w();
    arma::mat sv();
};

#endif
