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
    int n;
    Point o;
    Point* vertices;
    float area = -1.0;
    
    Point center(){
        Point result;
        for(size_t i = 0; i < n; ++i){
            result.x += vertices[i].x;
            result.y += vertices[i].y;
        }
        result.x /= n;
        result.y /= n;
        return result;
    }
    
public:
    virtual ~Figure() noexcept = default;
    
    //explicit virtual operator double() const noexcept = 0;
    
    friend ostream& operator<<(ostream& os, const Figure& f){
        if(f.vertices != NULL){
            os << "Coordinates:\n";
            for(size_t i = 0; i < f.n; ++i)
                os << f.vertices[i];
        }
        os << "Center: " << f.o << "Area: " << f.area << '\n';
        return os;
    }
};

class Square final: public Figure{
public:
    Square(){
        n = 4;
        vertices = new Point[n];
        o = center();
    }
    //Square(Point a, Point b, Point c, Point d) {}
};


int main(){
    Figure f;
    Square s;
    cout << f;
    cout << s;
    return 0;
}
