
#ifndef _INCLUDED_named_h
#define _INCLUDED_named_h

#include <string>
#include <iostream>

using namespace std;

struct Named
{
	string name;

	Named(const string & n):
		name(n)
	{}
	virtual ~Named(){}
	void print(ostream& out = cout) const
	{
		out << "\"" << name << "\"";
	}
	
};

static ostream& operator << (ostream& out, const Named& n)
{
	n.print(out);
	return out;
}

#endif