

#ifndef _INCLUDED_connection_h
#define _INCLUDED_connection_h

#include <iostream>

#include "named.hpp"
#include "layer.hpp"

struct Connection: public Named
{
	//data
	Layer* from;
	Layer* to;
	
	Connection(const string& name, Layer* f, Layer* t):
		Named(name),
		from(f),
		to(t)
	{
		assert(from);
		assert(to);
		assert(from->output_size());
		assert(to->input_size());		
	}
	virtual ~Connection(){}
	virtual size_t num_weights() const {return 0;}
	virtual void feed_forward(){}
	virtual void print(ostream& out) const{}
};

static ostream& operator <<(ostream& out, const Connection& c)
{
	c.print(out);
	return out;
}

#endif