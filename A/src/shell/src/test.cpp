#include "regexpr.h"

#include <iostream>

using namespace std;

int main() {
    while (true)
    {
        string a, b;
        cin >> a >> b;
        cout << sqlLike(a, b) << endl;
    }
    return 0;
}