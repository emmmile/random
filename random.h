#ifndef RANDOM_H
#define RANDOM_H

#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <limits>
#include <random>
using namespace std;

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

    void discard( unsigned long long z ) {
        for ( unsigned long long i = 0; i < z; ++i )
            this->operator()();
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


// http://en.wikipedia.org/wiki/Xorshift
struct xorshf64 {
    typedef uint64_t result_type;

    result_type s[16];
    int p;

    inline result_type operator ()( ) {
        result_type s0 = s[p];
        result_type s1 = s[p = (p + 1) & 15];
        s1 ^= s1 << 31; // a
        s1 ^= s1 >> 11; // b
        s0 ^= s0 >> 30; // c
        return (s[p] = s0 ^ s1) * 1181783497276652981LL;
    }

    xorshf64 ( result_type seed = 123456789 ) {
        p = seed & 15;
        s[0] = seed;
        discard( seed & 255 );
    }

    //result_type s[2];
    /*inline result_type operator ()( ) {
        result_type s1 = s[0];
        const result_type s0 = s[1];
        s[0] = s0;
        s1 ^= s1 << 23;
        return (s[1] = (s1 ^ s0 ^ (s1 >> 17) ^ (s0 >> 26))) + s0;
    }*/

    void discard( unsigned long long z ) {
        for ( unsigned long long i = 0; i < z; ++i )
            this->operator()();
    }

    inline void seed ( result_type seed = 123456789 ) {
        s[0] = seed;
    }

    inline static constexpr result_type min ( ) {
        return 0;
    }

    inline static constexpr result_type max ( ) {
        return numeric_limits<result_type>::max();
    }
};



template<class Generator, class RealType = double>
class RandomBase {
public:
    typedef typename Generator::result_type IntegralType;

    RandomBase ( IntegralType seed = 123456789 ) :
        generator( seed ) {
    }

    // get uniformly an integer in [0,max())
    IntegralType integer ( ) {
        return gen();
    }

    // get uniformly a real in [0,1)
    RealType real ( ) {
        return gen() / (RealType) Generator::max();
    }

    // get uniformly a real in (-1,1)
    RealType realnegative ( ) {
        return ( signed(gen()) << 1 ) / (RealType) Generator::max();
    }

    // get uniformly a real in [0,2)
    RealType real2 ( ) {
        return gen() / Generator::max() * 2.0;
    }

    // get uniformly a real in (-2,2)
    RealType real2negative ( ) {
        return ( signed(gen()) << 1 ) / Generator::max() * 2.0;
    }

    IntegralType operator() ( IntegralType n = max() ) {
        return gen() % n;
    }

    // change the seed of the underlying generator
    void seed ( IntegralType seed ) {
        generator.seed( seed );
    }

    static constexpr IntegralType max ( ) {
        return Generator::max();
    }

    // Som useful 2-dimensional routines

    // get uniformly a real in the unit disk
    RealType realdisk ( RealType& x, RealType& y ) {
        // this is usually a lot faster than using sin, cos and sqrt (below),
        // since the probability that a point is accepted is high: pi/4
        RealType s;
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
    void gaussian ( RealType& x, RealType& y ) {
        // this is the exact one (Marsaglia polar method applied to Box-Muller transform)
        RealType s = realdisk( x, y );
        RealType factor = sqrt( -2.0 * log( s ) / s );
        x = x * factor;
        y = y * factor;

        // this is approximated and uses the central limit thorem
        /*int64_t tmp = 0;
        for ( int i = 0; i < 12; ++i ) tmp += gen();
        x = tmp / (double) RAND_MAX - 6.0; tmp = 0;
        for ( int i = 0; i < 12; ++i ) tmp += gen();
        y = tmp / (double) RAND_MAX - 6.0;*/

    }

    void gaussian ( RealType& x, RealType& y, RealType radius ) {
        RealType s = realdisk( x, y );
        RealType factor = sqrt( -2.0 * log( s ) / s ) * radius;
        x = x * factor;
        y = y * factor;
    }

    // generate a two dimension random point exponentially distributed
    // (actually this is not really exponential i think but however is much more
    // dense aroun the origin than the gaussian mutation)
    void exponential ( RealType& x, RealType& y ) {
        RealType s = realdisk( x, y );
        RealType factor = -log( s ) / s;
        x = x * factor;
        y = y * factor;
    }

    void exponential ( RealType& x, RealType& y, RealType radius ) {
        RealType s = realdisk( x, y );
        RealType factor = -log( s ) / s * radius;
        x = x * factor;
        y = y * factor;
    }

private:
    Generator generator;

    inline IntegralType gen ( ) {
        static_assert( Generator::min() == 0, "RandomBase requires a generator with ::min() = 0" );

        return generator() & Generator::max();
    }
};


typedef RandomBase<mt19937> Random;

#endif // RANDOM_H
