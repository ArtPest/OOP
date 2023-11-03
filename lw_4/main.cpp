#include <iostream>
#include <sstream>
#include <cmath>
#include <algorithm>

#define EPS 1e-6
#define RUDE_EPS 0.1

using namespace std;

template <typename T>
struct Point {
    T x, y;

    Point(T a = 0.0, T b = 0.0) : x(a), y(b) {}

    Point& operator +=(const Point& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    Point operator -(const Point& other) {
        Point result(x - other.x, y - other.y);
        return result;
    }

    Point& operator /=(T f) {
        if(fabs(f) <= EPS)
            throw invalid_argument("ZERO_DIVISION");
        else {
            x /= f;
            y /= f;
        }
        return *this;
    }
    
    bool operator ==(const Point& other) {
        if(x == other.x and y == other.y)
            return true;
        return false;
    }
    
    bool operator !=(const Point& other) {
        if(x == other.x and y == other.y)
            return false;
        return true;
    }

    T distance(const Point& other) const {
        return sqrt((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y));
    }

    friend ostream& operator <<(ostream& os, const Point& p) {
        return os << "(" << p.x << ", " << p.y << ")\n";
    }

    friend istream& operator >>(istream& is, Point& p) {
        return is >> p.x >> p.y;
    }
};

template <typename T>
bool parallel(const Point<T>& p1, const Point<T>& p2, const Point<T>& p3, const Point<T>& p4) {
    if (abs(p1.x - p2.x) < EPS && abs(p3.x - p4.x) < EPS)
        return true;
    else if (abs(p1.x - p2.x) < EPS || abs(p3.x - p4.x) < EPS)
        return false;
    return abs((p2.y - p1.y) / (p2.x - p1.x) - (p4.y - p3.y) / (p4.x - p3.x)) < EPS;
}

template <typename T>
class Figure {
protected:
    int n = -1;
    Point<T>* vertices;

    virtual bool check() const {
        return !(area() <= 0.0);
    }

public:
    Figure() = default;
    
    Figure(int _n): n(_n) {vertices = new Point<T>[_n];}

    Figure(const Figure& other): n(other.n) {
        vertices = new Point<T>[n];
        for (size_t i = 0; i < n; ++i)
            vertices[i] = other.vertices[i];
    }

    Figure& operator =(const Figure& other) {
        if (this == &other)
            return *this;
        delete[] vertices;
        n = other.n;
        vertices = new Point<T>[n];
        for (size_t i = 0; i < n; ++i)
            vertices[i] = other.vertices[i];
        return *this;
    }

    Figure(Figure&& other) : n(other.n), vertices(other.vertices) {
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

    Point<T> center() const {
        Point<T> result;
        for (size_t i = 0; i < n; ++i)
            result += vertices[i];
        result /= n;
        return result;
    }

    virtual void add_point(const Point<T> p) {
        Point<T>* result = new Point<T>[n + 1];
        for (size_t i = 0; i < n; ++i)
            result[i] = vertices[i];
        result[n++] = p;
        delete[] vertices;
        vertices = result;
    }

    virtual T perimeter() const {
        T result = vertices[n - 1].distance(vertices[0]);
        for (size_t i = 0; i < n - 1; ++i)
            result += vertices[i].distance(vertices[i + 1]);
        return result;
    }

    virtual T area() const {
        auto tr_ar = [](const Point<T>& A, const Point<T>& B, const Point<T>& C) -> T {
            return 0.5 * abs(A.x * (B.y - C.y) + B.x * (C.y - A.y) + C.x * (A.y - B.y));
        };
        T result = 0.0;
        for (size_t i = 1; i < n - 1; ++i)
            result += tr_ar(vertices[0], vertices[i], vertices[i + 1]);
        return result;
    }

    virtual string get_info() {
        ostringstream os;
        os << n << ' ';
        for (size_t i = 0; i < n - 1; ++i)
            os << vertices[i].x << ' ' << vertices[i].y << ' ';
        os << vertices[n - 1].x << ' ' << vertices[n - 1].y;
        return os.str();
    }

    friend istream& operator >>(istream& is, Figure& f) {
        if (f.n == -1) {
            is >> f.n;
            if (f.n < 0)
                throw invalid_argument("IMPOSSIBLE_FIGURE");
        }
        if (f.n <= 0) {
            f.vertices = nullptr;
            return is;
        }
        if (f.vertices != nullptr)
            delete[] f.vertices;
        f.vertices = new Point<T>[f.n];
        for (size_t i = 0; i < f.n; ++i)
            is >> f.vertices[i];
        if (!f.check())
            throw invalid_argument("IMPOSSIBLE_FIGURE");
        return is;
    }

    friend ostream& operator <<(ostream& os, const Figure& f) {
        if (f.vertices != nullptr) {
            os << "Coordinates:\n";
            for (size_t i = 0; i < f.n; ++i)
                os << f.vertices[i];
            os << "Area: " << f.area() << '\n';
        }
        return os;
    }
    
    bool operator ==(const Figure& other) const {
        if (n != other.n)
            return false;
        for (size_t i = 0; i < n; ++i)
            if (vertices[i] != other.vertices[i])
                return false;
        return true;
    }
    
    virtual operator double() const {
        return static_cast<double>(this -> area());
    }
};

template <typename T>
class Square: public Figure<T> {
public:
    Square(): Figure<T>(4) {}

    T length() const {
        return this -> vertices[0].distance(this -> vertices[1]);
    }

    T area() const override {
        return length() * length();
    }

    void add_points(const Point<T>& a, const Point<T>& b, const Point<T>& c, const Point<T>& d) {
        this -> vertices[0] = a;
        this -> vertices[1] = b;
        this -> vertices[2] = c;
        this -> vertices[3] = d;
        this -> check();
    }

    friend istream& operator >>(istream& is, Square<T>& f) {
        T x, y;
        for(int i = 0; i < 4; ++i) {
            is >> x >> y;
            f.vertices[i] = Point<T>(x, y);
        }
        if(!f.check())
            throw invalid_argument("IMPOSSIBLE_SQUARE");
        return is;
    }

    bool operator ==(const Square<T>& other) const {
        return fabs(this -> length() - other.length()) < EPS;
    }

    explicit operator double() const override {
        return static_cast<double>(this -> area());
    }
};

template <typename T>
class Rectangle: public Figure<T> {
public:
    Rectangle(): Figure<T>(4) {}

    T length() const {
        return max(this -> vertices[0].distance(this->vertices[1]), this->vertices[0].distance(this->vertices[3]));
    }

    T width() const {
        return min(this -> vertices[0].distance(this->vertices[1]), this->vertices[0].distance(this->vertices[3]));
    }

    T area() const override {
        return length() * width();
    }

    void add_points(const Point<T>& a, const Point<T>& b, const Point<T>& c, const Point<T>& d) {
        this -> vertices[0] = a;
        this -> vertices[1] = b;
        this -> vertices[2] = c;
        this -> vertices[3] = d;
        this -> check();
    }

    friend istream& operator >>(istream& is, Rectangle<T>& f) {
        T x, y;
        for (int i = 0; i < 4; ++i) {
            is >> x >> y;
            f.vertices[i] = Point<T>(x, y);
        }
        if (!f.check())
            throw invalid_argument("IMPOSSIBLE_RECTANGLE");
        return is;
    }

    bool operator ==(const Rectangle<T>& other) const {
        return fabs(this -> length() - other.length()) < EPS && fabs(this -> width() - other.width()) < EPS;
    }

    explicit operator double() const override {
        return static_cast<double>(this -> area());
    }
};

template <typename T>
class Trapezoid: public Figure<T> {
public:
    Trapezoid(): Figure<T>(4) {}
    
    T top() const {
        if (parallel(vertices[0], vertices[1], vertices[2], vertices[3]))
            return min(this->vertices[0].distance(this->vertices[1]), this->vertices[2].distance(this->vertices[3]));
        return min(this->vertices[1].distance(this->vertices[2]), this->vertices[3].distance(this->vertices[0]));
    }

    T bottom() const {
        if (parallel(this->vertices[0], this->vertices[1], this->vertices[2], this->vertices[3]))
            return max(this->vertices[0].distance(this->vertices[1]), this->vertices[2].distance(this->vertices[3]));
        return max(this->vertices[1].distance(this->vertices[2]), this->vertices[3].distance(this->vertices[0]));
    }

    T height() const {
        return this -> area() * 2 / (top() + bottom());
    }

    void add_points(const Point<T>& a, const Point<T>& b, const Point<T>& c, const Point<T>& d) {
        this -> vertices[0] = a;
        this -> vertices[1] = b;
        this -> vertices[2] = c;
        this -> vertices[3] = d;
        this -> check();
    }

    friend istream& operator >>(istream& is, Trapezoid<T>& f) {
        T x, y;
        for (int i = 0; i < 4; ++i) {
            is >> x >> y;
            f.vertices[i] = Point<T>(x, y);
        }
        if (!f.check())
            throw invalid_argument("IMPOSSIBLE_TRAPEZOID");
        return is;
    }

    bool operator ==(const Trapezoid<T>& other) const {
        return fabs(this -> perimeter() + height() - (other.perimeter() + other.height())) < RUDE_EPS;
    }

    explicit operator double() const override {
        return static_cast<double>(this -> area());
    }
};

int main() {
    Figure<float> f;
    cout << "Figure!\n";
    cin >> f;
    cout << f;
    
    cout << "\nSquare!\n";
    Square<float> s;
    Point<float> a(0, 0), b(0, 2), c(2, 2), d(2, 0);
    s.add_points(a, b, c, d);
    cout << s;
    
    cout << "\nRectangle!\n";
    Rectangle<float> r;
    cin >> r;
    cout << r;
    
    cout << "\nTrapezoid!\n";
    Trapezoid<float> t_0;
    cin >> t_0;
    cout << t_0;
    
    cout << "\nAnother Trapezoid!\n";
    Trapezoid<float> t_1;
    cin >> t_1;
    cout << t_1;
    
    if(t_0 == t_1)
        cout << "\nTrapezoids are equal!";
    else
        cout << "\nTrapezoids are not equal!";
    return 0;
}
