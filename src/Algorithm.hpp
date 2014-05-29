#include <list>

#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

namespace gmum {

  struct SingleResult {
    int switched;
    int nrOfClusters;
    float energy;
    SingleResult() {}
    SingleResult(int switched, int nrOfClusters, float energy) :
      switched(switched), nrOfClusters(nrOfClusters), energy(energy) {}
  };

  struct TotalResult {
    int iterations;
    std::list<unsigned int> nrOfClusters;
    std::list<float> energy;
    TotalResult() : iterations(0) {}
    void append(SingleResult result, bool logNrOfClusters, bool logEnergy) {
      iterations++;
      if(logNrOfClusters) nrOfClusters.push_back(result.nrOfClusters);
      if(logEnergy) energy.push_back(result.energy);
    }
  };

  class Algorithm {
  protected:
    bool logNrOfClusters, logEnergy;
  public:
    Algorithm(bool logNrOfClusters, bool logEnergy)
      : logNrOfClusters(logNrOfClusters), logEnergy(logEnergy) {}
    virtual TotalResult loop(arma::mat &points, std::vector<unsigned int> &assignment,
			     float killThreshold, std::vector<Cluster> &clusters)=0;
    virtual SingleResult singleLoop(arma::mat &points, std::vector<unsigned int> &assignment,
				    float killThreshold, std::vector<Cluster> &clusters)=0;
    virtual ~Algorithm() {};
  };

}

#endif
