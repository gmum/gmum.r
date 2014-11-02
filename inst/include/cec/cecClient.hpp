#ifndef CECCLIENT_HPP_
#define CECCLIENT_HPP_

#include "cec.hpp"
#include "cecConfiguration.hpp"
#include "hartigan.hpp"
#include "assignment.hpp"
#include "randomAssignment.hpp"
#include "kmeansppAssignment.hpp"
#include "centroidsAssignment.hpp"

namespace gmum {

class cecClient {
private:
	cecConfiguration config;
public:
	cecClient(){}
	cecClient(cecConfiguration*);

	/**
	 * Assign points to clusters and learn model config->params.nstart number of times.
	 */
	CEC* findBestCEC();
};

} /* namespace gmum */

#endif /* CECCLIENT_HPP_ */
