#include <iostream>

using namespace std;

class Figure{
protected:
    float x0, y0, area = -1;

public:
    Figure(float x, float y): x0(x), y0(y){}
    
    friend ostream& operator<<(std::ostream& os, const Figure& fig) {
        return os << "Figure center: (" << fig.x0 << ", " << fig.y0 << ")\nArea: " << fig.area << '\n';
    }
};

int main(){
    Figure a(0, 0);
    cout << a;
    return 0;
}
