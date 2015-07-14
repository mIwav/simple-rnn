
#ifndef _INCLUDED_fullconnection_h
#define _INCLUDED_fullconnection_h

#include <vector>
#include <numeric>
#include <iostream>
#include <sstream>

#include "layer.hpp"
#include "helpers.hpp"
#include "connection.hpp"
#include "weightcontainer.hpp"

struct FullConnection: public Connection
{
	// data
	FullConnection* source;
	pair<size_t, size_t> paramRange;

	// functions
	FullConnection(Layer* f, Layer* t, FullConnection* s = 0):
		Connection(make_name(f,t), f, t),
		source(s),
		// now here is where the real connection magic happens
		paramRange(source ? source->paramRange: 
				WeightContainer::instance().new_parameters(this->from->output_size() * this->to->input_size(),
		this->from->name, this->to->name, name))
	{
		if(source)
		{
			WeightContainer::instance().link_layers(this->from->name, this->to->name, this->name, paramRange.first, paramRange.second);
		}
		
		assert(num_weights() == (this->from->output_size() * this->to->input_size()));
		if (this->from->name != "bias" && this->from != this->to && !this->to->source)
		{
			this->to->source = this->from;
		}
	}

	~FullConnection(){}

	static const string& make_name(Layer* f, Layer* t)
	{
		static string name;
		name = f->name + "_to_" + t->name;
		return name;
	}
	const View<real_t> weights()
	{
		return WeightContainer::instance().get_weights(paramRange);
	}
	size_t num_weights() const
	{
		return difference(paramRange);
	}

	void feed_forward()
	{
		dot(this->from->out_acts(), weights().begin(), this->to->in_acts());
	}

	void print(ostream& out) const
	{
		Named::print(out);
		out << " (" << num_weights() << " wts";
		if (source)
		{
			out << " shared with " << source->name;
		}
		out << ")";
	}
};
#endif
