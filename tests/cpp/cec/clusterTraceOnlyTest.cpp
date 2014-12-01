#include "gtest/gtest.h"
#include "cluster.hpp"
#include <RcppArmadillo.h>
#include <boost/shared_ptr.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
using namespace gmum;

TEST(TraceOnly,AddPoint) {
	//arma_rng::set_seed(0);
	int n = 10000;
	int dim = 2;
	int beg = dim+1;
	double acceptableDifference = 1e-6;

	std::vector<unsigned int> fits;
	unsigned id = 1;
	arma::mat data(n,dim);
	data.randu();
	for (int i = 0; i < n; ++i) {
		fits.push_back(id);
	}

	arma::mat initMatrix(beg,dim);
	for(int k = 0; k < beg; ++k) {
		for (int j = 0; j < dim; ++j) {
			initMatrix(k,j) = data(k,j);
		}
	}

	ASSERT_TRUE(true);

	boost::shared_ptr<Cluster> m(new ClusterSpherical(id,fits,initMatrix));
	// Dodajemy element o indeksie i
	for (int i = beg; i < n-1; ++i) {

		arma::mat tmpMatrix(i+1,dim);
		for (int j = 0; j <=i; ++j) {
			tmpMatrix.row(j) = data.row(j);
		}
		arma::mat covariance= cov(tmpMatrix,1);
		arma::mat realM = mean(tmpMatrix);

		arma::rowvec point(data.row(i));
		m = m->addPoint(point);
		ClusterOnlyTrace * upref = dynamic_cast<ClusterOnlyTrace*>(m.get());
		ClusterStandard tmp(id,fits,tmpMatrix);
		arma::rowvec meanOnlineDifference = upref->getMean() - realM;
		float traceDiff = upref->getCovMatTrace() - arma::trace(covariance);
		float relativeError = std::abs(traceDiff/arma::trace(covariance));

		EXPECT_EQ(m->size(),tmp.size());
		EXPECT_LT(std::abs(traceDiff),acceptableDifference);
		std::cout << i << " : " << relativeError << std::endl;
		for (int j = 0; j < dim; ++j) {
			EXPECT_LT(std::abs(meanOnlineDifference(j)),acceptableDifference) << "at position" << j << " means differ by more than " << acceptableDifference;
		}
	}
}

TEST(TraceOnly,removePoint) {
	//arma_rng::set_seed(0);
	int n = 100;
	int dim = 2;
	int end = dim+1;
	double acceptableDifference = 1e-6;

	std::vector<unsigned int> fits;
	unsigned id = 1;
	arma::mat data(n,dim);
	data.randu();
	for (int i = 0; i < n; ++i) {
		fits.push_back(id);
	}

	arma::mat initMatrix(n,dim);
	for(int k = 0; k < n; ++k) {
		for (int j = 0; j < dim; ++j) {
			initMatrix(k,j) = data(k,j);
		}
	}

	boost::shared_ptr<Cluster> m(new ClusterSpherical(id,fits,initMatrix));
	// Dodajemy element o indeksie i
	for (int i = n-1; i > end; --i) {

		arma::mat tmpMatrix(i,dim);
		for (int j = 0; j <i; ++j) {
			tmpMatrix.row(j) = data.row(j);
		}
		arma::mat covariance= cov(tmpMatrix,1);
		arma::mat realM = mean(tmpMatrix);

		arma::rowvec point(data.row(i));
		m = m->removePoint(point);
		ClusterStandard tmp(id,fits,tmpMatrix);

		ClusterOnlyTrace * upref = dynamic_cast<ClusterOnlyTrace*>(m.get());
		arma::rowvec meanOnlineDifference = upref->getMean() - realM;
		float traceDiff = upref->getCovMatTrace() - arma::trace(covariance);
		float relativeError = std::abs(traceDiff/arma::trace(covariance));
		std::cout << i << " " << relativeError << std::endl;
		EXPECT_EQ(m->size(),tmp.size());
		EXPECT_LT(std::abs(traceDiff), acceptableDifference);
		for (int j = 0; j < dim; ++j) {
			EXPECT_LT(std::abs(meanOnlineDifference(j)),acceptableDifference) << "at position" << j << " means differ by more than " << acceptableDifference;
		}
	}
	ASSERT_TRUE(true);
}
