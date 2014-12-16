#include "kmeanspp_assignment.hpp"

namespace gmum {

void KmeansppAssignment::operator()(std::vector<unsigned int> &assignment) {

	std::vector<unsigned int> centers;
	centers.reserve(m_nclusters);

    unsigned int npoints = assignment.size();

	static int seed = time(NULL);
	static boost::random::mt19937 gen(seed);

	//select points for centers
	std::list<Pair> selected;
    unsigned int nsections = 100;
    unsigned int section = npoints / nsections;
	if (section <= 1)
        for (unsigned int i = 0; i < npoints; ++i)
			selected.push_back(i);
	else {
		boost::random::uniform_int_distribution<> uniform(0, section - 1);
		unsigned int k = 0;
		//nrOfPoints > section
        for (; k < npoints - section; k += section) {
			unsigned int target = k + uniform(gen);
			selected.push_back(target);
		}
		k += uniform(gen);
        if (k >= npoints)
            k = npoints - 1;
		selected.push_back(k);
	}

    int centers_left = m_nclusters;
	//first center
    centers.push_back(selected.front().point_number);
	selected.pop_front();
    --centers_left;

	//choose other centers
	float bernoulli_p = 0.8;
	boost::random::bernoulli_distribution<> bernoulli(bernoulli_p);

    for (; centers_left > 0; --centers_left) {
		calculate_distance(centers, selected, m_points);
		choose(bernoulli, gen, selected);
        centers.push_back(selected.front().point_number);
		selected.pop_front();
	}

	assign_points(assignment, centers, m_points);
}

void calculate_distance(const std::vector<unsigned int> &centers,
		std::list<Pair> &selected, const arma::mat &points) {

	for (std::list<Pair>::iterator it = selected.begin(); it != selected.end();
			++it) {
        arma::rowvec point = points.row(it->point_number);
		float distance = std::numeric_limits<float>::max();

		for (unsigned int i = 0; i < centers.size(); ++i) {
			arma::rowvec vec = points.row(centers[i]) - point;
            float temp_dist = arma::as_scalar(vec * vec.t());
            if (distance > temp_dist)
                distance = temp_dist;
		}

		it->distance = distance;
	}

	selected.sort();
}

std::list<Pair>::iterator choose(
		boost::random::bernoulli_distribution<> &bernoulli,
		boost::random::mt19937 &gen, std::list<Pair> &selected) {

	std::list<Pair>::iterator it = selected.begin();
	for (; !bernoulli(gen) && it != selected.end(); ++it)
		;

	if (it == selected.end()) {
		it = selected.end();
		--it;
	}

	return it;
}
}
