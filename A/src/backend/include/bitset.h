#pragma once

#include <iostream>
#include <vector>

typedef unsigned char uchar;

class Bitset
{
  protected:
	std::vector<uchar> _mem;

  public:
	Bitset() {}
	~Bitset() {}
	Bitset(int max) : _mem(1 + (max >> 3), 0) {}
	void set(int k)
	{
		if ((k >> 3) >= _mem.size())
			_mem.resize(1 + (k >> 3));
		_mem[k >> 3] |= (0x80 >> (k & 0x07));
	}
	void unset(int k) { _mem[k >> 3] &= ~(0x80 >> (k & 0x07)); }
	bool test(int k) const
	{
		if (0 <= k && (k >> 3) < _mem.size())
			return _mem[k >> 3] & (0x80 >> (k & 0x07));
		return 0;
	}
	bool operator[](int k) const { return test(k); }
	int count() const
	{
		int ones = 0;
		for (auto &uc : _mem)
			ones += countOnes(uc);
		return ones;
	}
	static int countOnes(uchar n)
	{
		int ones = 0;
		while (0 < n)
		{
			ones++;
			n &= n - 1;
		}
		return ones;
	}
	friend std::ostream &operator<<(std::ostream &os, const Bitset &bs)
	{
		int maxN = bs._mem.size() << 3;
		for (int i = 0; i < maxN; i++)
			os << bs.test(i);
		return os;
	}
};
