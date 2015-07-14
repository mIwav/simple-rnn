#ifndef _INCLUDED_configfile_h  
#define _INCLUDED_configfile_h  

#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include "helpers.hpp"

using namespace std;

template<class T> static Vector<T> split(const string& original, char delim = ' ', size_t maxSplits = 0)
{
	Vector<T> vect;
	stringstream ss;
	ss << original;
	string s;
	while (delim == ' ' ? ss >> s : getline(ss, s, delim))
	{
		vect += read<T>(s);
		if (vect.size() == maxSplits-1)
		{
			delim = '\0';
		}
	}		
	return vect;
}

struct ConfigFile
{
	//data
	map<string, string> params;
	string filename;
	
	//functions
	ConfigFile(const string& fname, char readLineChar = '_'):
		filename(fname)
	{
		ifstream instream(filename.c_str());
		check(instream.is_open(), "could not open config file \"" + filename + "\"");
		string name;
		string val;
		while(instream >> name && instream >> val)
		{
			string line;
			getline(instream, line);
			if(name[0] != '#')
			{
				if (in(name, readLineChar) && line.size() > 1)
				{
					val += line;
				}
				params[name] = val;
			}
		}
	}
	bool has(const string& name)
	{
		return in(params, name);
	}
	template<class T> const T& set(const string& name, const T& val)
	{
		stringstream ss;
		ss << boolalpha << val;
		params[name] = ss.str();
		return val;
	}
	template<class T> T get(const string& name, const T& defaultVal)
	{
		MSSI it = params.find(name);
		if (it == params.end())
		{	
			set<T>(name, defaultVal);
			return defaultVal;
		}
		return get<T>(name);
	}
	template<class T> T get(const string& name) const
	{
		MSSCI it = params.find(name);
		if (it == params.end())
		{	
			cout << "param '" << name << "' not found in config file '" << filename << "', exiting" << endl;
			exit(0);
		}
		return read<T>(it->second);
	}
	template<class T> Vector<T> get_list(const string& name, const char delim = ',') const
	{
		Vector<T> vect;
		MSSCI it = params.find(name);
		if (it != params.end())
		{
			vect = split<T>(it->second, delim);
		}
		return vect;
	}
	template<class T> Vector<T> get_list(const string& name, const T& defaultVal, size_t length, const char delim = ',') const
	{
		Vector<T> vect = get_list<T>(name, delim);
		vect.resize(length, vect.size() == 1 ? vect.front() : defaultVal);
		return vect;
	}
	template<class T> vector<vector<T> > get_array(const string& name, const char delim1 = ';', const char delim2 = ',') const
	{
		vector<vector<T> > vect;
		MSSCI it = params.find(name);
		if (it != params.end())
		{
			Vector<string> lists = split<string>(it->second, delim1);
			for (VSCI it = lists.begin(); it != lists.end(); ++it)
			{
				vect.push_back(split<T>(*it, delim2));
			}
		}
		return vect;
	}
};

static ostream& operator << (ostream& out, const ConfigFile& conf)
{
	out << conf.params;
	return out;
}

#endif
