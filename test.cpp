
#include <iostream>
#include <random>
#include "random.hpp"
using namespace std;

const int stop = 100000000;

// read this for improvements
// http://gcc.gnu.org/ml/gcc-patches/2012-08/msg01979.html

int main ( ) {
    Random gen;
    double val = 0.0;
    for ( int i = 0; i < stop; ++i ) {
        val += gen.real();
    }

//	double val = 0.0;
//	default_random_engine eng( 0 );
//	mt19937 eng( 0 );
//	uniform_real_distribution<double> uniform_real(0.0,1.0);
//	for ( int i = 0; i < stop; ++i ) {
//		val += uniform_real(eng);
//	}

	cout << val / stop << endl;

	return 0;
}
