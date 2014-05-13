#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

class Algorithm {
public:
  virtual void loop(arma::mat &points, std::vector<unsigned int> &assignment,
		    float killThreshold, std::vector<Cluster> &clusters)=0;
  virtual int singleLoop(arma::mat &points, std::vector<unsigned int> &assignment,
			 float killThreshold, std::vector<Cluster> &clusters)=0;
  virtual ~Algorithm() {};
};

#endif
