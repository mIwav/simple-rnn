
#ifndef _INCLUDED_datasequence_h
#define _INCLUDED_datasequence_h

#include <vector>
#include <string>

#include "seqbuffer.hpp"

struct DataSequence
{
	//data
	SeqBuffer<real_t> inputs;
	SeqBuffer<int> inputClasses;
	SeqBuffer<real_t> targetPatterns;
	SeqBuffer<int> targetClasses;
	SeqBuffer<real_t> importance;
	vector<int> targetLabelSeq;
	vector<string> targetWordSeq;
	string tag;
	
	//functions
	DataSequence(const DataSequence& ds):
		inputs(ds.inputs),
		inputClasses(ds.inputClasses),
		targetPatterns(ds.targetPatterns),
		targetClasses(ds.targetClasses),
		importance(ds.importance),
		targetLabelSeq(ds.targetLabelSeq),
		tag(ds.tag)
	{
	}
	DataSequence(size_t inputDepth = 0, size_t targetPattDepth = 0):
		inputs(inputDepth),
		inputClasses(0),
		targetPatterns(targetPattDepth),
		targetClasses(0),
		importance(0)
	{
	}
	void print(ostream& out, vector<string>* targetLabels = 0, vector<string>* inputLabels = 0) const
	{
		PRINT(tag, out);
		out << "input shape = (" << inputs.shape << ")" << endl;
		out << "timesteps = " << inputs.seq_size() << endl;
		if (targetLabelSeq.size() && targetLabels)
		{
			out << "target label sequence:" << endl;
			// out << label_seq_to_str(this->targetLabelSeq, *targetLabels) << endl;
		}
		if (targetPatterns.size())
		{
			out << "target shape = (" << targetPatterns.shape << ")" << endl;
		}
		if (verbose)
		{
			/*if(targetClasses.size() && targetLabels)
			{
				out << label_seq_to_str(this->targetClasses.data, *targetLabels) << endl;
			}*/
			/*if(inputClasses.size() && inputLabels)
			{
				out << label_seq_to_str(this->inputClasses.data, *inputLabels) << endl;
			}*/
		}
	}
};

static ostream& operator <<(ostream& out, const DataSequence& seq)
{
	seq.print(out);
	return out;
}

#endif