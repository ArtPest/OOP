#include <iostream>

using namespace std;

struct Point{
    float x, y;
    
    Point(float a = 0.0, float b = 0.0): x(a), y(b) {}
    
    friend ostream& operator<<(ostream& os, const Point& p){
        return os << "(" << p.x << ", " << p.y << ")\n";
    }
};

class Figure{
protected:
    //
    
public:
    //
};

class Square: class Figure{
    float length = -1.0;

public:

}


int main(){
    //
    return 0;
}
