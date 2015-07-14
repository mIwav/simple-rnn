#ifndef _INCLUDED_random_h  
#define _INCLUDED_random_h  

#include "helpers.hpp"

namespace Random
{ 
	unsigned int set_seed(unsigned int seed = 17649);
	real_t normal();				//normal distribution with mean 0 std dev 1
	real_t normal(real_t dev, real_t mean = 0);	//normal distribution with user defined mean, dev
	real_t uniform(real_t range);			//uniform real in (-range, range)
	real_t uniform();				//uniform real in (0,1)
};

#endif