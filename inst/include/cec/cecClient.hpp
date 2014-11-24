#ifndef CECCLIENT_HPP_
#define CECCLIENT_HPP_

#include "cecConfiguration.hpp"
#include "cec.hpp"

class cecClient {
private:
	cecConfiguration config;
public:
	cecClient(cecConfiguration*);

	/**
	 * Assign points to clusters and learn model config->params.nstart number of times.
	 */
    cecModel* findBestCEC();
};

#endif /* CECCLIENT_HPP_ */
