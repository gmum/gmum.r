#include "gng_configuration.h"

void GNGConfiguration::deserialize(std::istream & in)  {
		///Utility constant
		in >> experimental_utility_k;

		///Utility option. Currently supported simples utility
		in >> experimental_utility_option;

		/**Maximum number of nodes*/
		in >> max_nodes; //=1000;
		/**Uniform grid optimization*/
		in >> uniformgrid_optimization; //=true,lazyheap=true;
		/**Lazy heap optimization*/
		in >> lazyheap_optimization;
		/**Bounding box specification*/

		/**Dimensionality of examples*/
		in >> dim;

		REPORT(dim);

		orig = vector<double>(dim, 0);
		axis = vector<double>(dim, 0);

		for (size_t i = 0; i < dim; ++i) {
			in >> axis[i] >> orig[i];
		}
		/**Max edge age*/
		in >> max_age; //=200;
		/**Alpha coefficient*/
		in >> alpha; //=0.95;
		/**Beta coefficient*/
		in >> beta; //=0.9995;
		/**Lambda coefficient*/
		in >> lambda; //=200;
		/**Epsilion v. How strongly move winning node*/
		in >> eps_w; //=0.05;
		/**Memory bound*/
		in >> graph_memory_bound;
		/**Epsilion n*/
		in >> eps_n; //=0.0006;

		in >> verbosity;

		/**Pseudodistance function used (might be non metric)*/
		in >> distance_function;

		/**Type of used database, unsgined int for compabititlity with Rcpp**/
		in >> datasetType;

		/**Initial reserve memory for nodes */
		in >> starting_nodes;

        in >> max_iter;
	}

	void GNGConfiguration::serialize(std::ostream & out) const{
		///Utility constant
		out << experimental_utility_k << endl;

		///Utility option. Currently supported simples utility
		out << experimental_utility_option << endl;

		/**Maximum number of nodes*/
		out << max_nodes << endl; //=1000;
		/**Uniform grid optimization*/
		out << uniformgrid_optimization << endl; //=true,lazyheap=true;
		/**Lazy heap optimization*/
		out << lazyheap_optimization << endl;
		/**Bounding box specification*/

		/**Dimensionality of examples*/
		out << dim << endl;

		REPORT(dim);

		for (size_t i = 0; i < dim; ++i) {
			out << axis[i] << endl << orig[i] << endl;
		}
		/**Max edge age*/
		out << max_age << endl; //=200;
		/**Alpha coefficient*/
		out << alpha << endl; //=0.95;
		/**Beta coefficient*/
		out << beta << endl; //=0.9995;
		/**Lambda coefficient*/
		out << lambda << endl; //=200;
		/**Epsilion v. How strongly move winning node*/
		out << eps_w << endl; //=0.05;
		/**Memory bound*/
		out << graph_memory_bound << endl;
		/**Epsilion n*/
		out << eps_n << endl; //=0.0006;

		out << verbosity << endl;

		/**Pseudodistance function used (might be non metric)*/
		out << distance_function << endl;

		/**Type of used database, unsgined int for compabititlity with Rcpp**/
		out << datasetType << endl;

		/**Initial reserve memory for nodes */
		out << starting_nodes << endl; //imporant not to add endl for binary correctness

        out << max_iter;
	}


	/**Validate server configuration. *Not working now**/
	bool GNGConfiguration::check_correctness() {
		if(alpha <= 0 || alpha > 1){
            cerr << "ERROR: alpha should be in range [0,1]\n";
            return false;        
        }

        if(beta <= 0 || beta > 1){
            cerr << "ERROR: beta should be in range [0,1]\n";
            return false;        
        }

        if(eps_w <= 0 || eps_w > 1){
            cerr << "ERROR: eps_v  should be in range [0,1]\n";
            return false;        
        }

        if(eps_n <= 0 || eps_n > 1){
            cerr << "ERROR: eps_v  should be in range [0,1]\n";
            return false;        
        }

        if(max_age <= 2 || max_age > 10000){
            cerr << "ERROR: max_age  should be in range [2,10000]\n";
            return false;        
        }

        if((max_iter <= 2 && max_iter != -1) || max_nodes <= 2){
            cerr << "ERROR: max_iter and max_nodes  should be in range [2,+inf]\n";
            cerr<<max_iter<< " "<<max_nodes<<endl;
            return false;        

        }

        if(dim <= 0){
            cerr << "ERROR: incorrect dimensionality\n";
            return false;            
        }

        if(lambda <= 0 || lambda > 100000){
            cerr << "ERROR lambda should in range [0, 100000]\n";
            return false;
        }



        if (experimental_utility_option == UtilityOff){
            if(uniformgrid_optimization || lazyheap_optimization) {
			    cerr << "ERROR: please turn OFF optimization when using experimental utility option\n";
			    return false;
		    }
        }

		if (datasetType > 3 or datasetType <= 0) {
			cerr << "ERROR: wrong database specified\n";

			return false;
		}
		if (!(dim < 20 || !uniformgrid_optimization)) {

			cerr
					<< "WARNING: It might be too big dimensionality for OptimizedGNG."
							"OptimizedGNG works best for smaller dimensionality dataset"
							"Consider using PCA or other dim. reduction technique"
							"\n";

		}
		if (!(distance_function == gmum::GNGGraph::Euclidean
				|| !uniformgrid_optimization)) {

			cerr
					<< "ERROR: You can use only Euclidean distance function with uniformgrid optimization\n";
			return false;
		}
		if (!(!uniformgrid_optimization
				or (dim == axis.size() && dim == orig.size()))) {

			cerr << "ERROR: dimensionality doesn't agree with axis and orig"
					<< endl;
			return false;
		}

		return true;
	}
