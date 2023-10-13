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
    Point o;
    Point* vertices;
    float area = -1.0;
    
public:
    virtual ~Figure() noexcept = default;
    
    //explicit virtual operator double() const noexcept = 0;
    
    friend ostream& operator<<(ostream& os, const Figure& f){
        if(f.vertices != NULL){
            os << "Coordinates: ";
            for(size_t i = 0; i < sizeof(vertices); ++i)
                os << f.vertices[i];
        }
        os << "Center: " << f.o << "Area: " << f.area << '\n';
        return os;
    }

};

/*class Square: class Figure{
}*/


int main(){
    Figure f;
    cout << f;
    return 0;
}
