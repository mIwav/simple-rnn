

#ifndef _INCLUDED_multiarray_h
#define _INCLUDED_multiarray_h

#include <iostream>
#include <numeric>
#include <vector>
#include <algorithm>
#include <boost/array.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/optional.hpp>
#include <boost/range.hpp>

#include "helpers.hpp"
#include "list_of.hpp"

using namespace std;
using namespace boost;
using namespace boost::assign;

template <class T> struct MultiArray
{
	// data
	vector<T> data;
	vector<size_t> shape;
	vector<size_t> strides;

	//functions
	MultiArray()
	{
	}
	MultiArray(const vector<size_t>& s)
	{
		reshape(s);
	}
	MultiArray(const vector<size_t>& s, const T& fillval)
	{
		reshape(s, fillval);
	}
	virtual ~MultiArray()
	{
	}
	virtual size_t size() const
	{
		return data.size();
	}
	virtual size_t num_dims() const
	{
		return shape.size();
	}
	virtual bool empty() const
	{
		return data.empty();
	}
	virtual void resize_data()
	{
		if(product(shape) != 0)
		{
			data.resize(product(shape));
		}
		strides.resize(shape.size());
		strides.back() = 1;
		for(int i = shape.size() - 2; i >= 0; --i)
			strides.at(i) = strides.at(i+1) * shape.at(i);
	}
	template<class R> void reshape(const R& newShape)
	{
		assert(newShape.size());
		vector_assign(newShape, shape);
		resize_data();
	}
	void fill_data(const T& fillVal)
	{
		fill(data, fillVal);
	}
	template<class R> void reshape(const R& dims, const T& fillVal)
	{
		reshape(dims);
		fill_data(fillVal);
	}
	template<class T2> void assign(const MultiArray<T2>& a)
	{
		reshape(a.shape);
		copy(a.data.begin(), a.data.end(), data.begin());
	}
	template<class T2> MultiArray<T>& operator=(const MultiArray<T2>& a)
	{
		assign(a);
		return *this;
	}
	size_t offset(const vector<int>& coords) const
	{
		return inner_product(coords, strides);
	}
	const View<T> operator[](const vector<int>& coords)
	{
		// check(coords.size() <= shape.size(), "operator [" + str(coords) + "] called with shape " + str(shape));
		if (coords.empty())
		{
			return View<T>(&data.front(), &data.front() + data.size());
		}
		T* start = &data.front() + offset(coords); 
		T* end = start + strides[0];
		return View<T>(start, end);
	}
	const View<const T> operator[](const vector<int>& coords) const
	{
		// check(coords.size() <= shape.size(), "operator [" + str(coords) + "] called with shape " + str(shape));
		if (coords.empty())
		{
			return View<const T>(&data.front(), &data.front() + data.size());
		}
		const T* start = &data.front() + offset(coords); 
		// VSTCI strideIt = strides.begin() + offset(coords);
		const T* end = start + strides[0];
		return View<const T>(start, end);
	}
	
	bool in_range(const vector<int>& coords) const
	{
		if (coords.size() > shape.size())
		{
			return false;
		}
		VSTCI shapeIt = shape.begin();
		for (VICI coordIt = coords.begin(); coordIt != coords.end(); ++coordIt, ++shapeIt)
		{
			int c = *coordIt;
			if (c < 0 || c >= static_cast<int>(*shapeIt))
			{
				return false;
			}
		}
		return true;
	}

	const View<T> range(const vector<const vector<int>>& coords)
	{
		check(coords.size() == 2, "Range only can have two coord pairs <dim1, dim2>")
		View<T> startAddr = (*this)[coords[0]];
		View<T> stopAddr  = (*this)[coords[1]];
		return View<T>(&startAddr.at(0), &stopAddr.at(0));
	}

	void print(ostream &out) const
	{
		BOOST_FOREACH(int j, span(shape[1]))
		{
			BOOST_FOREACH(int i, span(shape[0]))
			{
				out << (*this)[list_of(i)(j)] << " ";
			}
			out << endl;
		}
	}
};

template <class T> static bool operator == (const MultiArray<T>& a, const MultiArray<T>& b)
{
	return (a.data == b.data && a.shape == b.shape);
}

template <class T> static ostream& operator<<(ostream &out, MultiArray<T> &a)
{
	a.print(out);
	return out;
}

#endif