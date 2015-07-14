
#ifndef _INCLUDED_softmaxlayer_h
#define _INCLUDED_softmaxlayer_h


#include "outputlayer.hpp"
#include "log.hpp"

struct SoftmaxLayer: public FlatLayer
{
	Vector<string> targetLabels;
	SeqBuffer<Log<real_t> > logActivations;
	SeqBuffer<Log<real_t> > unnormedlogActivations;
	SeqBuffer<real_t> unnormedActivations;

	SoftmaxLayer(const string& name, const Vector<string>& labs):
		FlatLayer(name, labs.size()),
		targetLabels(labs)
	{
		logActivations.reshape(list_of<size_t>(this->output_size())(1));
		unnormedlogActivations.reshape(list_of<size_t>(this->output_size())(1));
		unnormedActivations.reshape(list_of<size_t>(this->output_size())(1));
	}

	void start_sequence()
	{
		/*Layer::start_sequence();
		logActivations.reshape(list_of<size_t>(this->output_size())(1));
		unnormedlogActivations.reshape(list_of<size_t>(this->output_size())(1));
		unnormedActivations.reshape(list_of<size_t>(this->output_size())(1));*/
	}
	void feed_forward()
	{	
		//transform to log scale and centre inputs on 0 for safer exponentiation
		View<Log<real_t> > unnormedLogActs = View<Log<real_t>>(&unnormedlogActivations.data.front(), &unnormedlogActivations.data.back()+1);
		real_t offset = pair_mean(minmax(this->in_acts()));
		BOOST_FOREACH(TDL t, zip(this->in_acts(), unnormedLogActs))
		{
			t.get<1>() = Log<real_t>(t.get<0>() - offset, true);
		}

		//apply exponential
		View<real_t> unnormedActs = View<real_t>(&unnormedActivations.data.front(), &unnormedActivations.data.back()+1);
		transform(unnormedLogActs, unnormedActs, mem_fun_ref(&Log<real_t>::exp));
		
		//normalise
		real_t Z = sum(unnormedActs);
		range_divide_val(this->out_acts(), unnormedActs, Z);
		range_divide_val(View<Log<real_t>>(&logActivations.data.front(), &logActivations.data.back()+1), unnormedLogActs, Log<real_t>(Z));
	}
};
#endif