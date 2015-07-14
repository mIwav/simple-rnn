

#ifndef _INCLUDED_neuronlayer_h
#define _INCLUDED_neuronlayer_h

#include "layer.hpp"
#include "helpers.hpp"

template <class F> struct NeuronLayer: public Layer
{
	NeuronLayer(const string& name, size_t size):
		Layer(name, size, size)
	{
		display(this->inputActivations, "inputActivations");
		display(this->outputActivations, "outputActivations");
	}
	~NeuronLayer(){}
	void feed_forward()
	{
		transform(this->in_acts(), this->out_acts(), F::fn);
	}
};
#endif