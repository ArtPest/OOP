#include "palindrome.h"
#include <cctype>

void palindrome(string& str, bool& flag){
    int left = 0, right = str.length() - 1;
    while(left < right){
        while(!isalnum(str[left++]));
        while(!isalnum(str[right--]));
        if(tolower(str[left++]) != tolower(str[right--])){
            flag = false;
            return;
        }
    }
}
