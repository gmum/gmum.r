#include <armadillo>
#include "Cluster.hpp"

#ifndef HARTIGAN_HPP
#define HARTIGAN_HPP

class Hartigan {
private:
  std::vector<Cluster> clusters;
  /*
   * fits is a function. 
   * fits : {index of x : x \in points} -> {index of y : y \in clusters}
   * it is used to remember which cluster a point from dataset belong to.
   */
  std::vector<int> &fits;
  /*
   * dataset. A point is stored as a row.
   */
  arma::mat &points;
  /*
   * in hartigan algorithm a cluster is removed when its size
   * is less than certain fraction of number of points in dataset.
   * This fraction is the killThreshold (cluster gets 'killed'
   * when threshold is reached).
   */
  const float killThreshold;
public:
  Hartigan(int numberOfClusters, float killThreshold,
	   std::vector<int> &fits, arma::mat &points);
  /*
   *
   */
  void loop();
  /*
   *
   */
  int singleLoop();
  /*
   *
   */
  double entropy();
};

#endif
