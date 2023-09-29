#include <iostream>
#include <initializer_list>
#include <cstring>
#include "four.h"

using namespace std;

class Four final{
    unsigned char* data;
    size_t size;
    
public:
    Four(): size(0), data(nullptr) {}

    Four(const size_t& n, unsigned char t = 0): size(n), data(new unsigned char[n]){
        for(size_t i = 0; i < n; ++i)
            data[i] = t;
    }
    
    /*Four(const initializer_list<unsigned char> &t): size(t.size()), data(new unsigned char[t.size()]){
        //
    }*/

    Four(const string &t): size(t.length()), data(new unsigned char[t.length()]){
        for(size_t i = 0; i < t.length(); ++i)
            data[i] = t[size - 1 - i] - '0';
    }

    Four(const Four& other): size(other.size), data(new unsigned char[other.size]){
        memcpy(data, other.data, other.size);
    }
    
    Four(Four&& other) noexcept: size(other.size), data(other.data){
        other.size = 0;
        other.data = nullptr;
    }
    
    ~Four() noexcept{
        if(data != nullptr)
            delete[] data;
    }
    
    void print() const{
        for (size_t i = size - 1; i != -1; --i)
            cout << static_cast<int>(data[i]);
        cout << endl;
    }
    
    bool operator ==(const Four& other) const{
        if(size != other.size)
            return false;
        for(size_t i = 0; i < size; ++i)
            if(data[i] != other.data[i])
                return false;
        return true;
    }
    
    bool operator <(const Four& other) const{
        if(size != other.size)
            return size < other.size;
        for(size_t i = size - 1; i != -1; --i)
            if(data[i] < other.data[i])
                return true;
            else if(data[i] > other.data[i])
                return false;
        return false;
    }
    
    bool operator >(const Four& other) const{
        if(size != other.size)
            return size > other.size;
        for(size_t i = size - 1; i != -1; --i)
            if(data[i] > other.data[i])
                return true;
            else if(data[i] < other.data[i])
                return false;
        return false;
    }
    
    Four& operator =(const Four& other){
        if(this == &other)
            return *this;
        if(data != nullptr)
            delete[] data;
        size = other.size;
        data = new unsigned char[size];
        for(size_t i = 0; i < size; ++i)
            data[i] = other.data[i];
        return *this;
    }
    
    Four operator +(const Four& other) const{
        size_t max_s = max(size, other.size);
        Four result(max_s, 0);
        unsigned char rema = 0;
        for(size_t i = 0; i < max_s; ++i){
            int sum = rema;
            if(i < size)
                sum += data[i];
            if(i < other.size)
                sum += other.data[i];
            result.data[i] = sum % 4;
            rema = sum / 4;
        }
        if(rema){
            unsigned char* tmp = new unsigned char[max_s + 1];
            for(size_t i = 0; i < max_s; ++i)
                tmp[i] = result.data[i];
            tmp[max_s] = rema;
            delete[] result.data;
            result.data = tmp;
            ++result.size;
        }
        return result;
    }
    
    Four operator -(const Four& other) const{
        Four result;
        if(*this < other)
            throw invalid_argument("IMPOSSIBLE_SUBTRACTION");
        result = *this;
        unsigned char borr = 0;
        for(size_t i = 0; i < size; ++i){
            int dif = result.data[i] - borr;
            if(i < other.size)
                dif -= other.data[i];
            if(dif < 0){
                dif += 4;
                borr = 1;
            } else
                borr = 0;
            result.data[i] = dif;
        }
        while(result.data[result.size - 1] == 0 and result.size > 1){
            unsigned char* tmp = new unsigned char[result.size - 1];
            for(size_t i = 0; i < result.size - 1; ++i)
                tmp[i] = result.data[i];
            delete[] result.data;
            result.data = tmp;
            --result.size;
        }
        return result;
    }
};
