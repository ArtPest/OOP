#include <iostream>

using namespace std;

class Four{
    unsigned char* data;
    size_t size;
    
public:
    Four(): size(0), data(nullptr) {}

    Four(const size_t& n, unsigned char t = 0): size(n), data(new unsigned char[n]){
        for(size_t i = 0; i < n; ++i)
            data[i] = t;
    }
    
    Four(const initializer_list<unsigned char> &t): size(t.size()), data(new unsigned char[t.size()]){
        //
    }

    Four(const string &t): size(t.length()), data(new unsigned char[t.length()]){
        for(size_t i = 0; i < t.length(); ++i)
            data[i] = t[i];
    }

    Four(const Four& other): size(other.size), data(new unsigned char[other.size]){
        for(size_t i = 0; i < other.size; ++i)
            data[i] = other.data[i];
    }
    
    Four(Four&& other) noexcept: size(other.size), data(other.data){
        other.size = 0;
        other.data = nullptr;
    }
    
    virtual ~Four() noexcept{
        if(data != nullptr)
            delete[] data;
    }
    
    void print(){
        for(size_t i = size - 1; i != -1; --i)
            cout << data[i];
        cout << '\n';
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
        unsigned char rem = 0;
        for(size_t i = 0; i < max_s; ++i){
            int sum = rem;
            if(i < size)
                sum += data[i];
            if(i < other.size)
                sum += other.data[i];
            result.data[i] = sum % 4;
            rem = sum / 4;
        }
        if(rem){
            //
        }
        return result;
    }
};
