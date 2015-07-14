// rnntest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "helpers.hpp"
#include "configfile.hpp"
#include "rnn.hpp"
#include "multilayernet.hpp"
#include "dataimporthandler.hpp"

#include <iostream>

extern bool verbose;

int main(int argc, char* argv[])
{
	string configFilename;
	
	if (argc != 2 && argc != 3)
	{
		cout << "usage rnnlib config_file" << endl;
		exit(0);
	}
	else
	{
		configFilename = argv[1];
	}
	ConfigFile conf(configFilename);
	string task = conf.get<string>("task");
	bool display = conf.get<bool>("display", false);
	string dumpPath = "";
	string logname = "";
	if (display)
	{
		dumpPath = conf.get<string>("dumpPath");
		logname = dumpPath + "log";
	}

	ostream &out = std::cout;

	Rnn *net;
	net = new MultilayerNet(out, conf);
	out << endl << "network:" << endl;
	PRINT(task, out);
	out << *net;

	//build weight container after net is created
	WeightContainer& wc = WeightContainer::instance();
	wc.build();
	int numWeights = WeightContainer::instance().weights.size();
	out << numWeights << " weights" << endl << endl;
	
	//build the network after the weight container
	net->build();

	if (conf.get<bool>("loadWeights", false))
	{
		out << "loading weight data from " << conf.filename << endl;
		DataImportHandler::instance().load(conf, out);
	}

	real_t initWeightRange = conf.get<real_t>("initWeightRange", static_cast<real_t>(0.1));
	int numRandWts = wc.randomise(initWeightRange);
	if (numRandWts)
	{
		out << numRandWts << " uninitialised weights randomised uniformly in [-" << initWeightRange << "," << initWeightRange << "]" << endl;
	}

	DataImportHandler::instance().display(dumpPath);

	//save_data(out, "try_out.save", conf);

	vector<size_t> shape;
	shape.push_back(4);
	shape.push_back(1);
	SeqBuffer<real_t> seq;
	seq.reshape(shape, Random::normal(0.1f));
	
	int i = 0;
	FOR(i, 4)
	{
		seq.data[i] = Random::normal(0.1f);
	}

	net->feed_forward(seq);
	out << net->outputLayer->inputActivations;
	out << net->outputLayer->outputActivations;

	delete net;

	return 0;
}

