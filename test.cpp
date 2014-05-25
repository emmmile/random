
#include <iostream>
#include <iomanip>
#include <random>
#include <chrono>
#include <cassert>
#include "random.h"
#include "timer.h"
using namespace std;

const int stop = 100000000;

template<class G>
void single_run (G& generator, size_t times ) {
    typename G::result_type total = 0;

    for ( size_t i = 0; i < times; ++i )
        total += generator();

    if ( total == 0 ) cout << "This is to avoid compiler optimizations." << endl;
}

template<class G>
double average_time(G& generator, size_t trials, size_t times ) {
    double average_time = 0.0;

    for ( size_t i = 0; i < trials; ++i ) {
        timer elapsed;
        single_run( generator, times );
        average_time += elapsed.elapsed();
    }

    return trials * times / average_time / 1000000.0;
}

template<class Real, class G>
void canonical_single_run (G& generator, size_t times ) {
    Real total = 0.0;

    for ( size_t i = 0; i < times; ++i )
        total += generate_canonical<Real, sizeof(Real)>(generator);

    if ( total == 0 ) cout << "This is to avoid compiler optimizations." << endl;
}

template<class Real, class G>
double canonical_average_time(G& generator, size_t trials, size_t times ) {
    double average_time = 0.0;

    for ( size_t i = 0; i < trials; ++i ) {
        timer elapsed;
        canonical_single_run<Real>( generator, times );
        average_time += elapsed.elapsed();
    }

    return trials * times / average_time / 1000000.0;
}

template<class Real, class G, class D>
void dist_single_run (D& dist, G& generator, size_t times ) {
    Real total = 0.0;

    for ( size_t i = 0; i < times; ++i )
        total += dist(generator);

    if ( total == 0 ) cout << "This is to avoid compiler optimizations." << endl;
}

template<class Real, class G, class D>
double dist_average_time(D& dist, G& generator, size_t trials, size_t times ) {
    double average_time = 0.0;

    for ( size_t i = 0; i < trials; ++i ) {
        timer elapsed;
        dist_single_run<Real>( dist, generator, times );
        average_time += elapsed.elapsed();
    }

    return trials * times / average_time / 1000000.0;
}



int main ( ) {
    typedef double Real;
    cout.precision(2);
    random_device rd;

    size_t trials = 5;
    size_t times = 100000000;

    cout << "Testing base generators..." << endl;

    xorshf xorgen( rd() );
    cout << "Generator: xorshf\tspeed: " << fixed << average_time(xorgen, trials, times) << " M/s" << endl;

    xorshf64 xorgen64( rd() );
    cout << "Generator: xorshf64\tspeed: " << fixed << average_time(xorgen64, trials, times) << " M/s" << endl;

    mt19937 mtgen( rd() );
    cout << "Generator: mt19937\tspeed: " << fixed << average_time(mtgen, trials, times) << " M/s" << endl;

    minstd_rand minstdgen( rd() );
    cout << "Generator: minstd\tspeed: " << fixed << average_time(minstdgen, trials, times) << " M/s" << endl;


    cout << endl << "Testing canonical [0,1) distribution..." << endl;
    cout << "Generator: xorshf\tspeed: " << fixed << canonical_average_time<Real>(xorgen, trials, times) << " M/s" << endl;
    cout << "Generator: xorshf64\tspeed: " << fixed << canonical_average_time<Real>(xorgen64, trials, times) << " M/s" << endl;
    cout << "Generator: mt19937\tspeed: " << fixed << canonical_average_time<Real>(mtgen, trials, times) << " M/s" << endl;
    cout << "Generator: minstd\tspeed: " << fixed << canonical_average_time<Real>(minstdgen, trials, times) << " M/s" << endl;


    uniform_real_distribution<> dist(-1, 1);
    cout << endl << "Testing [-1,1) distribution..." << endl;
    cout << "Generator: xorshf\tspeed: " << fixed << dist_average_time<Real>(dist, xorgen, trials, times) << " M/s" << endl;
    cout << "Generator: xorshf64\tspeed: " << fixed << dist_average_time<Real>(dist, xorgen64, trials, times) << " M/s" << endl;
    cout << "Generator: mt19937\tspeed: " << fixed << dist_average_time<Real>(dist, mtgen, trials, times) << " M/s" << endl;
    cout << "Generator: minstd\tspeed: " << fixed << dist_average_time<Real>(dist, minstdgen, trials, times) << " M/s" << endl;


    return 0;
}
