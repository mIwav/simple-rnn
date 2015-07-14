

#ifndef _INCLUDED_seqbuffer_h
#define _INCLUDED_seqbuffer_h

#include "helpers.hpp"
#include "multiarray.hpp"

template <class T> struct SeqBuffer:public MultiArray<T>
{
	//data
	

	//functions

	SeqBuffer(/*vector<size_t> shape = list_of<size_t>(0)*/)
	{
		/*this->shape = shape;
		reshape(shape);*/
	}

	SeqBuffer(const SeqBuffer& sb)
	{
		*this = sb;
	}

	virtual ~SeqBuffer()
	{
	}

	const View<const T> operator[](int coord) const
	{
		check(this->shape.size(), "operator [" + str(coord) + "] called for empty array");
		const T* start = &this->data.front() + (coord * this->shape.back());
		const T* end = start  + this->shape.back();
		return View<const T>(start, end);
	}

	const View<const size_t> seq_shape() const
	{
		return View<const size_t>(&this->shape.front(), &this->shape.back());
	}

	template<class T2> void reshape(const SeqBuffer<T2>& buff)
	{
		reshape(buff.seq_shape());
	}

	template<class T2> void reshape(const SeqBuffer<T2>& buff, const T& fillVal)
	{
		reshape(buff.seq_shape(), fillVal);
	}

	template<class R> void reshape(const R& newSeqShape)
	{
		vector_assign(newSeqShape, this->shape); // override first dimension and delete second
		this->resize_data();
	}

	template<class R> void reshape(const R& newSeqShap, const T& fillval)
	{
		reshape(newSeqShap);
		fill(this->data, fillval);
	}

	void print(ostream& out, string delim1 =",", string delim2 = ";") const
	{
		//out << "DIMENSIONS: " << seq_size() << endl;
		BOOST_FOREACH(int j, span(seq_size()))
		{
			BOOST_FOREACH(int i, (span(this->shape.back())))
			{
				out << setprecision(9) << (*this)[j][i] << " ";
			}
			out <<endl;
		}
	}
	template<class T2> SeqBuffer<T>& operator =(const SeqBuffer<T2>& a)
	{
		depth = a.depth;
		MultiArray<T>::assign(a);
		return *this;
	}

	using MultiArray<T>::operator[];
	//const View<T> operator[](int coord)
	//{
	//	check(this->shape.size(), "operator [" + str(coord) + "] called for empty array");
	//	T* start = &this->data.front() + (coord * this->shape.back());
	//	T* end = start  + this->shape.front();
	//	return View<T>(start, end);
	//}
	const View<T> front()
	{
		return (*this)[*begin()];
	}
	const View<T> back()
	{
		return (*this)[*rbegin()];
	}

	size_t seq_size() const
	{
		return product(seq_shape());
	}
};

template <class T> static ostream& operator <<(ostream& out, const SeqBuffer<T>& a)
{
	a.print(out);
	return out;
}
#endif