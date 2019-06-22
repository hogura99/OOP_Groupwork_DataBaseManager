#include <iostream>
#include <fstream>

#include "regexpr.h"
#include "fileio.h"

using namespace std;

int main() {
    int arr[5] = {0};
    int *pt = NULL;
    std::cout << typeid(arr).name() << std::endl;
    cout << typeid(pt).name() << endl;
    cout << __ProgramPath << endl;
    while (true)
    {
        string a, b;
        cin >> a >> b;
        cout << sqlLike(a, b) << endl;
    }
    return 0;
}