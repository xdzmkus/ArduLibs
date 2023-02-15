#ifndef _LOOPINDEX_HPP_
#define _LOOPINDEX_HPP_

template <class T>
class LoopIndex
{
public:

	LoopIndex(T min, T max) : _index(min), _minIndex(min), _maxIndex(max)
	{
	};
	~LoopIndex()
	{
	};

	T set(T index)
	{
		if (index >= _maxIndex) _index = _maxIndex;
		else if (index <= _minIndex) _index = _minIndex;
		else _index = index;

		return _index;
	}

	inline T getMin() const
	{
		return _minIndex;
	}

	inline T getMax() const
	{
		return _maxIndex;
	}

	inline operator T() const
	{
		return _index;
	}

	LoopIndex operator++()
	{
		increment();
		return LoopIndex(*this);
	}

	LoopIndex operator++(int)
	{
		LoopIndex preModified(*this);
		increment();
		return preModified;
	}

	LoopIndex operator--()
	{
		decrement();
		return LoopIndex(*this);
	}

	LoopIndex operator--(int)
	{
		LoopIndex preModified(*this);
		decrement();
		return preModified;
	}

protected:

	inline void increment()
	{
		if (_index == _maxIndex) _index = _minIndex;
		else ++_index;
	}

	inline void decrement()
	{
		if (_index == _minIndex) _index = _maxIndex;
		else --_index;
	}

private:

	T _index;
	T _minIndex;
	T _maxIndex;

};


#endif