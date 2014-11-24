#include "cecClient.hpp"
#include "hartigan.hpp"
#include "randomAssignment.hpp"
#include "kmeansppAssignment.hpp"
#include "centroidsAssignment.hpp"
using namespace gmum;

cecClient::cecClient(cecConfiguration* config) {
	cecConfiguration current_config = *config;
	this->config = current_config;
}

cecModel* cecClient::findBestCEC() {
	boost::shared_ptr < std::vector<unsigned int>
			> assignment(new std::vector<unsigned int>());
	boost::shared_ptr<Hartigan> hartigan(
			new Hartigan(this->config.getParams().logNrOfClusters,
					this->config.getParams().logEnergy));

	Assignment *assignmentType = NULL;
	switch (this->config.getParams().assignmentType) {
	case krandom:
		assignmentType = new RandomAssignment(
				*(this->config.getParams().dataset),
				this->config.getParams().nrOfClusters);
		break;
	case kkmeanspp:
		assignmentType = new KmeansppAssignment(
				*(this->config.getParams().dataset),
				this->config.getParams().nrOfClusters);
		break;
	case kcentroids:
		assignmentType = new CentroidsAssignment(
				*(this->config.getParams().dataset),
				this->config.getParams().centroids);
		break;
	case knoAssignment:
		break; // TODO: handle no assignment
	}

	assignment->resize(this->config.getParams().dataset->n_rows);
	(*assignmentType)(*assignment);

	cecModel *currentCEC = NULL;
	try {
		currentCEC = new cecModel(hartigan, assignment,
				this->config.getParams());

		currentCEC->loop();

		for (unsigned int i = 1; i < cecClient::config.getParams().nstart; ++i) {

			(*assignmentType)(*assignment);
			cecModel *nextCEC = new cecModel(hartigan, assignment,
					this->config.getParams());
			nextCEC->loop();

			if (nextCEC->entropy() < currentCEC->entropy()) {
				delete currentCEC;
				currentCEC = nextCEC;
			}
		}
	} catch (std::exception &e) {
		Rcpp::stop(std::string("exception ") + e.what() + " caught in CEC_new");
	}

	delete assignmentType;
	return currentCEC;
}
