
namespace gmum {

  struct CONST {
    static const char* dataset;
    static const char* nrOfClusters;
    static const char* clusters;
    static const char* nstart;
    static const char* centroidsList;
    static const char* killThreshold;
    static const char* itmax;
    static const char* energy;
    static const char* logClusters;

    struct CLUSTERS {
      static const char* type;
      static const char* standard;
      static const char* full;
      static const char* diagonal;
      static const char* sphere;
      static const char* fsphere;
      static const char* custom;

      static const char* init;
      static const char* kmeanspp;
      static const char* random;
      static const char* centroids;

      static const char* covMat;
      static const char* radius;
      static const char* functionName;
    };

    static const unsigned int nrOfClustersInit;
    static const double killThresholdInit;
    static const unsigned int nstartInit;
    static const AssignmentType defaultAssignment;

    struct ERRORS {
      static const char* datasetReq;
      static const char* nstartsPositive;
      static const char* nrOfClustersPositive;
      static const char* datasetSize;
      static const char* killThresholdSize;
      static const char* assignmentError;
      static const char* covMatReq;
      static const char* radiusReq;
      static const char* clusterRecError;
      static const char* centroidsError;
      static const char* functionNameReq;
    };
  };

  const char* CONST::dataset = "x";
  const char* CONST::nrOfClusters = "k";
  const char* CONST::clusters = "params.mix";
  const char* CONST::nstart = "params.nstart";  
  const char* CONST::centroidsList = "params.centroids";
  const char* CONST::killThreshold = "control.eps";
  const char* CONST::itmax = "control.itmax";
  const char* CONST::energy = "log.energy";
  const char* CONST::logClusters = "log.ncluster";

  const char* CONST::CLUSTERS::type = "method.type";
  const char* CONST::CLUSTERS::standard = "std";
  const char* CONST::CLUSTERS::full = "full";
  const char* CONST::CLUSTERS::diagonal = "diagonal";
  const char* CONST::CLUSTERS::sphere = "sphere";
  const char* CONST::CLUSTERS::fsphere = "fsphere";
  const char* CONST::CLUSTERS::custom = "func";

  const char* CONST::CLUSTERS::init = "method.init";
  const char* CONST::CLUSTERS::kmeanspp = "kmeans++";
  const char* CONST::CLUSTERS::random = "random";
  const char* CONST::CLUSTERS::centroids = "centroids";

  const char* CONST::CLUSTERS::covMat = "params.cov";
  const char* CONST::CLUSTERS::radius = "params.r";
  const char* CONST::CLUSTERS::functionName = "params.function";
  
  const unsigned int CONST::nrOfClustersInit = 10;
  const double CONST::killThresholdInit = 1e-4;
  const unsigned int CONST::nstartInit = 1;
  const AssignmentType CONST::defaultAssignment = kkmeanspp;

  const char* CONST::ERRORS::datasetReq = "Dataset is required!";
  const char* CONST::ERRORS::nstartsPositive = "Number of starts should be a positive integer!";
  const char* CONST::ERRORS::nrOfClustersPositive = "Number of clusters should be a positive integer!";
  const char* CONST::ERRORS::datasetSize = "Size of dataset cannot be less than number of clusters!";
  const char* CONST::ERRORS::killThresholdSize = (std::string(CONST::killThreshold)+" is too hight").c_str();
  const char* CONST::ERRORS::assignmentError = "Cannot recognize assignment initiation method";
  const char* CONST::ERRORS::covMatReq = "Cavariance matrix required";
  const char* CONST::ERRORS::radiusReq = "Radius is required";
  const char* CONST::ERRORS::clusterRecError = "Cannot recognise cluster type";
  const char* CONST::ERRORS::centroidsError = "Number of centroids is different than number of clusters";
const char* CONST::ERRORS::functionNameReq = "Function is required";

}
