#include <iostream>
#include <initializer_list>

class Four {
    unsigned char* data;
    size_t size;

public:
    Four();
    Four(const size_t &n, unsigned char t = 0);
    Four(const initializer_list<unsigned char>& t);
    Four(const std::string &t);
    Four(const Four& other);
    Four(Four&& other) noexcept;
    virtual ~Four() noexcept;

    void print() const;
    bool operator ==(const Four& other) const;
    bool operator <(const Four& other) const;
    bool operator >(const Four& other) const;
    Four& operator =(const Four& other);
    Four operator +(const Four& other) const;
    Four operator -(const Four& other) const;
};
