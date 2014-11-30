#include "cluster.hpp"

namespace gmum {

Cluster::Cluster(int _count, const arma::rowvec &_mean) :
		count(_count), mean(_mean) {
	N = mean.n_elem;
}

Cluster::Cluster(unsigned int id, const std::vector<unsigned int> &assignment,
		const arma::mat &points) {
	initializeMean(id, assignment, points);
	if (count == 0)
		throw(NoPointsInCluster());

	N = points.n_cols;
}

arma::rowvec Cluster::initializeMean(unsigned int id,
		const std::vector<unsigned int> &assignment, const arma::mat &points) {
	int dimention = points.n_cols;
	count = 0;
	mean = arma::rowvec(dimention, arma::fill::zeros);
	for (unsigned int i = 0; i < points.n_rows; i++) {
		if (assignment[i] == id) {
			mean += points.row(i);
			count += 1;
		}
	}
	mean = mean / count;

	return mean;
}

int Cluster::size() const {
	return count;
}

arma::rowvec Cluster::getMean() {
	return mean;
}

double Cluster::entropy() const {
	return _entropy;
}

void ClusterUseCovMat::initializeCovarianceMatrix(unsigned int id,
		const std::vector<unsigned int> &assignment, const arma::mat &points) {
	int dimension = points.n_cols;
	arma::rowvec m = mean;
	arma::mat out(dimension, dimension, arma::fill::zeros);
	for (unsigned int i = 0; i < points.n_rows; i++)
		if (assignment[i] == id) {
			arma::rowvec point = points.row(i);
			arma::rowvec tmp = point - m;
			out += (tmp.t() * tmp) / (count);
		}

	covMat = out;
}

ClusterUseCovMat::ClusterUseCovMat(int _count, const arma::rowvec &_mean,
		const arma::mat &_covMat) :
		Cluster(_count, _mean), covMat(_covMat) {
}

ClusterUseCovMat::ClusterUseCovMat(unsigned int id,
		const std::vector<unsigned int> &assignment, const arma::mat &points) :
		Cluster(id, assignment, points) {
	initializeCovarianceMatrix(id, assignment, points);
}

boost::shared_ptr<Cluster> ClusterUseCovMat::addPoint(
		const arma::rowvec &point) {
	int ncount = count + 1;
	arma::rowvec nmean = (count * mean + point) / (ncount);
	arma::rowvec r = mean - point;
	arma::mat nCovMat = (1.0 * count / ncount)
			* (covMat + (r.t() * r) / ncount);
	return createInstance(ncount, nmean, nCovMat);
}

boost::shared_ptr<Cluster> ClusterUseCovMat::removePoint(
		const arma::rowvec &point) {
	int ncount = count - 1;
	arma::rowvec nmean = (count * mean - point) / ncount;
	arma::rowvec r = mean - point;
	arma::mat nCovMat = (1.0 * count / ncount)
			* (covMat - (r.t() * r) / ncount);
	return createInstance(ncount, nmean, nCovMat);
}

arma::mat ClusterUseCovMat::getCovMat(unsigned int id,
		const std::vector<unsigned int> &assignment, const arma::mat &points) {
	return covMat;
}

ClusterOnlyTrace::ClusterOnlyTrace(int _count, const arma::rowvec & _mean,
		double _covMatTrace) :
		Cluster(_count, _mean), covMatTrace(_covMatTrace) {
}

ClusterOnlyTrace::ClusterOnlyTrace(unsigned int id,
		const std::vector<unsigned int> & assignment, const arma::mat & points) :
		Cluster(id, assignment, points) {
	initializeMean(id, assignment, points);
	computeCovMatTrace(id, assignment, points);
}

void ClusterOnlyTrace::computeCovMatTrace(unsigned int id,
		const std::vector<unsigned int> &assignment, const arma::mat &points) {
	covMatTrace = 0.0;
	for (unsigned int i = 0; i < points.n_rows; i++)
		if (assignment[i] == id) {
			arma::rowvec point = points.row(i);
			arma::rowvec tmp = point - mean;
			covMatTrace += dot(tmp, tmp);
		}
	covMatTrace /= count;
}

boost::shared_ptr<Cluster> ClusterOnlyTrace::addPoint(
		const arma::rowvec & point) {
	int ncount = count + 1;
	arma::rowvec nmean = (count * mean + point) / (ncount);
	arma::rowvec r = nmean - point;
	arma::rowvec meanDiff = mean - nmean;
	double ntrace =
			((covMatTrace + dot(meanDiff, meanDiff)) * count + dot(r, r))
					/ ncount;
	return createInstance(ncount, nmean, ntrace);
}

boost::shared_ptr<Cluster> ClusterOnlyTrace::removePoint(
		const arma::rowvec &point) {
	int ncount = count - 1;
	arma::rowvec nmean = (count * mean - point) / (ncount);
	arma::rowvec meanDiff = mean - nmean;
	arma::rowvec r = nmean - point;
	double ntrace =
			((covMatTrace + dot(meanDiff, meanDiff)) * count - dot(r, r))
					/ ncount;
	return createInstance(ncount, nmean, ntrace);
}

double ClusterOnlyTrace::getCovMatTrace() {
	return covMatTrace;
}

arma::mat ClusterOnlyTrace::getCovMat(unsigned int id,
		const std::vector<unsigned int> &assignment, const arma::mat &points) {
	arma::mat out(N, N, arma::fill::zeros);
	for (unsigned int i = 0; i < points.n_rows; i++) {
		if (assignment[i] == id) {
			arma::rowvec point = points.row(i);
			arma::rowvec tmp = point - mean;
			out += (tmp.t() * tmp) / (count);
		}
	}
	return out;
}

void ClusterStandard::calculateEntropy() {
	_entropy = N * log(2 * M_PI * M_E) / 2 + log(arma::det(covMat)) / 2;
}

boost::shared_ptr<ClusterUseCovMat> ClusterStandard::createInstance(int _count,
		const arma::rowvec &_mean, const arma::mat &_covMat) {
	return boost::shared_ptr < ClusterUseCovMat
			> (new ClusterStandard(_count, _mean, _covMat));
}

ClusterStandard::ClusterStandard(int _count, const arma::rowvec &_mean,
		const arma::mat &_covMat) :
		ClusterUseCovMat(_count, _mean, _covMat) {
	calculateEntropy();
}

ClusterStandard::ClusterStandard(unsigned int id,
		const std::vector<unsigned int> &assignment, const arma::mat &points) :
		ClusterUseCovMat(id, assignment, points) {
	calculateEntropy();
}

ClusterCovMat::ClusterCovMat(const arma::mat & sigma, unsigned int id,
		const std::vector<unsigned int> &assignment, const arma::mat &points) :
		ClusterUseCovMat(id, assignment, points) {
	sigmaDet = arma::det(sigma);
	invSigma = arma::inv(sigma);
	calculateEntropy();
}

ClusterCovMat::ClusterCovMat(const arma::mat & _invSigma, double _sigmaDet,
		int _count, const arma::rowvec & _mean, const arma::mat & covMat) :
		ClusterUseCovMat(_count, _mean, covMat), invSigma(_invSigma), sigmaDet(
				_sigmaDet) {
	calculateEntropy();
}

void ClusterCovMat::calculateEntropy() {
	_entropy = N * log(2 * M_PI) / 2 + arma::trace(invSigma * covMat) / 2
			+ N * log(sigmaDet) / 2;
}
boost::shared_ptr<ClusterUseCovMat> ClusterCovMat::createInstance(int _count,
		const arma::rowvec & _mean, const arma::mat & _covMat) {
	return boost::shared_ptr < ClusterUseCovMat
			> (new ClusterCovMat(invSigma, sigmaDet, _count, _mean, _covMat));
}

ClusterConstRadius::ClusterConstRadius(double r, unsigned int id,
		const std::vector<unsigned int> &assignment, const arma::mat &points) :
		ClusterOnlyTrace(id, assignment, points), r(r) {
	calculateEntropy();
}

ClusterConstRadius::ClusterConstRadius(double r, int _count,
		const arma::rowvec & _mean, double _covMatTrace) :
		ClusterOnlyTrace(_count, _mean, _covMatTrace), r(r) {
	calculateEntropy();
}

void ClusterConstRadius::calculateEntropy() {
	_entropy = N * log(2 * M_PI) / 2 + covMatTrace / (2 * r) + N * log(r) / 2;
}

boost::shared_ptr<ClusterOnlyTrace> ClusterConstRadius::createInstance(
		int _count, const arma::rowvec & _mean, double _covMatTrace) {
	return boost::shared_ptr < ClusterOnlyTrace
			> (new ClusterConstRadius(r, _count, _mean, _covMatTrace));
}

ClusterSpherical::ClusterSpherical(unsigned int id,
		const std::vector<unsigned int> &assignment, const arma::mat &points) :
		ClusterOnlyTrace(id, assignment, points) {
	calculateEntropy();
}

ClusterSpherical::ClusterSpherical(int _count, const arma::rowvec &_mean,
		double _covMatTrace) :
		ClusterOnlyTrace(_count, _mean, _covMatTrace) {
	calculateEntropy();
}

void ClusterSpherical::calculateEntropy() {
	_entropy = N * log(2 * M_PI * M_E / N) / 2 + N * log(covMatTrace) / 2;
}

boost::shared_ptr<ClusterOnlyTrace> ClusterSpherical::createInstance(int _count,
		const arma::rowvec & _mean, double _covMatTrace) {
	return boost::shared_ptr < ClusterOnlyTrace
			> (new ClusterSpherical(_count, _mean, _covMatTrace));
}

ClusterDiagonal::ClusterDiagonal(unsigned int id,
		const std::vector<unsigned int> &assignment, const arma::mat &points) :
		ClusterUseCovMat(id, assignment, points) {
	calculateEntropy();
}

ClusterDiagonal::ClusterDiagonal(int _count, const arma::rowvec & _mean,
		const arma::mat & _covMat) :
		ClusterUseCovMat(_count, _mean, _covMat) {
	calculateEntropy();
}

void ClusterDiagonal::calculateEntropy() {
	_entropy = N * log(2 * M_PI * M_E) / 2
			+ log(arma::det(arma::diagmat(covMat))) / 2;
}

boost::shared_ptr<ClusterUseCovMat> ClusterDiagonal::createInstance(int _count,
		const arma::rowvec & _mean, const arma::mat & _covMat) {
	return boost::shared_ptr < ClusterUseCovMat
			> (new ClusterDiagonal(_count, _mean, _covMat));
}
}
