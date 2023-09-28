#include <iostream>

using namespace std;

int main(){
    Four a("333"), b("1"), c, d;
    c = a + b;
    d = a;
    a.print();
    b.print();
    c.print();
    d.print();
}
