

#ifndef _INCLUDED_multilayernet_h
#define _INCLUDED_multilayernet_h

#include "helpers.hpp"
#include "rnn.hpp"
#include "configfile.hpp"

struct MultilayerNet: public Rnn
{
	MultilayerNet(ostream &out, ConfigFile &conf):
		Rnn(out, conf)
	{
		vector<int> hiddenSizes = conf.get_list<int>("hiddenSize");
		assert(hiddenSizes.size());
		vector<bool> recurrent = conf.get_list<bool>("recurrent", true, hiddenSizes.size());
		Layer* input = this->get_input_layer();
		for(size_t i = 0; i < hiddenSizes.size(); ++i)
		{
			Layer* layer = new NeuronLayer<Tanh>("hidden_layer_" + str(i), hiddenSizes.at(i));
			this->add_layer(layer, false, true);
		}
		this->connect_layers(input, this->hiddenLayers.at(0));
		this->connect_hidden_layers();

		string outputName = "output";
		Layer* output = this->get_output_layer();
		this->connect_layers(hiddenLayers.back(), output);
	}
};

#endif
