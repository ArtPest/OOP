#include "turkish_numbers.h"

using namespace std;

void number_translater(int num, string& str_num){
    string ones[] = {"sıfır", "bir", "iki", "üç", "dört", "beş", "altı", "yedi", "sekiz", "dokuz"},
    tens[] = {"-", "on", "yirmi", "otuz", "kırk", "elli", "altmış", "yetmiş", "seksen", "doksan"};
    if(num / 10 == 0)
        str_num = ones[num % 10];
    else if(num % 10 == 0)
        str_num = tens[num / 10];
    else
        str_num = tens[num / 10] + ' ' + ones[num % 10];
}
