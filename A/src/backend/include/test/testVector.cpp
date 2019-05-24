#include <iostream>
#include "../vector.h"


int main()
{
    Vector<int> a;
    for(int i=0;i<6;i++)
    {
        a.push_back(i);
    }
    cout << a.remove(1);
    return 0;
}