/* 
 * File:   Utils.h
 * Author: staszek
 *
 * Created on 10 sierpień 2012, 16:53
 */

#ifndef UTILS_H
#define	UTILS_H

#include <exception>
#include <vector>
#include <sstream>
#include <string>
#include <map>
#include <cmath>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <utils/logger.h>
#include "boost/foreach.hpp"
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;
typedef vector<int> VI;
#define FOR(x, b, e) for(size_t x=b; x<=(e); ++x)
#define FORD(x, b, e) for(size_t x=b; x>=(e); ––x)
#define REP(x, n) for(size_t x=0; x<(n); ++x)
#define VAR(v,n) typeof(n) v=(n)
#define SIZE(x) (int)(x).size()
#define FOREACH(i,c) BOOST_FOREACH(i, c) //for(VAR(i,(c).begin());i!=(c).end();++i)


#ifdef DEBUG_GMUM
	#ifdef RCPP_INTERFACE
	  #include <RcppArmadillo.h>
	  using namespace Rcpp;
	  #define DBG(logger, level, text) logger.log(level, text);
	  #define REPORT(x) Rcpp::Rcout<<#x<<"="<<(x)<<endl<<std::flush;
	#else
	  #define EXIT(x) exit(x);
	  #define DBG(logger, level, text) logger.log(level, text);
	  #define REPORT(x) cout<<#x<<"="<<(x)<<endl<<std::flush;
	#endif
	#define ASSERT(x) assert(x);
#else
	#define DBG(verbosity, level, text)
	#define REPORT(x)
	#define ASSERT(x)
#endif

#ifdef RCPP_INTERFACE
#define EXIT(x) Rcpp::stop("");
#else
#define EXIT(x) exit(x);
#endif

template<class T>
std::string to_str(const T& x) {
	stringstream ss;
	ss << x;
	return ss.str();
}

struct BasicException: public std::exception {
	std::string s;
	BasicException(std::string ss) :
			s(ss) {
	}
	~BasicException() throw () {
	} // Updated
	const char* what() const throw () {
		return s.c_str();
	}
};

//conflicting with boost namespace
namespace gmum {
  template<class T>
  std::string to_string(const T& x) {
  	stringstream ss;
  	ss << x;
  	return ss.str();
  }
}

const int __one__ = 1;
const bool isCpuLittleEndian = 1 == *(char*) (&__one__); // CPU endianness
const bool isFileLittleEndian = false;  // output endianness - you choose :)

static int __seed(int seed){
    srand(seed);
}

#define RANDOM_INT(rng, min, max) (rng() % (max - min +1) + min)

static int __rnd(int min, int max) {
	return (rand() % (max - min + 1) + min);
}

static int __int_rnd(int min, int max) {
	return (rand() % (max - min + 1) + min);
}

static double __double_rnd(double min, double max) {
	return min + (max - min) * ((double) rand()) / RAND_MAX;
}

static void _write_bin(ostream & out, double v) {
	if (isCpuLittleEndian ^ isFileLittleEndian) {
		// Switch between the two
		char data[8], *pDouble = (char*) (double*) (&v);
		for (int i = 0; i < 8; ++i) {
			data[i] = pDouble[7 - i];
		}
		out.write(data, 8);
	} else
		out.write((char*) (&v), 8);
}

static void _write_bin_vect(ostream & out, vector<double> & v) {
	_write_bin(out, (double) v.size());
	// TODO: remove
	REPORT(v.size());
	for (size_t i = 0; i < v.size(); ++i) {
		_write_bin(out, v[i]);
	}
}

static double _load_bin(istream & in) {
	char data[8];
	double res;
	in.read(data, 8);
	if (isCpuLittleEndian ^ isFileLittleEndian) {
		char data_load[8];
		// Switch between the two
		for (int i = 0; i < 8; ++i) {
			data_load[i] = data[7 - i];
		}
		memcpy((char*) &res, &data_load[0], 8);
	} else
		memcpy((char*) &res, &data[0], 8);

	return res;
}

static vector<double> _load_bin_vector(istream & in) {
	int N = (int) _load_bin(in);
	vector<double> x;
	x.reserve(N);
	REPORT(N);
	for (int i = 0; i < N; ++i) {
		x.push_back(_load_bin(in));
	}
	return x;
}

int check_argc(const char*);
int check_argc(const std::string);
char** to_argv(const char*);
char** to_argv(const std::string);
char** free_argv(int argc, char** argv);

#endif	/* UTILS_H */
