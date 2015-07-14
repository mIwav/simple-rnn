

#ifndef _INCLUDED_layer_h
#define _INCLUDED_layer_h

#include "helpers.hpp"
#include "seqbuffer.hpp"

#include "named.hpp"
#include "dataimporthandler.hpp"

struct Layer: public DataImporter
{
	SeqBuffer<real_t> inputActivations;
	SeqBuffer<real_t> outputActivations;
	// SeqBuffer<real_t> inputErrors;
	// SeqBuffer<real_t> outputErrors;
	Layer* source;

	Layer(const string& name, size_t inputSize, size_t outputSize, Layer* src = 0):
		DataImporter(name),
		source(src)
	{
		inputActivations.reshape(list_of<size_t>(inputSize)(1));
		outputActivations.reshape(list_of<size_t>(outputSize)(1));
		assert(inputSize || outputSize);
	}
	virtual ~Layer(){}
	virtual void build()
	{
		assert(source);
	}

	virtual void start_sequence()
	{	
		assert(!in(source->output_seq_shape(), 0));
		// inputActivations.reshape(source->output_seq_shape(), 0);
		// outputActivations.reshape(source->output_seq_shape(), 0);
	}

	virtual void feed_forward(){}

	virtual size_t input_size() const
	{
		return inputActivations.seq_size();
	}
	virtual size_t output_size() const
	{
		return outputActivations.seq_size();
	}
	virtual const View<const size_t> output_seq_shape() const
	{
		return outputActivations.seq_shape();
	}
	virtual const View<real_t> out_acts()
	{
		return View<real_t>(&outputActivations.data.front(),&outputActivations.data.back()+1);
	}
	virtual const View<real_t> in_acts()
	{
		return View<real_t>(&inputActivations.data.front(),&inputActivations.data.back()+1);
	}

	virtual void print(ostream& out = cout) const
	{
		out << typeid(*this).name() << " "; 
		Named::print(out);
		if (input_size() == 0)
		{
			out << " size " << output_size();
		}
		else if (output_size() == 0 || input_size() == output_size())
		{
			out << " size " << input_size();
		}
		else
		{	
			out << " inputSize " << input_size()  << " outputSize " << output_size();
		}

		if (source)
		{
			out << " source \"" << source->name << "\"";
		}
	}
};

ostream& operator << (ostream& out, const Layer& l)
{
	l.print(out);
	return out;
}

struct FlatLayer: public Layer
{
	FlatLayer(const string& name, size_t size, Layer* src = 0):
		Layer(name, size, size, src)
	{}
};

#endif