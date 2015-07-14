#ifndef _INCLUDED_helpers_h  
#define _INCLUDED_helpers_h  

// #include <boost/date_time.hpp>
// #include <boost/date_time/local_time/local_time.hpp>
#include <boost/array.hpp>
#include <boost/timer.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/range.hpp>
#include <boost/iterator/counting_iterator.hpp>
#include <boost/iterator/zip_iterator.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/minmax.hpp>
#include <boost/algorithm/minmax_element.hpp>
#include <boost/bimap.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/foreach.hpp>
#include <boost/range/irange.hpp>
#include <boost/math/special_functions/round.hpp>
#include <math.h>
#include <numeric>
#include <utility>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <list>
#include <set>
#include <algorithm>
#include <iterator>
#include <map>
#include <assert.h>
#include "log.hpp"

using namespace std;
using namespace boost;
using namespace boost::assign;
// using namespace boost::posix_time;
// using namespace boost::gregorian;

#define check(condition, str)  if(!(condition)) {cout << "ERRROR: " << (str) << endl; assert((condition));}
#define PRINT(x, o) ((o) << boolalpha << #x " = " << (x) << endl)
#define FOR(i, n) for(int (i) = 0; (i) < (n); ++(i))

typedef float real_t;

static const real_t real_tMax = numeric_limits<real_t>::max();
static const real_t real_tMin = numeric_limits<real_t>::min();
static const real_t infinity = numeric_limits<real_t>::infinity();
static bool runningGradTest = false;
static bool verbose = false;
static ostream& COUT = cout;

typedef vector<size_t>::const_iterator VSTCI;
typedef vector<real_t>::iterator VDI;
typedef vector<real_t>::const_iterator VDCI;
typedef vector<real_t>::reverse_iterator VDRI;
typedef string::iterator SI;
typedef string::const_iterator SCI;
typedef vector<int>::iterator VII;
typedef vector<string>::iterator VSI;
typedef vector<string>::const_iterator VSCI;
typedef vector<int>::reverse_iterator VIRI;
typedef vector<vector<int> >::reverse_iterator VVIRI;
typedef vector<int>::const_iterator VICI;
typedef vector<bool>::iterator VBI;
typedef vector<real_t>::iterator VFI;
typedef vector<real_t>::const_iterator VFCI;
typedef vector<vector<real_t> >::iterator VVDI;
typedef vector<vector<real_t> >::const_iterator VVDCI;
typedef vector<vector<int> >::iterator VVII;
typedef vector<vector<int> >::const_iterator VVICI;
typedef vector<unsigned int>::iterator VUII;
typedef vector<vector<real_t> >::iterator VVFI;
typedef map<string, string>::iterator MSSI;
typedef map<string, string>::const_iterator MSSCI;
typedef map<string, real_t>::iterator MSDI;
typedef map<string, real_t>::const_iterator MSDCI;
typedef map<string, pair<int,real_t> >::iterator MSPIDI;
typedef map<string, pair<int,real_t> >::const_iterator MSPIDCI;
typedef vector< map<string, pair<int,real_t> > >::const_iterator VMSDCI;
typedef vector<map<string, pair<int,real_t> > >::iterator VMSDI;
typedef vector<map<string, pair<int,real_t> > >::reverse_iterator VMSDRI;
typedef map<string, int>::iterator MSII;
typedef map<string, int>::const_iterator MSICI;
typedef map<int, int>::iterator MIII;
typedef map<int, int>::const_iterator MIICI;
typedef vector<vector<int> >::const_reverse_iterator VVIRCI;
typedef vector<int>::const_reverse_iterator VIRCI;
typedef vector<const real_t*>::const_iterator VPCFCI;
typedef vector<const real_t*>::iterator VPCFI;
typedef vector<const real_t*>::const_reverse_iterator VPCFCRI;
typedef vector<bool>::const_iterator VBCI;
typedef vector<bool>::iterator VBI;
typedef map <string, pair<real_t, int> >::iterator MCSPDII;
typedef map <string, pair<real_t, int> >::const_iterator MCSPDICI;
typedef bimap<int, string>::left_const_iterator BMISLCI;
typedef bimap<int, string>::right_const_iterator BMISRCI;
typedef bimap<int, string>::relation BMISR;
typedef pair<string, real_t> PSD;
typedef pair<int, int> PII;
typedef pair<const string, real_t> PCSD;
typedef pair<string, int> PSI;
typedef pair<string, string> PSS;
typedef const boost::tuple<real_t&, real_t&, real_t&, real_t&>& TDDDD;
typedef const boost::tuple<real_t&, real_t&, real_t&, real_t&, real_t&>& TDDDDD;
typedef const boost::tuple<real_t&, real_t&, real_t&>& TDDD;
typedef const boost::tuple<real_t&, real_t&, int&>& TDDI;
typedef const boost::tuple<real_t&, real_t&, real_t&>& TDDF;
typedef const boost::tuple<real_t&, real_t&, real_t>& TDDCF;
typedef const boost::tuple<real_t&, real_t&>& TDD;
typedef const boost::tuple<string, int>& TSI;
typedef const boost::tuple<int, int>& TII;
typedef const boost::tuple<int, set<int>&>& TISETI;
typedef const boost::tuple<real_t, Log<real_t>& >& TDL;
typedef const boost::tuple<real_t&, Log<real_t>, Log<real_t> >& TDLL;
typedef Log<real_t> prob_t;

// casting
template<class T> static string str(const T& t)
{
	stringstream ss;
	ss << t;
	return ss.str();
//	return lexical_cast<string>(t);
}

template<class T> T read(const string& data)
{
	T val;
	stringstream ss;
	ss << boolalpha << data;
	check(ss >> val, "cannot read string '" + data + "' into variable with type '" + typeid(T).name() + "'");
	return val;
}



template <class R, class T> static typename range_iterator<R>::type find(R& r, const T& t)
{
return find(boost::begin(r), boost::end(r), t);
}

//template <class R, class T> static bool in(const R& r, const T& t)
//{
//	// return find(boost::begin(r), boost::end(r), t) != boost::end(r);
//	return find(r, t) != boost::end(r);
//}

// vector operations
template<class R1, class R2> static void copy(const R1& source, R2& dest)
{
	assert(boost::size(dest) >= boost::size(source));
	copy(boost::begin(source), boost::end(source), boost::begin(dest));
}

template<class R, class T> void vector_assign(const R& r, vector<T>& v)
{
	v.resize(boost::size(r));
	copy(r, v);
}

// pair operations

template<class T1, class T2> static ostream& operator << (ostream& out, const pair<T1, T2>& p)
{
	out << p.first << " " << p.second;
	return out;
}

template<class T1, class T2> static bool operator == (const pair<T1, T2>& a, const pair<T1, T2>& b)
{
	return(a.first == b.first &&
	a.second == b.second);
}

template<class T1, class T2> static void operator+= (pair<T1, T2>& a, const pair<T1, T2>& b)
{
	a.first += b.first;
	a.second += b.second;
}
template<class T1, class T2, class T3> static pair<T1, T2> operator+ (const pair<T1, T2>& a, const T3& b)
{
	return make_pair(a.first + b, a.second + b);
}

template<class T1, class T2> static real_t pair_product(const pair<T1, T2>& p)
{
	return (real_t)(p.first * p.second);
}
template<class T1, class T2> static real_t pair_sum(const pair<T1, T2>& p)
{
	return (real_t)(p.first + p.second);
}
template<class T1, class T2> static real_t pair_mean(const pair<T1, T2>& p)
{
	return pair_sum(p)/static_cast<real_t>(2.0);
}
template <class T1, class T2> static size_t difference(const pair<T1,T2>& p)
{
	return p.second - p.first;
}

// map operations

template<class T1, class T2> static ostream& operator << (ostream& out, const map<T1, T2>& m)
{
	for (typename map<T1, T2>::const_iterator it = m.begin(); it != m.end(); ++it)
	{
		out << *it << endl; 
	}
	return out;
}

template<class T1, class T2> static ostream& operator << (ostream& out, const map<T1, T2*>& m)
{
	for (typename map<T1, T2*>::const_iterator it = m.begin(); it != m.end(); ++it)
	{
		out << it->first << " " << *(it->second) << endl; 
	}
	return out;
}

template<class T1, class T2> static bool in (const map<T1, T2>& a, const T1& b)
{
	return (a.find(b) != a.end());
}

template<class R> void delete_map(R& r)
{
	for (typename range_iterator<R>::type it = boost::begin(r); it != boost::end(r); ++it)
	{
		delete it->second;
	}
}

static bool in(const string& str, const string& search)
{
	return (str.find(search) != string::npos);
}

static bool in(const string& str, const char* search)
{
	return in(str, string(search));
}

template<class R> static void fill (R& r, const typename boost::range_value<R>::type& v)
{
	fill(boost::begin(r), boost::end(r), v);
}

template <class R> static typename range_value<R>::type product(const R& r)
{
	return accumulate(boost::begin(r), boost::end(r), (typename range_value<R>::type)1, multiplies<typename range_value<R>::type>());
}

template <class R> static typename range_value<R>::type sum(const R& r)
{
	return accumulate(boost::begin(r), boost::end(r), (typename range_value<R>::type)0);
}
// generic range operations
template<class R1, class R2> static size_t range_min_size (const R1& a, const R2& b)
{
	return min(boost::size(a), boost::size(b));
}

template<class R1, class R2, class R3> static size_t range_min_size (const R1& a, const R2& b, const R3& c)
{
	return min(min(boost::size(a), boost::size(b)), boost::size(c));
}

template<class R1, class R2, class R3, class R4> static size_t range_min_size (const R1& a, const R2& b, const R3& c, const R4& d)
{
	return min(min(min(boost::size(a), boost::size(b)), boost::size(c)), boost::size(d));
}

template<class R1, class R2, class R3, class R4, class R5> static size_t range_min_size (const R1& a, const R2& b, const R3& c, const R4& d, const R5& e)
{
	return min(min(min(min(boost::size(a), boost::size(b)), boost::size(c)), boost::size(d)), boost::size(e));
}

template <class R1, class R2> static pair<zip_iterator<boost::tuple<typename range_iterator<R1>::type, typename range_iterator<R2>::type> >,
											zip_iterator<boost::tuple<typename range_iterator<R1>::type, typename range_iterator<R2>::type> > > 
zip(R1& r1, R2& r2)
{
	size_t size = range_min_size(r1, r2);
  return make_pair(make_zip_iterator(boost::make_tuple(boost::begin(r1), boost::begin(r2))),
    make_zip_iterator(boost::make_tuple(boost::end(r1) - (boost::size(r1) - size), boost::end(r2) - (boost::size(r2) - size))));
}

template<class R> void delete_range(R& r)
{
	for (typename range_iterator<R>::type it = boost::begin(r); it != boost::end(r); ++it)
	{
		delete *it;
	}
}

template <class R, class T> static bool in(const R& r, const T& t)
{
	return find(boost::begin(r), boost::end(r), t) != boost::end(r);
}

template <class R1, class R2, class F> static typename range_iterator<R2>::type transform(const R1& r1, R2& r2, const F& f)
{
	return transform(boost::begin(r1), boost::end(r1), boost::begin(r2), f);
}

// add

template<class R1, class R2, class R3> static R1& range_plus(R1& a, const R2& b, const R3& c)
{
	transform(boost::begin(b), boost::end(b), boost::begin(c), boost::begin(a), plus<typename boost::range_value<R1>::type>());
	return a;
}

//divide
template<class R1, class R2> static void range_divide_val(R1& a, const R2& b, const typename boost::range_value<R1>::type& c)
{
	transform(boost::begin(b), boost::end(b), boost::begin(a), bind2nd(divides<typename boost::range_value<R1>::type>(), c));
}
template<class R> static void range_divide_val(R& a, const typename boost::range_value<R>::type& b)
{
	range_divide_val(a, a, b);
}

template<class _Ty> struct round: public unary_function<_Ty, _Ty>
{	// functor for unary operator round to int
	int operator()(const _Ty& _Left) const
	{	// apply operator round to operand
		return (boost::math::iround(_Left));
	}
};

template<class R1, class R2> static R2& range_roundi(R1 &a, R2 &b)
{
	transform(boost::begin(a), boost::end(a), boost::begin(b), round<typename boost::range_value<R1>::type>());
	return b;
}

// matrix type multiplications

#define OP_TRACKING

#ifdef OP_TRACKING
static unsigned long long matrixOps = 0;
#endif

static void dot(const real_t *inBegin, const real_t *inEnd, const real_t *M, real_t *out, real_t *outEnd)
{
#ifdef OP_TRACKING
	const real_t* mStart = M;
#endif
	for (;out != outEnd; ++out)
	{
		real_t sum = 0;
		for (const real_t *in = inBegin; in != inEnd; ++in, ++M)
		{ 
			sum += *M * (*in);
		}
		*out += sum; 
	}
#ifdef OP_TRACKING
	matrixOps += M - mStart;
#endif
}

template<class R> static void dot(const R& a, const real_t *M, const R& b)
{
	dot(boost::begin(a), boost::end(a), M, boost::begin(b), boost::end(b));
}

template <class R> pair<typename range_value<R>::type, typename range_value<R>::type> minmax(const R& r)
{
	pair<typename range_const_iterator<R>::type, typename range_const_iterator<R>::type> p = boost::minmax_element(boost::begin(r), boost::end(r));
	return make_pair(*p.first, *p.second); 
}

template<class T> static assign_detail::generic_list<T> empty_list_of()
{
	return assign_detail::generic_list<T>();
}

template<class R1, class R2> static typename range_value<R1>::type inner_product(const R1& a, const R2& b, typename range_value<R1>::type c = 0)
{
	return inner_product(boost::begin(a), boost::end(a), boost::begin(b), c);
}

template <class C, class Tr, class R> static void print_range(basic_ostream<C, Tr>& out, const R& r, const basic_string<C, Tr>& delim = " ")
{
	typename range_const_iterator<R>::type b = boost::begin(r); 
	typename range_const_iterator<R>::type e = boost::end(r);
	if (b != e) 
	{ 
		out << *b;
		while (++b != e) 
		{
			out << delim << *b; 
		}
	}
}

template <class C, class Tr, class R> static void print_range(basic_ostream<C, Tr>& out, const R& r, const char delim)
{
	print_range(out, r, str(delim));
}

template <class C, class Tr, class R> static basic_ostream<C, Tr>& operator <<(basic_ostream<C, Tr>& out, const R& r)
{
	print_range(out, r);
	return out;
}

static void prt_line(ostream& out = cout)
{
	out << "------------------------------" << endl;
}

template <class T1, class T2> static integer_range<T2> span(T1 t1, T2 t2)
{
    return irange<T2>((t1 < t2 ? static_cast<T2>(t1) : (t2)), t2);
	//return make_pair(counting_iterator<T2>(t1 < t2 ? static_cast<T2>(t1) : t2), counting_iterator<T2>(t2));
}

template <class T> static integer_range<T> span(const T &t)
{
	return span(static_cast<T>(0), t);
    //	return make_pair(counting_iterator<T>(0), counting_iterator<T>(t));
}

template <class C, class Tr, class R> static basic_istream<C, Tr>& operator >>(basic_istream<C, Tr>& in, R& r)
{
	typename range_iterator<R>::type b = boost::begin(r); 
	typename range_iterator<R>::type e = boost::end(r);
	for (; b != e; ++b)
	{
		in >> *b; 
	}
	return in;
}

//HELPER STRUCTS
template<class T> struct View: public sub_range<pair <T*, T*> >
{	
	View(pair<T*, T*>& p):
		sub_range<pair <T*, T*> >(p)
	{}

	View(T* first = 0, T* second = 0):
		sub_range<pair <T*, T*> >(make_pair(first, second))
	{}

	T& at(size_t i)
	{
		check(i < (size_t) this->size(), "at(" + str(i) + ") called for view of size " + str(this->size()));
		return (*this)[i];
	}

	const T& at(size_t i) const
	{
		check(i < this->size(), "at(" + str(i) + ") called for view of size " + str(this->size()));
		return (*this)[i];	
	}
	template<class R> const View<T>& operator =(const R& r) const
	{
		check(boost::size(r) == this->size(), "can't assign range " + str(r) + " to view " + str(*this));
		copy(r, *this);
		return *this;
	}
};

template<class T> struct Vector: public vector<T>
{
	Vector()
	{}
	Vector(const vector<T>& v):
		vector<T>(v)
	{}
	Vector(const View<const T>& v)
	{
		*this = v;
	}
	Vector(size_t n):
		vector<T>(n)
	{}
	Vector(size_t n, const T& t):
		vector<T>(n, t)
	{}
	Vector<T>& grow(size_t length)
	{
		this->resize(this->size() + length);
		return *this;
	}
	Vector<T>& shrink(size_t length)
	{
		this->resize(max((size_t)0, this->size() - length));
		return *this;
	}
	void push_front(const T& t)
	{
		this->insert(this->begin(), t);
	}
	void push_back(const T& t)
	{
		this->insert(this->end(), t);
	}
	T& pop_front()
	{
		T& front = front();
		erase(this->begin());
		return front;
	}
	View<T> slice(int first = 0, int last = numeric_limits<int>::max())
	{
		first = bound(first, 0, (int)this->size());
		if (last < 0)
		{
			last += (int)this->size();
		}
		last = bound(last, first, (int)this->size());
		return View<T>(&((*this)[first]), &((*this)[last]));
	}
	View<T> slice(pair<int, int>& r)
	{
		return slice(r.first, r.second);
	}
	const View<T> slice(int first = 0, int last = numeric_limits<int>::max()) const
	{
		return slice(first, last);
	}
	const View<T> slice(pair<int, int>& r) const
	{
		return slice(r.first, r.second);
	}
	template<class R> Vector<T>& extend(const R& r)
	{
		size_t oldSize = this->size();
		grow(boost::size(r));
		copy(boost::begin(r), boost::end(r), this->begin() + oldSize);
		return *this;
	}
	Vector<T> replicate(size_t times) const
	{
		Vector<T> v;
		REPEAT(times)
		{
			v.extend(*this);
		}
		return v;
	}
	template<class R> Vector<T>& operator =(const R& r)
	{
		resize(boost::size(r));
		copy(r, *this);
		return *this;
	}
	template<class T2> Vector<T2> to() const
	{
		Vector<T2> v;
		LOOP(const T& t, *this)
		{
			v += lexical_cast<T2>(t);
		}
		return v;
	}
};

template<class T> static vector<T> readFromFile(const string &filename, const char delim = '\n')
{
	ifstream instream(filename.c_str());
	check(instream.is_open(), "could not open file \"" + filename + "\"");
	vector<T> vect;
	string line;
	while(getline(instream, line, delim))
	{
		if(line.size() > 0)
		{
			vect += read<T>(line);
		}
		else
		{
			break;
		}
	}
	return vect;
}

typedef const boost::tuple<View<real_t>&, vector<real_t>&>& TVWDVD;
typedef const boost::tuple<View<real_t>&, View<real_t>&>& TVWDVWD;

#endif
