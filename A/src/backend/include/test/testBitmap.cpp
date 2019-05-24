#include <iostream>
#include "../bitset.h"



int main()
{
    Bitset bs;
    for(int i=1; i<18; i+=3)
    {
        bs.set(i);
    }
    cout << bs <<endl;
    cout << bs.test(-1) << bs.test(100)<<bs.test(4)<<endl;
    return 0;
}