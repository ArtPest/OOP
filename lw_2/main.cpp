#include <iostream>

using namespace std;

int main(){
    Four a("3333"), b("1000"), c("1"), d, e, f;
    d = a + c;
    e = b - c;
    f = a - a;
    d.print();
    e.print();
    f.print();
}
