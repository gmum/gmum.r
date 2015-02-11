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
#include "boost/foreach.hpp"
using namespace std;
typedef vector<int> VI;
typedef long long LL;
#define FOR(x, b, e) for(int x=b; x<=(e); ++x)
#define FORD(x, b, e) for(int x=b; x>=(e); ––x)
#define REP(x, n) for(int x=0; x<(n); ++x)
#define VAR(v,n) typeof(n) v=(n)
#define ALL(c) c.begin(),c.end()
#define SIZE(x) (int)(x).size()
#define FOREACH(i,c) BOOST_FOREACH(i, c) //for(VAR(i,(c).begin());i!=(c).end();++i)
#define IWAS(x) cout<<x<<endl<<flush;
#define PB push_back
#define ST first
#define ND second

#include <sstream>
#include <string>
#include <map>
#include <cmath>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

#include <utils/logger.h>

#ifdef DEBUG
	#define DBG_2(logger, level, text) logger->log(level, text);
	#define REPORT_2(x) cerr<<#x<<"="<<(x)<<endl<<std::flush;
#else
	#define DBG_2(verbosity, level, text)
	#define REPORT_2(x)
#endif

#ifdef DEBUG
	#define DBG(logger, level, text) logger->log(level, text);
	#define REPORT(x) cout<<#x<<"="<<(x)<<endl<<std::flush;
#else
	#define DBG(verbosity, level, text)
	#define REPORT(x)
#endif

#define REPORT_PRODUCTION(x) cout<<#x<<"="<<(x)<<endl<<std::flush;

void __init_rnd();
int __rnd(int min, int max);
int __int_rnd(int min, int max);
double __double_rnd(double min, double max);

template<class T>
void write_array(T* begin, T*end) {
	for (; begin != end; ++begin) {
		std::cerr << *begin << ",";
	}
	std::cerr << endl;
}

template<class T>
void write_cnt(T begin, T end) {
	for (; begin != end; ++begin) {
		std::cerr << *begin << ",";
	}
	std::cerr << endl;
}

template<class T>
std::string write_cnt_str(T begin, T end) {
	std::stringstream ss;

	for (; begin != end; ++begin) {
		ss << *begin << ",";
	}
	return ss.str();
}

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

#include <string.h>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

const int __one__ = 1;
const bool isCpuLittleEndian = 1 == *(char*) (&__one__); // CPU endianness
const bool isFileLittleEndian = false;  // output endianness - you choose :)

void _write_bin(ostream & out, double v);

void _write_bin_vect(ostream & out, vector<double> & v);

double _load_bin(istream & in);

vector<double> _load_bin_vector(istream & in);

#endif	/* UTILS_H */
