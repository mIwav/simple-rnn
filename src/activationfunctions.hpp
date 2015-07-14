

#ifndef _INCLUDED_activationfunctions_h
#define _INCLUDED_activationfunctions_h

#include <numeric>
#include <limits>

static const real_t expLimit = log(numeric_limits<real_t>::max());
static const real_t negExpLimit = log(numeric_limits<real_t>::min());

struct Logistic
{
	static real_t fn(real_t x)
	{
		if (x < -expLimit)
			{
				return 1;
			}
			else if (x > expLimit)
			{
				return 0;
			}
			return 1.f / (1.f + exp(-x));
			}
	static real_t deriv(real_t y)
	{
		return y*(1.f-y); 
	}
};

struct Maxmin1
{
	static real_t fn(real_t x)
	{
		return (2 * Logistic::fn(x)) - 1;
	}
	static real_t deriv(real_t y)
	{
		if (y==-1 || y==1)
		{
			return 0;
		}
		return (1.f - (y * y)) / 2.f;
	}
};

struct Tanh
{
	static real_t fn(real_t x)
	{
		return Maxmin1::fn(2*x);
	}
	static real_t deriv(real_t y)
	{
		return 1.f - (y *  y); 
	}
};

#endif