#include <iostream>
#include <cmath>
#include <algorithm>

using namespace std;

struct Point final {
    float x, y;
    Point(float a = 0.0, float b = 0.0): x(a), y(b) {}
    
    Point& operator +=(const Point& other) {
        x += other.x;
        y += other.y;
        return *this;
    }
    
    Point& operator -(const Point& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }
    
    Point& operator /=(float f) {
        if(f != 0) {
            x /= f;
            y /= f;
        }
        else
            throw invalid_argument("ZERO_DIVISION");
        return *this;
    }
    
    float distance(const Point& other) const {
        return sqrt((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y));
    }
    
    friend ostream& operator <<(ostream& os, const Point& p) {
        return os << "(" << p.x << ", " << p.y << ")\n";
    }
    
    friend istream& operator >>(istream& is, Point& p) {
        return is >> p.x >> p.y;
    }
};

bool parallel(const Point& p1, const Point& p2, const Point& p3, const Point& p4) {
    if (p1.x - p2.x == 0 && p3.x - p4.x == 0)
        return true;
    else if (p1.x - p2.x == 0 || p3.x - p4.x == 0)
        return false;
    return (p2.y - p1.y) / (p2.x - p1.x) == (p4.y - p3.y) / (p4.x - p3.x);
}

class Figure {
protected:
    int n = -1;
    Point* vertices;
    
    void sort_ver() {
        if(n <= 1)
            return;
        size_t p0_index = 0;
        for(size_t i = 1; i < n; i++)
            if ((vertices[i].y < vertices[p0_index].y) || 
                (vertices[i].y == vertices[p0_index].y && vertices[i].x < vertices[p0_index].x))
                p0_index = i;
        swap(vertices[0], vertices[p0_index]);
        sort(vertices + 1, vertices + n, [this](const Point &p1, const Point &p2) {
            float o = (p1.y - vertices[0].y) * (p2.x - vertices[0].x) - 
                      (p1.x - vertices[0].x) * (p2.y - vertices[0].y);
            if(o == 0)
                return vertices[0].distance(p1) < vertices[0].distance(p2);
            else
                return o > 0;
        });
        size_t m = 1;
        for(size_t i = 1; i < n; i++){
            while (i < n - 1 && ((vertices[i].y - vertices[0].y) * (vertices[i + 1].x - vertices[0].x) - 
                                 (vertices[i].x - vertices[0].x) * (vertices[i + 1].y - vertices[0].y)) == 0)
                i++;
            vertices[m] = vertices[i];
            m++;
        }
        n = m;
    }
    
public:
    Figure() = default;

    Figure(const Figure& other): n(other.n) {
        vertices = new Point[n];
        for (size_t i = 0; i < n; ++i)
            vertices[i] = other.vertices[i];
    }
    
    Figure& operator =(const Figure& other) {
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
    
    Point center() const {
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
        if(f.n == -1){
            cout << "Type in number of vertices: ";
            is >> f.n;
            if(f.n < 0)
                throw invalid_argument("IMPOSSIBLE_FIGURE");
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
        f.sort_ver();
        if(f.area() <= 0)
            throw invalid_argument("IMPOSSIBLE_FIGURE");
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
    
    virtual explicit operator double() const {
        return static_cast<double>(area());
    }
};

class Square final: public Figure {
public:
    Square(): Figure() {
        n = 4;
        vertices = new Point[n];
    }
        
    float length() const {
        return vertices[0].distance(vertices[1]);
    }
    
    float area() const override {
        return length() * length();
    }
    
    friend istream& operator >>(istream& is, Square& f) {
        if(f.vertices != nullptr)
            delete[] f.vertices;
        f.vertices = new Point[f.n];
        cout << "Type in coordinates:\n";
        for (size_t i = 0; i < f.n; ++i)
            is >> f.vertices[i];
        f.sort_ver();
        if((f.vertices[0].distance(f.vertices[1]) != f.vertices[0].distance(f.vertices[2]))
            or f.vertices[0].distance(f.vertices[2]) != f.vertices[1].distance(f.vertices[3]))
            throw invalid_argument("IMPOSSIBLE_SQUARE");
        return is;
    }
    
    bool operator ==(const Square& other) const {
        return length() == other.length();
    }
    
    explicit operator double() const override {
        return static_cast<double>(area());
    }
};

class Rectangle final: public Figure {
public:
    Rectangle(): Figure() {
        n = 4;
        vertices = new Point[n];
    }
    
    float length() const {
        return max(vertices[0].distance(vertices[1]), vertices[0].distance(vertices[3]));
    }
    
    float width() const {
        return min(vertices[0].distance(vertices[1]), vertices[0].distance(vertices[3]));
    }
    
    float area() const override {
        return length() * width();
    }
    
    friend istream& operator >>(istream& is, Rectangle& f) {
        if(f.vertices != nullptr)
            delete[] f.vertices;
        f.vertices = new Point[f.n];
        cout << "Type in coordinates:\n";
        for (size_t i = 0; i < f.n; ++i)
            is >> f.vertices[i];
        f.sort_ver();
        if((f.vertices[0].distance(f.vertices[1]) != f.vertices[2].distance(f.vertices[3]))
            or f.vertices[0].distance(f.vertices[2]) != f.vertices[1].distance(f.vertices[3]))
            throw invalid_argument("IMPOSSIBLE_RECTANGLE");
        return is;
    }
    
    bool operator ==(const Rectangle& other) const {
        return length() == other.length() and width() == other.width();
    }
    
    explicit operator double() const override {
        return static_cast<double>(area());
    }
};

class Trapezoid final: public Figure {
public:
    Trapezoid(): Figure() {
        n = 4;
        vertices = new Point[n];
    }
    
    float top() const {
        if(parallel(vertices[0], vertices[1], vertices[2], vertices[3]))
            return min(vertices[0].distance(1), vertices[2].distance(3));
        return min(vertices[0].distance(2), vertices[1].distance(3));    
    }
    
    float bottom() const {
        if(parallel(vertices[0], vertices[1], vertices[2], vertices[3]))
            return max(vertices[0].distance(1), vertices[2].distance(3));
        return max(vertices[0].distance(2), vertices[1].distance(3));
    }
    
    float heigth() const {
        return area() * 2 / (top() + bottom());
    }
    
    friend istream& operator >>(istream& is, Trapezoid& f) {
        if(f.vertices != nullptr)
            delete[] f.vertices;
        f.vertices = new Point[f.n];
        cout << "Type in coordinates:\n";
        for (size_t i = 0; i < f.n; ++i)
            is >> f.vertices[i];
        f.sort_ver();
        if((parallel(f.vertices[0], f.vertices[1], f.vertices[2], f.vertices[3]) 
            and not parallel(f.vertices[0], f.vertices[2], f.vertices[1], f.vertices[3]))
            or (parallel(f.vertices[0], f.vertices[2], f.vertices[1], f.vertices[3]) 
            and not parallel(f.vertices[0], f.vertices[1], f.vertices[2], f.vertices[3])))
            throw invalid_argument("IMPOSSIBLE_TRAPEZOID");
        return is;
    }
    
    bool operator ==(const Trapezoid& other) const {
        //Не работает. Скорее всего, из-за неточного исчисления корней чисел с плавающей точкой
        return vertices[0].distance(vertices[1]) + 
            vertices[1].distance(vertices[2]) + 
            vertices[2].distance(vertices[3]) + 
            vertices[3].distance(vertices[0]) + heigth() ==
            other.vertices[0].distance(other.vertices[1]) + 
            other.vertices[1].distance(other.vertices[2]) +
            other.vertices[2].distance(other.vertices[3]) +
            other.vertices[3].distance(other.vertices[0]) + other.heigth();
    }
    
    explicit operator double() const override {
        return static_cast<double>(area());
    }
};

int main() {
    Figure f;
    cout << "Figure!\n";
    cin >> f;
    cout << f;
    
    cout << "\nSquare!\n";
    Square s;
    cin >> s;
    cout << s;
    
    cout << "\nRectangle!\n";
    Rectangle r;
    cin >> r;
    cout << r;
    
    cout << "\nTrapezoid!\n";
    Trapezoid t_0;
    cin >> t_0;
    cout << t_0;
    
    cout << "\nAnother Trapezoid!\n";
    Trapezoid t_1;
    cin >> t_1;
    cout << t_1;
    
    if(t_0 == t_1)
        cout << "\nTrapezoids are equal!";
    else
        cout << "\nTrapezoids are not equal!";
    return 0;
}
