

#ifndef _INCLUDED_outputlayer_h
#define _INCLUDED_outputlayer_h

#include <map>
#include "layer.hpp"

struct OutputLayer:public Layer
{
	OutputLayer(const string& name, size_t numSeqDims, size_t size):
		Layer(name, size, size)
	{
		display(this->inputActivations, "activations");
	}
};
#endif // !_INCLUDED_outputlayer_h
