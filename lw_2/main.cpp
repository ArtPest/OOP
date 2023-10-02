#include <iostream>
#include "four.h"

using namespace std;

int main(){
    Four a("3333"), b = {1, 0, 0, 0}, c("1"), d, e, f(3, 2);
    d = a + c;
    e = b - c;
    d.print();
    e.print();
    f.print();
    try {
        f = c - b;
    } catch(const exception& exc){
        cerr << "ERROR: " << exc.what() << '\n';
        return -1;
    }
    cout << (a > b ? "YES" : "NO") << '\n';
}
