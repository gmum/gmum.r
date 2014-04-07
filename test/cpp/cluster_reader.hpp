#ifndef CLUSTER_READER_HPP
#define CLUSTER_READER_HPP

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <armadillo>
class ClusterReader {
protected:
  std::string prefix();
  std::string inputPath();
  std::string clusterPath();
  std::string folderName;
  std::vector<std::vector<double> > points;
  std::vector<unsigned int> clustering;
  int dim;
public:
  ClusterReader(const char * _name,int _dim);
  void readPoints();
  void readClustering();
  
  void getPoints(std::vector<std::vector<double> > & out);
  void getClustering(std::vector<unsigned int> & out);
  arma::mat getPointsInMatrix();
  
  
};

ClusterReader::ClusterReader(const char * name, int _dim) {
  folderName = std::string(name);
  dim = _dim;
}

std::string ClusterReader::prefix() {
  return std::string("../data/") + folderName + "/";
}

std::string ClusterReader::clusterPath() {
  return prefix() + "cluster.txt";
}

std::string ClusterReader::inputPath() {
  return prefix() + "input.txt";
}

void ClusterReader::readPoints() {
  std::cout << " read points " << std::endl;
  std::ifstream file(inputPath().c_str());
  if(file.is_open()){
    std::string line;
    while(file.good()){
      std::getline(file,line);
      std::stringstream ss(line);
      std::vector<double> currentVector;
      if (line.size() < dim) 
	continue;
      for (int i = 0 ; i < dim ; ++i) {
	double x;
	ss >> x ;
	currentVector.push_back(x);
      }
      points.push_back(currentVector);
    }
    file.close();
  }
  else {
    std::cerr << "Failed to open " << inputPath() << std::endl;
  }
  std::cout << "Finish reading opening. Read " << points.size()  << std::endl;
}

void ClusterReader::readClustering() {
  std::cout << "reading clusters " << std::endl;
  std::ifstream file(clusterPath().c_str());
  if(file.is_open()){
    std::string line;
      while(file.good()){
	std::getline(file,line);
	std::stringstream ss(line);
	if (line.size() ==0)
	  continue;
	unsigned int x;
	ss >> x;
	clustering.push_back(x);
     }
      file.close();
  }
  else {
    std::cerr << "Failed to open " << clusterPath() << std::endl;
  }
  std::cout << "Finished reading clusters. Read " << clustering.size() << std:: endl;
}

void ClusterReader::getPoints(std::vector<std::vector<double> > & out) {
    if (points.size() == 0) 
      readPoints();
    for (std::vector<std::vector<double> >::iterator it = points.begin(); it != points.end() ; ++it) 
    out.push_back(*it);
  
}

void ClusterReader::getClustering(std::vector<unsigned int> & out) {
  if (clustering.size() == 0)
    readClustering();
  for(std::vector<unsigned int>::iterator it = clustering.begin() ; it!= clustering.end(); ++it)
    out.push_back(*it);
}

arma::mat ClusterReader::getPointsInMatrix() {
  if (points.size() == 0)
    readPoints();
  unsigned int n = points.size();
  arma::mat result(n,dim);
  for (int i = 0 ; i <n ; ++i) {
    for (int j = 0 ; j < dim ; ++j) {
      result(i,j) = points[i][j];
    }
  } 
  return result;
}


#endif
