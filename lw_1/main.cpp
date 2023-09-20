#include <iostream>
#include <string>
#include "palindrome.h"

using namespace std;

int main(){
    string str, res;
    bool flag = true;
    cin >> str;
    palindrome(str, flag);
    res = flag ? "YES" : "NO";
    cout << res << "\n";
    return 0;
}
