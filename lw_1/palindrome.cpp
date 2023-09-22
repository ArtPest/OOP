#include "palindrome.h"

void palindrome(string& str, bool& flag){
    int left = 0, right = str.length() - 1;
    while (left < right)
        if (str[left++] != str[right--])
            flag = false;
}
