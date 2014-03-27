#ifndef CLUSTER_TEST_READER_HPP
#define CLUSTER_TEST_READER_HPP

#include <Rcpp.h>
#include <iostream>
#include <vector>


class ClusterTestReader{
private :
  std::string dir;
  std::string getPrefix();
  std::string getDataPath();
  std::string getAnswerPath();
public :
  ClusterTestReader(std::string _dir);
  void readData(int dim,std::vector<Rcpp::NumericVector> & out);
  void readAnswer(std::vector<int> & out);
  
};

#endif
