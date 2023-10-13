#include <iostream>

using namespace std;

struct Point final {
    float x, y;

    Point(float a = 0.0, float b = 0.0): x(a), y(b) {}
    
    Point& operator +=(const Point& other) {
        x += other.x;
        y += other.y;
        return *this;
    }
    
    Point& operator /=(float f) {
        if(f != 0) {
            x /= f;
            y /= f;
        }
        //else
        return *this;
    }
        
    friend ostream& operator <<(ostream& os, const Point& p) {
        return os << "(" << p.x << ", " << p.y << ")\n";
    }
    
    friend istream& operator >>(istream& is, Point& p) {
        return is >> p.x >> p.y;
    }
};

class Figure {
protected:
    int n = -1;
    Point* vertices;
    
public:
    Figure(const Figure& other): n(other.n) {
        vertices = new Point[n];
        for (size_t i = 0; i < n; ++i)
            vertices[i] = other.vertices[i];
    }
    
    Figure& operator=(const Figure& other) {
        if (this == &other)
            return *this;
        delete[] vertices;
        n = other.n;
        vertices = new Point[n];
        for (size_t i = 0; i < n; ++i)
            vertices[i] = other.vertices[i];
        return *this;
    }

    virtual ~Figure() noexcept {
        delete[] vertices;
    }
    
    virtual Point center() const {
        Point result;
        for(size_t i = 0; i < n; ++i) {
            result += vertices[i];
        }
        result /= n;
        return result;
    }
    
    virtual float area() const {
        auto tr_ar = [](const Point& A, const Point& B, const Point& C) -> float {
            return 0.5 * abs(A.x * (B.y - C.y) + B.x * (C.y - A.y) + C.x * (A.y - B.y));
        };
        float result = 0.0;
        for(size_t i = 1; i < n - 1; ++i)
            result += tr_ar(vertices[0], vertices[i], vertices[i + 1]);
        return result;
    }
        
    friend istream& operator >>(istream& is, Figure& f) {
        if(f.n == -1) {
            cout << "Type in number of vertices: ";
            is >> f.n;
            //if(f.n < 0)
        }
        if(f.n <= 0) {
            f.vertices = nullptr;
            return is;
        }
        if(f.vertices != nullptr)
            delete[] f.vertices;
        f.vertices = new Point[f.n];
        cout << "Type in coordinates:\n";
        for (size_t i = 0; i < f.n; ++i)
            is >> f.vertices[i];
        return is;
    }
    
    friend ostream& operator <<(ostream& os, const Figure& f) {
        if(f.vertices != nullptr){
            os << "Coordinates:\n";
            for(size_t i = 0; i < f.n; ++i)
                os << f.vertices[i];
        }
        os << "Center: " << f.center() << "Area: " << f.area() << '\n';
        return os;
    }
};

class Square final: public Figure {
public:
    Square() {
        n = 4;
        vertices = new Point[n];
    }
    //Square(Point a, Point b, Point c, Point d) {}
    
    
};


int main() {
    Figure f;
    cout << "Figure!\n";
    cin >> f;
    cout << f;
    cout << "Square!\n";
    Square s;
    cin >> s;
    cout << s;
    return 0;
}
