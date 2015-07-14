

#include "dataimporthandler.hpp"

void DataImportHandler::save(ostream& out) const
{
	BOOST_FOREACH(const PSPDI& imp, dataImporters)
	{
		out << *(imp.second);
	}
}
void DataImportHandler::load(ConfigFile& conf, ostream& out)
{
	BOOST_FOREACH(PSPDI& imp, dataImporters)
	{
		if(!imp.second->load(conf, out))
		{
			out << " for '" << imp.first << "' in config file " << conf.filename << ", exiting" << endl;
			exit(0);
		}
	}
}
void DataImportHandler::display(const string& path) const
{
	BOOST_FOREACH(const PSPDI& imp, dataImporters)
	{
		BOOST_FOREACH(const PSPV& val, imp.second->displayVals)
		{			
			string filename = path + imp.first + "_" + val.first;
			ofstream out (filename.c_str());
			check(out.is_open(), "couldn't open display file " + filename + " for writing");
			out << *(val.second);
		}
	}
}
