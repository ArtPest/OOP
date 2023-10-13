#include <iostream>

using namespace std;

struct Point final{
    float x, y;
    
    Point(float a = 0.0, float b = 0.0): x(a), y(b) {}
    
    friend ostream& operator<<(ostream& os, const Point& p){
        return os << "(" << p.x << ", " << p.y << ")\n";
    }
};

class Figure{
protected:
    Point a, b, c, d, o;
    float area = -1.0;
    
public:
    friend ostream& operator<<(ostream& os, const Figure& f){
        return os << "Coordinates:\n" << f.a << f.b << f.c << f.d << "Center: " <<
            f.o << "Area: " << f.area << '\n';
    }
};

/*class Square: class Figure{
    float length = -1.0;

public:

}*/


int main(){
    Figure f;
    cout << f;
    return 0;
}
