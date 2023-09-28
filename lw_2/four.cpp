#include <iostream>

using namespace std;

class Four{
    unsigned char* data;
    size_t size;
    
public:
    Four(): size(0), data(nullptr) {}

    Four(const size_t& n, unsigned char t = 0): size(n), data(new unsigned char[n]){
        for(size_t i = 0; i < n; i++)
            data[i] = t;
    }
    
    Four(const initializer_list<unsigned char> &t): size(t.size()), data(new unsigned char[t.size()]){
        for(size_t i = 0; i < t.size(); i++)
            data[i] = t[i];
    }

    Four(const string &t): size(t.length()), data(new unsigned char[t.length()]){
        for (size_t i = 0; i < t.length(); ++i)
            data[i] = t[i];
    }

    Four(const Four& other): size(other.size), data(new unsigned char[other.size]){
        for (size_t i = 0; i < other.size; ++i)
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
};
