#ifndef CLUSTERING_COMPARATOR_HPP
#define CLUSTERING_COMPARATOR_HPP

#include <armadillo>
#include <cassert>
class ClusteringComparator {

  
public:
  
  virtual std::vector<unsigned int> computePermutation(unsigned int numberOfClusters, arma::mat & points, std::vector<unsigned int> & realFits, std::vector<unsigned int> & myFits) = 0;

  double correct(std::vector<unsigned int> f,std::vector<unsigned int> & realFits, std::vector<unsigned int> & myFits, unsigned int numberOfElements) {
   int numberOfCorrect =0;
       for (unsigned int i = 0 ; i < numberOfElements ; ++i) 
      numberOfCorrect += ( realFits[i] == f[myFits[i]]);
    //    std::cout << "numberOfCorrect = " << numberOfCorrect << std::endl;
   
    return 1.0*numberOfCorrect/numberOfElements;
  }

  double evaluateClustering(unsigned int numberOfClusters, arma::mat & points, std::vector<unsigned int> & realFits, std::vector<unsigned int> & myFits) {
    std::vector<unsigned int> f = computePermutation(numberOfClusters,points,realFits,myFits);
    return correct(f,realFits,myFits,points.n_rows);
  }
  
};

class BestPermutationComparator : public ClusteringComparator {
public :
  virtual std::vector<unsigned int> computePermutation(unsigned int numberOfClusters, arma::mat & points, std::vector<unsigned int> & realFits, std::vector<unsigned int> & myFits);
};

std::vector<unsigned int> BestPermutationComparator::computePermutation(unsigned int numberOfClusters, arma::mat & points, std::vector<unsigned int> & realFits, std::vector<unsigned int> & myFits) {
  std::vector<unsigned int> current ;
  for (unsigned int i = 0 ; i < numberOfClusters ; ++i) 
    current.push_back(i);
  std::vector<unsigned int> best;
  double bestValue =  0.0;
  do {
    double currentValue = correct(current,realFits,myFits, points.n_rows);
    // std::cout << "Current value " << currentValue << std::endl;
    if (bestValue < currentValue) {
      best = current;
      bestValue = currentValue;
    }
  } while( std::next_permutation(current.begin() , current.end()));
  return best;
  
}


#endif
