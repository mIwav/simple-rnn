

#ifndef _INCLUDED_dataimporter_h
#define _INCLUDED_dataimporter_h

#include <boost/concept/requires.hpp>
#include <boost/range/concepts.hpp>
#include "named.hpp"
#include "helpers.hpp"
#include "seqbuffer.hpp"
#include "configfile.hpp"


#define SAVE(x) (save (x, #x))
#define DISPLAY(x) (display (x, #x))

struct Val
{
	virtual void print(ostream& out) const = 0;
	virtual bool load(istream& in, ostream& out = cout)
	{
		return false;
	}
	virtual ~Val(){}
};

static ostream& operator <<(ostream& out, const Val& v)
{	
	v.print(out);
	return out;
}

static istream& operator >>(istream& in, Val& v)
{	
	v.load(in);
	return in;
}

template <typename R> struct RangeVal: public Val
{
	//data
	R range;

	//functions
	RangeVal(const R& r):
		range(r)
	{}
	void print(ostream& out) const
	{
		out << boost::size(range) << " ";
		print_range(out, range);
	}
	bool load(istream& in, ostream& out = cout)
	{
		int size;
		if (in >> size && size == boost::size(range))
		{
			if (!(in >> range))
			{
				out << "ERROR unable to read from stream" << endl;
				return false;
			}
			return true;
		}
		else
		{
			out << "ERROR saved size " << size << " != current size " << boost::size(range) << endl;
			return false; 
		}
	}
};

template <typename T> struct ParamVal: public Val
{
	//data
	T& param;

	//functions
	ParamVal(T& p):
		param(p)
	{}
	void print(ostream& out) const
	{
		out << param;
	}
	bool load(istream& in, ostream& out = cout)
	{
		in >> param;
		return true;
	}
};

template <typename T> struct SeqBufferVal: public Val
{
	//data
	const SeqBuffer<T>& array;
	const vector<string>* labels;
	
	//functions
	SeqBufferVal(const SeqBuffer<T>& a, const vector<string>* labs = 0):
		array(a),
		labels(labs)
	{}
	void print(ostream& out) const
	{
		if (!array.empty())
		{
			if (labels)
			{
				out << "LABELS: " << *labels << endl;
//				print_range(out, *labels);
//				out << endl;
			}
			out << array;
		}
	}
};

// class DataImportHandler;
// class Val;
struct DataImporter;

typedef map<string, Val*>::const_iterator CONST_VAL_IT;
typedef pair<const string, Val*> PSPV;

struct DataImportHandler
{
	//data
	map<string, DataImporter*> dataImporters;
	
	//functions
	static DataImportHandler& instance()
	{
		static DataImportHandler d;
		return d;
	}
	void save(ostream& out) const;
	void load(ConfigFile& conf, ostream& out = cout);
	void display(const string& path) const;
};

static ostream& operator << (ostream& out, const DataImportHandler& de)
{
	de.save(out);
	return out;
}

struct DataImporter: public Named
{
	//data
	map<string, Val*> saveVals;
	map<string, Val*> displayVals;
	
	//functions
	DataImporter(const string& name):
		Named(name)
	{
 		DataImportHandler::instance().dataImporters[name] = this;
	}
	~DataImporter()
	{
		delete_map(displayVals);
		delete_map(saveVals);
	}
	void save(ostream& out) const
	{
		for (CONST_VAL_IT it = saveVals.begin(); it != saveVals.end(); ++it)
		{
			out << name << "_" << it->first << " " << *(it->second) << endl;
		}
	}
	bool load(ConfigFile& conf, ostream& out = cout)
	{
		BOOST_FOREACH(PSPV& val, saveVals)
		{
			string lookupName = name + "_" + val.first;
			string displayName = name + "." + val.first;
			if (verbose)
			{
				out << "loading "<< displayName << endl;
			}
			map<string, string>::iterator stringIt = conf.params.find(lookupName);
			if (stringIt == conf.params.end())
			{
				out << "WARNING: unable to find '" << displayName << "'" << endl;
			}
			else
			{
				stringstream ss;
				ss << stringIt->second;
				if (val.second->load(ss, out))
				{
					conf.params.erase(stringIt);
				}
				else
				{
					out << "WARNING: unable to load '" << displayName << "'" << endl;
					return false;
				}
			}
		}
		return true;
	}
	void delete_val(map<string, Val*>& vals, const string& name)
	{
		if (in(vals, name))
		{
			delete vals[name];
			vals.erase(name);
		}
	}
	template<typename T> void save(T& param, const string& name)
	{
		delete_val(saveVals, name);
		saveVals[name] = new ParamVal<T>(param);
	}
	template<typename R> void save_range(const R& range, const string& name)
	{
		delete_val(saveVals, name);
 		saveVals[name] = new RangeVal<R>(range);
	}
	template <typename T> void display(const SeqBuffer<T>& array, const string& name, const vector<string>* labels = 0)
	{
		delete_val(displayVals, name);
		displayVals[name] = new SeqBufferVal<T>(array, labels);
	}
};

static ostream& operator <<(ostream& out, const DataImporter& d)
{	
	d.save(out);
	return out;
}

static void save_data(ostream &out, const string& filename, ConfigFile& conf)
{		
	ofstream fout(filename.c_str());
	if (fout.is_open())
	{
		out << "saving to " << filename << endl;
		// conf.set_val<bool>("loadWeights", true);
		fout << conf << DataImportHandler::instance();
	}
	else
	{
		out << "WARNING unable to save data to file " << filename << endl;
	}
}

typedef pair<const string, DataImporter*> PSPDI;


#endif