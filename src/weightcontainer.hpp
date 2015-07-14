
#ifndef _INCLUDED_weightcontainer_h
#define _INCLUDED_weightcontainer_h

#include <vector>
#include <numeric>
#include <algorithm>
#include <functional>
#include <map>
#include <math.h>
#include <boost/tuple/tuple.hpp>

#include "connection.hpp"
#include "random.hpp"

using namespace std;

typedef multimap<string, boost::tuple<string, string, int, int>>::iterator WC_CONN_IT;
typedef pair<string, boost::tuple<string, string, int, int> > WC_CONN_PAIR;

struct WeightContainer: public DataImporter
{
	// data
	vector<real_t> weights;
	multimap<string, boost::tuple<string, string, int, int> > connections;
	WeightContainer():
		DataImporter("weightContainer")
	{
	}
	static WeightContainer& instance()
	{
		static WeightContainer wc;
		return wc;
	}
	void link_layers(const string& fromName, const string& toName, const string& connName = "", int paramBegin = -1, int paramEnd = -1)
	{
		connections.insert(make_pair(toName, boost::make_tuple(fromName, connName, paramBegin, paramEnd)));
	}
	pair<size_t, size_t> new_parameters(size_t numParams, const string& fromName, const string& toName, const string& connName)
	{
		size_t begin = weights.size();
		weights.resize(weights.size() + numParams);
		size_t end = weights.size();
		cout << "conn fr "<< fromName << " to "<< toName << " frsz: " << begin << " tsz: " << end << endl;
		link_layers(fromName, toName, connName, begin, end);
		return make_pair(begin, end);
	}
	View<real_t> get_weights(pair<int, int> range)
	{
		return View<real_t>(&weights[range.first], &weights[range.second-1]);
	}

	int randomise(real_t range)
	{
		int numRandWts = 0;
		BOOST_FOREACH(real_t& w, weights)
		{
			if (w == infinity)
			{
				w = Random::normal() * range;
				++numRandWts;
			}
		}
		return numRandWts;
	}
	void save_by_conns(vector<real_t>& container, const string& nam)
	{
		BOOST_FOREACH(const WC_CONN_PAIR& p, connections)
		{
			VDI begin = container.begin() + p.second.get<2>();
			VDI end = container.begin() + p.second.get<3>();
			if (begin != end)
			{
				save_range(make_pair(begin, end), p.second.get<1>() + "_" + nam);
			}
		}
	}
	void build()
	{
		fill(weights, infinity);
		save_by_conns(weights, "weights");
	}
};
#endif
