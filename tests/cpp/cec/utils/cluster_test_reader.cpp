#include "cluster_test_reader.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
ClusterTestReader::ClusterTestReader(std::string _dir) :
		dir(_dir) {
}

std::string ClusterTestReader::getPrefix() {
	std::string result("../data/");
	result.append(dir);
	return result;
}

std::string ClusterTestReader::getDataPath() {
	return getPrefix() + "data.txt";
}

std::string ClusterTestReader::getAnswerPath() {
	return getPrefix() + "answer.txt";
}

void ClusterTestReader::readData(int dim,
		std::vector<Rcpp::NumericVector> & out) {
	std::ifstream file(getDataPath().c_str());
	if (file.is_open()) {
		std::string line;
		while (file.good()) {
			std::getline(file, line);
			Rcpp::NumericVector v(dim);
			std::stringstream ss(line);
			for (int i = 0; i < dim; ++i) {
				double x;
				ss >> x;
				v[i] = x;
			}
			out.push_back(v);
		}
	} else {
		std::cout << "Failed to open file " << getDataPath() << std::endl;
	}
}

void ClusterTestReader::readAnswer(std::vector<int> & out) {
	std::ifstream file(getAnswerPath().c_str());
	if (file.is_open()) {
		std::string line;
		while (file.good()) {
			std::getline(file, line);
			std::stringstream ss(line);
			int v;
			ss >> v;
			out.push_back(v);
		}
	} else {
		std::cout << "Failed to open file " << getAnswerPath() << std::endl;
	}
}

