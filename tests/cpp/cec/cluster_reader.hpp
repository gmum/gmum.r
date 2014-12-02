#ifndef CLUSTERREADER_HPP
#define CLUSTERREADER_HPP

#include <RcppArmadillo.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

class ClusterReader {
protected:
	std::string prefix();
	std::string inputPath();
	std::string clusterPath();
	std::string energyPath();
	std::string dimensionPath();
	std::string folderName;
	std::vector<std::vector<double> > points;
	std::vector<unsigned int> clustering;
	double energy;
	unsigned int dim;
public:
	ClusterReader(const char * _name, unsigned int _dim);
	ClusterReader(const char * _name);
	void readPoints();
	void readClustering();
	void readDimension();

	void getPoints(std::vector<std::vector<double> > & out);
	void getClustering(std::vector<unsigned int> & out);
	void readEnergy();
	double getEnergy();
	double getDimension();
	arma::mat getPointsInMatrix();

};

#endif
