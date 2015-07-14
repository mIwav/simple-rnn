

#ifndef _INCLUDED_rnn_h
#define _INCLUDED_rnn_h

#include "helpers.hpp"
#include "layer.hpp"
#include "outputlayer.hpp"
#include "neuronlayer.hpp"
#include "fullconnection.hpp"
#include "inputlayer.hpp"
#include "softmaxlayer.hpp"
#include "activationfunctions.hpp"
#include "configfile.hpp"
//#include "datasequence.hpp"
#include "seqbuffer.hpp"

typedef pair<const Layer*, Connection*> PLC;
typedef multimap<const Layer*, Connection*>::iterator CONN_IT ;
typedef multimap<const Layer*, Connection*>::const_iterator CONST_CONN_IT;

struct Rnn
{
	ostream& out;
	multimap<const Layer*, Connection*> connections;
	vector<Layer*> hiddenLayers;
	InputLayer* inputLayer;
	SoftmaxLayer* outputLayer;
	vector<Layer*> recurrentLayers;

	Rnn(ostream& o, ConfigFile& conf):
		out(o),
		inputLayer(new InputLayer("input", conf.get<int>("inDims", 0))),
		outputLayer(new SoftmaxLayer("output", conf.get_list<string>("outLabels")))
	{
	}
	virtual ~Rnn()
	{
		delete inputLayer;
		delete outputLayer;
		delete_map(connections);
		delete_range(hiddenLayers);
	}
	Layer *get_input_layer() const
	{
		return (Layer*) inputLayer;
	}
	Layer *get_output_layer() const
	{
		return (Layer*) outputLayer;
	}
	Connection* add_connection(Connection* conn)
	{
		connections.insert(make_pair(conn->to, conn));
		return conn;
	}

	/*FullConnection* add_bias(Layer* layer)
	{
		return connect_layers(&bias, layer);
	}*/

	FullConnection* connect_layers(Layer* from, Layer* to)
	{
		FullConnection* conn = new FullConnection(from, to);
		add_connection(conn);
		return conn;
	}
	bool is_recurrent(const Layer* layer) const
 	{
		BOOST_FOREACH (const PLC& c, connections.equal_range(layer))
 		{
			if (c.second->from == c.second->to)
			{
				return true;
			}
 		}
		return false;
 	}
	void make_layer_recurrent(Layer* layer)
	{
		connect_layers(layer, layer);
	}

	Layer* add_layer(Layer* layer, bool addBias = false, bool recurrent = false)
	{
		hiddenLayers += layer;
		
		/*if (addBias)
		{
			add_bias(layer);
		}*/
		if (recurrent)
		{
			make_layer_recurrent(layer);
		}
					
		return layer;
	}

	Layer* add_layer(const string& type, const string& name, int size, 
					 bool addBias = false, bool recurrent = false)
	{
		Layer* layer = nullptr;
		if (type == "tanh")
		{
			layer = new NeuronLayer<Tanh>(name, size);
		}
		else
		{
			check(false, "layer type " + type + " unknown");
		}
		return add_layer(layer, addBias, recurrent);
	}

	void connect_hidden_layers()
	{
		for (size_t i = 0; i < hiddenLayers.size()-1; ++i)
		{
			connect_layers(hiddenLayers.at(i),hiddenLayers.at(i+1));
		}
	}

	virtual void build()
	{
		/*BOOST_FOREACH(Layer* dest, hiddenLayers)
		{
			if (is_mirror(dest))
			{
				vector<int> sourceDirs(dest->directions.size());
				BOOST_FOREACH(TIBI t, zip(sourceDirs, symmetry, dest->directions))
				{
					t.get<0>() = (((t.get<1>() > 0) || t.get<2>()) ? 1 : -1);
				}
				BOOST_FOREACH(Layer* src, v)
				{
					if (src->directions == sourceDirs)
					{
						copy_connections(src, dest, true);
						break;
					}
				}
			}				
		}*/
		recurrentLayers.clear();
		BOOST_FOREACH(Layer* l, hiddenLayers)
		{
			l->build();
			if (is_recurrent(l))
			{
				recurrentLayers += l;
			}
		}
	}

	virtual void feed_forward(const SeqBuffer<real_t>& seq)
	{
		check(seq.size(), "empty inputs in sequence\n" + str(seq));
		inputLayer->copy_inputs(seq);
		BOOST_FOREACH(Layer* layer, hiddenLayers)
		{
			feed_forward_layer(layer);
		}
		feed_forward_layer(outputLayer);
	}

	void feed_forward_layer(Layer* layer)
	{
		layer->start_sequence();
		pair<CONN_IT, CONN_IT> connRange = connections.equal_range(layer);
		BOOST_FOREACH (PLC c, connRange)
		{
			c.second->feed_forward();
		}			
		layer->feed_forward();
	}

	virtual void print(ostream& out = cout) const
	{
		out << typeid(*this).name() << endl;
		prt_line(out);
		out << hiddenLayers.size() + 2 << " layers:" << endl;
		out << (Layer&)*(inputLayer) << endl;
		BOOST_FOREACH(const Layer* layer, hiddenLayers)
		{
			if (is_recurrent(layer))
			{
				out << "(R) "; 
			}
			out << *layer << endl;
		}
		out << (Layer&)*outputLayer << endl;
		prt_line(out);
		out << connections.size() << " connections:" << endl;
		for (CONST_CONN_IT it = connections.begin(); it != connections.end(); ++it)
		{
			out << *it->second << endl;
		}
		prt_line(out);
	}
};

ostream& operator << (ostream& out, const Rnn& net)
{
	net.print(out);
	return out;
}

#endif