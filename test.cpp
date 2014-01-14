
#include <iostream>
#include <random>
#include <cassert>
#include "random.h"
using namespace std;

const int stop = 10000000;

// read this for improvements
// http://gcc.gnu.org/ml/gcc-patches/2012-08/msg01979.html

int main ( ) {
    Random gen;
    double sum = 0.0;
    for ( int i = 0; i < stop; ++i ) {
        assert( gen.real() < 1.0 );
        assert( gen() < Random::max() );
        assert( gen() >= 0 );
        assert( gen.real2negative() > -1.0 );
        assert( gen.real2negative() < 1.0 );

        sum += gen.realnegative();
    }

    cout << sum / stop << endl;

	return 0;
}
