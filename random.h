#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <stdint.h>
#include <math.h>
#include <stdlib.h>

#define USE_CPLUSPLUS_11 1

#ifndef USE_CPLUSPLUS_11
#define USE_CPLUSPLUS_11		(__cplusplus > 0)
#endif

#if USE_CPLUSPLUS_11
#include <random>
using namespace std;
#else
struct xorshf {
    typedef uint32_t result_type;

    // Mersenne xorshf generator
    // http://stackoverflow.com/questions/1640258/need-a-fast-random-generator-for-c
    result_type x, y, z, w;

    xorshf ( result_type seed = 123456789 ) {
        x = seed;
        y = 362436069;
        z = 521288629;
        w = 88675123;
    }

    inline result_type operator ()( ) {
        result_type t;

        t = x ^ (x << 11);
        x = y; y = z; z = w;
        return ( w = w ^ (w >> 19) ^ (t ^ (t >> 8)) );
    }

    inline void seed ( result_type seed = 123456789 ) {
        x = seed;
    }

    inline static constexpr result_type min ( ) {
        return 0;
    }

    inline static constexpr result_type max ( ) {
        return 0xFFFFFFFF;
    }
};
#endif



template<class G>
class RandomBase {
public:
    typedef typename G::result_type result_type;

    RandomBase ( result_type seed = 123456789 ) :
        generator( seed ) {
    }

    // get uniformly an integer in [0,max())
    result_type integer ( ) {
        return gen();
    }

    // get uniformly a real in [0,1)
    double real ( ) {
        return gen() / (double) G::max();
    }

    // get uniformly a real in (-1,1)
    double realnegative ( ) {
        return ( signed(gen()) << 1 ) / (double) G::max();
    }

    // get uniformly a real in [0,2)
    double real2 ( ) {
        return gen() / G::max() * 2.0;
    }

    // get uniformly a real in (-2,2)
    double real2negative ( ) {
        return ( signed(gen()) << 1 ) / G::max() * 2.0;
    }

    result_type operator() ( result_type n = max() ) {
        return gen() % n;
    }

    // change the seed
    void seed ( result_type seed ) {
        generator.seed( seed );
    }

    static constexpr result_type max ( ) {
        return G::max();
    }

    // get uniformly a real in the unit disk
    double realdisk ( double& x, double& y ) {
        // this is usually a lot faster than using sin, cos and sqrt (below),
        // since the probability that a point is accepted is high: pi/4
        double s;
        while ( 1 ) {
            x = realnegative();
            y = realnegative();

            s = x * x + y * y;
            if ( s <= 1.0 ) return s;
        }

        // all generators except mt19937 can generate artifacts with the following
        // procedure that choose an angle in [0,2pi) and a radius in [0,1) and then
        // project the point on x and y. I don't know why.
        // http://mathworld.wolfram.com/DiskPointPicking.html
        /*double phi = M_PI * real2();
        double r = sqrt( real() );
        x = r * cos( phi );
        y = r * sin( phi );*/
    }

    // generate a two dimension random point normally distributed (mean = 0, variance = 1)
    void gaussian ( double& x, double& y ) {
        // this is the exact one (Marsaglia polar method applied to Box-Muller transform)
        double s = realdisk( x, y );
        double factor = sqrt( -2.0 * log( s ) / s );
        x = x * factor;
        y = y * factor;

        // this is approximated and uses the central limit thorem
        /*int64_t tmp = 0;
        for ( int i = 0; i < 12; ++i ) tmp += gen();
        x = tmp / (double) RAND_MAX - 6.0; tmp = 0;
        for ( int i = 0; i < 12; ++i ) tmp += gen();
        y = tmp / (double) RAND_MAX - 6.0;*/

    }

    void gaussian ( double& x, double& y, double radius ) {
        double s = realdisk( x, y );
        double factor = sqrt( -2.0 * log( s ) / s ) * radius;
        x = x * factor;
        y = y * factor;
    }

    // generate a two dimension random point exponentially distributed
    // (actually this is not really exponential i think but hower is much more
    // dense aroun the origin than the gaussian mutation)
    void exponential ( double& x, double& y ) {
        double s = realdisk( x, y );
        double factor = -log( s ) / s;
        x = x * factor;
        y = y * factor;
    }

    void exponential ( double& x, double& y, double radius ) {
        double s = realdisk( x, y );
        double factor = -log( s ) / s * radius;
        x = x * factor;
        y = y * factor;
    }

private:
    G generator;

    inline result_type gen ( ) {
        static_assert( G::min() == 0, "RandomBase requires a generator with ::min() = 0" );

        return generator() & G::max();
    }
};


#if USE_CPLUSPLUS_11
typedef RandomBase<mt19937> Random;
#else
typedef RandomBase<xorshf>  Random;
#endif


#endif // RANDOM_H
