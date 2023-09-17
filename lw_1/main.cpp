#include <iostream>
#include "turkish_numbers.h"

using namespace std;

int main(){
    int num;
    string str_num;
    cin >> num;
    number_translater(num, str_num);
    cout << str_num << '\n';
    return 0;
}
