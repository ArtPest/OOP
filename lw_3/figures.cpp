#include <iostream>
#include <cmath>
#include <algorithm>

#define EPS 1e-6
#define RUDE_EPS 0.1

using namespace std;

struct Point final {
    float x, y;
    Point(float a = 0.0, float b = 0.0): x(a), y(b) {}
    
    Point& operator +=(const Point& other) {
        x += other.x;
        y += other.y;
        return *this;
    }
    
    Point operator -(const Point& other) {
        Point result(x - other.x, y - other.y);
        return result;
    }
    
    Point& operator /=(float f) {
        if(fabs(f) <= EPS)
            throw invalid_argument("ZERO_DIVISION");
        else {
            x /= f;
            y /= f;
        }
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
    if(abs(p1.x - p2.x) < EPS and abs(p3.x - p4.x) < EPS)
        return true;
    else if(abs(p1.x - p2.x) < EPS or abs(p3.x - p4.x) < EPS)
        return false;
    return abs((p2.y - p1.y) / (p2.x - p1.x) - (p4.y - p3.y) / (p4.x - p3.x)) < EPS;
}

class Figure {
protected:
    int n = -1;
    Point* vertices;
    
    virtual bool check() const {
        return !(area() <= 0.0);
    }
    
public:
    Figure() = default;

    Figure(const Figure& other): n(other.n) {
        vertices = new Point[n];
        for (size_t i = 0; i < n; ++i)
            vertices[i] = other.vertices[i];
    }
    
    Figure& operator =(const Figure& other) {
        if(this == &other)
            return *this;
        delete[] vertices;
        n = other.n;
        vertices = new Point[n];
        for(size_t i = 0; i < n; ++i)
            vertices[i] = other.vertices[i];
        return *this;
    }
    
    Figure(Figure&& other): n(other.n), vertices(other.vertices) {
        other.n = -1;
        other.vertices = nullptr;
    }
    
    Figure& operator =(Figure&& other) {
        if (this == &other)
            return *this;
        delete[] vertices;
        n = other.n;
        vertices = other.vertices;
        other.n = -1;
        other.vertices = nullptr;
        return *this;
    }

    virtual ~Figure() noexcept {
        delete[] vertices;
    }
    
    Point center() const {
        Point result;
        for(size_t i = 0; i < n; ++i)
            result += vertices[i];
        result /= n;
        return result;
    }
    
    virtual void add_point(const Point p) {
        Point* result = new Point[n + 1];
        for(size_t i = 0; i < n; ++i)
            result[i] = vertices[i];
        result[n++] = p;
        delete[] vertices;
        vertices = result;
    }
    
    virtual float perimeter() const {
        float result = vertices[n - 1].distance(vertices[0]);
        for(size_t i = 0; i < n - 1; ++i)
            result += vertices[i].distance(vertices[i + 1]);
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
        for (size_t i = 0; i < f.n; ++i)
            is >> f.vertices[i];
        if(!f.check())
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
    virtual bool check() const override {
        return !(abs(vertices[0].distance(vertices[1]) - vertices[0].distance(vertices[3])) > EPS
            or abs(vertices[0].distance(vertices[2]) != vertices[1].distance(vertices[3])) > EPS);
    }
    
public:
    Square(): Figure() {
        n = 4;
        vertices = nullptr;
    }
        
    float length() const {
        return vertices[0].distance(vertices[1]);
    }
    
    float area() const override {
        return length() * length();
    }
    
    void add_points(const Point& a, const Point& b, const Point& c, const Point& d) {
        vertices = new Point[4];
        vertices[0] = a;
        vertices[1] = b;
        vertices[2] = c;
        vertices[3] = d;
        if(!check())
            throw invalid_argument("IMPOSSIBLE_SQUARE");
    }
    
    friend istream& operator >>(istream& is, Square& f) {
        if(f.vertices != nullptr)
            delete[] f.vertices;
        f.vertices = new Point[f.n];
        for (size_t i = 0; i < f.n; ++i)
            is >> f.vertices[i];
        if(!f.check())
            throw invalid_argument("IMPOSSIBLE_SQUARE");
        return is;
    }
    
    bool operator ==(const Square& other) const {
        return abs(length() - other.length()) < EPS;
    }
    
    explicit operator double() const override {
        return static_cast<double>(area());
    }
};

class Rectangle final: public Figure {
    virtual bool check() const override {
        return !(abs(vertices[0].distance(vertices[1]) - vertices[2].distance(vertices[3])) > EPS
            or abs(vertices[0].distance(vertices[2]) - vertices[1].distance(vertices[3])) > EPS);
    }
    
public:
    Rectangle(): Figure() {
        n = 4;
        vertices = nullptr;
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
    
    void add_points(const Point& a, const Point& b, const Point& c, const Point& d) {
        vertices = new Point[4];
        vertices[0] = a;
        vertices[1] = b;
        vertices[2] = c;
        vertices[3] = d;
        if(!check())
            throw invalid_argument("IMPOSSIBLE_RECTANGLE");
    }
    
    friend istream& operator >>(istream& is, Rectangle& f) {
        if(f.vertices != nullptr)
            delete[] f.vertices;
        f.vertices = new Point[f.n];
        for (size_t i = 0; i < f.n; ++i)
            is >> f.vertices[i];
        if(!f.check())
            throw invalid_argument("IMPOSSIBLE_RECTANGLE");
        return is;
    }
    
    bool operator ==(const Rectangle& other) const {
        return abs(length() - other.length()) < EPS and abs(width() - other.width()) < EPS;
    }
    
    explicit operator double() const override {
        return static_cast<double>(area());
    }
};

class Trapezoid final: public Figure {
    virtual bool check() const override {
        return !((parallel(vertices[0], vertices[1], vertices[2], vertices[3]) 
            and not parallel(vertices[0], vertices[2], vertices[1], vertices[3]))
            or (parallel(vertices[0], vertices[2], vertices[1], vertices[3]) 
            and not parallel(vertices[0], vertices[1], vertices[2], vertices[3])));
    }
    
public:
    Trapezoid(): Figure() {
        n = 4;
        vertices = nullptr;
    }
    
    float top() const {
        if(parallel(vertices[0], vertices[1], vertices[2], vertices[3]))
            return min(vertices[0].distance(vertices[1]), vertices[2].distance(vertices[3]));
        return min(vertices[1].distance(vertices[2]), vertices[3].distance(vertices[0]));    
    }
    
    float bottom() const {
        if(parallel(vertices[0], vertices[1], vertices[2], vertices[3]))
            return max(vertices[0].distance(vertices[1]), vertices[2].distance(vertices[3]));
        return max(vertices[1].distance(vertices[2]), vertices[3].distance(vertices[0]));
    }
    
    float height() const {
        return area() * 2 / (top() + bottom());
    }
    
    void add_points(const Point& a, const Point& b, const Point& c, const Point& d) {
        vertices = new Point[4];
        vertices[0] = a;
        vertices[1] = b;
        vertices[2] = c;
        vertices[3] = d;
        if(!check())
            throw invalid_argument("IMPOSSIBLE_TRAPEZOID");
    }
    
    friend istream& operator >>(istream& is, Trapezoid& f) {
        if(f.vertices != nullptr)
            delete[] f.vertices;
        f.vertices = new Point[f.n];
        for (size_t i = 0; i < f.n; ++i)
            is >> f.vertices[i];
        if(!f.check())
            throw invalid_argument("IMPOSSIBLE_TRAPEZOID");
        return is;
    }
    
    bool operator ==(const Trapezoid& other) const {
        return abs(perimeter() + height() - (other.perimeter() + other.height())) < RUDE_EPS;
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
    Point p;
    cin >> p;
    f.add_point(p);
    cout << f;
    
    cout << "\nSquare!\n";
    Square s;
    Point a(0, 0), b(0, 2), c(2, 2), d(2, 0);
    s.add_points(a, b, c, d);
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
