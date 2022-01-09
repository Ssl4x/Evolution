#ifndef DOMAIN_H
#define DOMAIN_H

typedef signed		char		sint_8;
typedef unsigned	char		uint_8;
typedef signed		short		sint_16;
typedef unsigned	short		uint_16;
typedef signed		long		sint_32;
typedef unsigned	long		uint_32;
typedef signed		long long	sint_64;
typedef unsigned	long long	uint_64;

template <typename T>
union Pair
{
	struct
	{
		T i, j;
	};
	struct
	{
		T x, y;
	};
	struct
	{
		T first, second;
	};

	Pair()
	{
		i = j = 0;
	}
	Pair(const T iInit, const T jInit) :
		i(iInit),
		j(jInit)
	{}

	T& operator[] (const int num)
	{
		return num ? j : i;
	}

	void operator+= (const Pair& n)
	{
		i += n.i;
		j += n.j;
	}
	void operator-= (const Pair& n)
	{
		i -= n.i;
		j -= n.j;
	}

	Pair operator+ (const Pair& n) const
	{
		Pair ret;
		ret.i = i + n.i;
		ret.j = j + n.j;
		return ret;
	}
	Pair operator- (const Pair& n) const
	{
		Pair ret;
		ret.i = i - n.i;
		ret.j = j - n.j;
		return ret;
	}

	bool operator< (const Pair& n) const
	{
		if (i != n.i) return i < n.i;
		return j < n.j;
	}
	bool operator>(const Pair& n) const
	{
		if (i != n.i) return i > n.i;
		return j > n.j;
	}
	bool operator== (const Pair& n) const
	{
		return i == n.i && j == n.j;
	}
	bool operator!= (const Pair& n) const
	{
		return i != n.i || j != n.j;
	}
};

#endif // DOMAIN_H