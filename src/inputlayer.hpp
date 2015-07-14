

#ifndef _INCLUDED_InputLayer_h  
#define _INCLUDED_InputLayer_h  

#include "Layer.hpp"

struct InputLayer: public Layer
{
	//functions
	InputLayer(const string& name, size_t size):
		Layer(name, 0, size)
	{
		display(this->outputActivations, "activations");
		// display(this->outputErrors, "errors");
	}
	~InputLayer(){}

	void start_sequence()
	{
		// this->outputActivations.reshape(this->output_seq_shape(), 0);
	}

	template<typename T> void copy_inputs(const SeqBuffer<T>& inputs)
	{
		assert(inputs.seq_size() == this->output_size());
		this->outputActivations = inputs;
		// this->outputErrors.reshape(this->outputActivations, 0);
	}
};

#endif