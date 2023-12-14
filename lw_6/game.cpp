#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

class NPC {
    int x, y;
    string name, type, enemy = "default";
    bool status = true;
    
public:
    NPC() = default;
    NPC(const string& _name, const string& _type, int _x, int _y): name(_name), type(_type), x(_x), y(_y) {
        if(type == "Knight")
            enemy = "Squirrel";
        else if(type == "Squirrel")
            enemy = "Pegasus";
    }
    ~NPC() = default;
    
    string get_name() const { return name; }
    string get_type() const { return type; }
    int get_x() const { return x; }
    int get_y() const { return y; }
    bool get_status() const { return status; }
    
    void set_status(bool effect) {
        status = effect;
    }
    
    int sqr_distance(const NPC& other) {
        return (x - other.x) * (x - other.x) + (y - other.y) * (y - other.y);
    }
    
    void attack(NPC& other, int radius) {
        if((other.get_type() == enemy) and sqr_distance(other) <= radius * radius)
            other.set_status(false);
    }
    
    friend ostream& operator<<(ostream& os, const NPC& npc) {
        os << "Name: " << npc.name << '\n';
        os << "Type: " << npc.type << '\n';
        os << "Coordinates: (" << npc.x << ", " << npc.y << ")" << '\n';
        return os;
    }
};

class Board {
    int n, m;
    vector<NPC> figures;
    
public:
    Board() = default;
    Board(int _n, int _m): n(_n), m(_m) {}
    ~Board() = default;
    
    void add_npc (const string& _name, const string& _type, int _x, int _y) {
        figures.emplace_back(_name, _type, _x, _y);
    }
    
    void cycle(int radius) {
        for(size_t i = 0; i < figures.size() - 1; ++i)
            for(size_t j = i + 1; j < figures.size(); ++j) {
                figures[i].attack(figures[j], radius);
                figures[j].attack(figures[i], radius);
            }
        for(size_t i = 0; i < figures.size(); ++i)
            if(figures[i].get_status() == false)
                figures.erase(figures.begin() + i--);
    }
    
    void get_info() {
        cout << "Now alive:\n\n";
        for(size_t i = 0; i < figures.size(); ++i)
            cout << figures[i] << '\n';
    }
};

int main() {
    Board board(10, 10);
    board.add_npc("Arthur", "Knight", 0, 0);
    board.add_npc("Percy", "Squirrel", 3, 4);
    board.cycle(5);
    board.get_info();
    return 0;
}
