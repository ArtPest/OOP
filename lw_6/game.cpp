#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

class npc;

class npc_factory {
public:
    static npc create_npc(const string& name, const string& type, int x, int y);
};

class npc {
    int x, y;
    string name, type, enemy = "default";
    bool status = true;
    
public:
    npc() = default;
    npc(const string& _name, const string& _type, int _x, int _y): name(_name), type(_type), x(_x), y(_y) {
        if(type == "Knight")
            enemy = "Squirrel";
        else if(type == "Squirrel")
            enemy = "Pegasus";
    }
    ~npc() = default;
    
    string get_name() const { return name; }
    string get_type() const { return type; }
    int get_x() const { return x; }
    int get_y() const { return y; }
    bool get_status() const { return status; }
    
    void set_status(bool effect) {
        status = effect;
    }
    
    int sqr_distance(const npc& other) const {
        return (x - other.x) * (x - other.x) + (y - other.y) * (y - other.y);
    }
    
    void capture(npc& other, int radius) {
        if((other.get_type() == enemy) and sqr_distance(other) <= radius * radius)
            other.set_status(false);
    }
    
    friend ostream& operator <<(ostream& os, const npc& piece) {
        os << "Name: " << piece.name << '\n';
        os << "Type: " << piece.type << '\n';
        os << "Coordinates: (" << piece.x << ", " << piece.y << ")" << '\n';
        return os;
    }
    
    friend npc npc_factory::create_npc(const string& name, const string& type, int x, int y);
};

npc npc_factory::create_npc(const string& name, const string& type, int x, int y) {
    return npc(name, type, x, y);
}

class Board {
    int n, m;
    vector<npc> pieces;
    
public:
    Board() = default;
    Board(int _n, int _m): n(_n), m(_m) {}
    ~Board() = default;
    
    void cycle(int radius) {
        for(size_t i = 0; i < pieces.size() - 1; ++i)
            for(size_t j = i + 1; j < pieces.size(); ++j) {
                pieces[i].capture(pieces[j], radius);
                pieces[j].capture(pieces[i], radius);
            }
        for(size_t i = 0; i < pieces.size(); ++i)
            if(pieces[i].get_status() == false)
                pieces.erase(pieces.begin() + i--);
    }
    
    void get_info() {
        cout << "Now alive:\n\n";
        for(npc piece: pieces)
            cout << piece << '\n';
    }
    
    void add_npc(const npc& piece) {
        if(piece.get_x() > n or piece.get_y() > m or !(piece.get_name() != "Knight" and 
            piece.get_name() != "Squirrel" and piece.get_name() != "Pegasus"))
            throw invalid_argument("IMPOSSIBLE_PIECE");
        pieces.push_back(piece);
    }
};

int main() {
    Board board(8, 8);
    board.add_npc(npc_factory::create_npc("Arthur", "Knight", 0, 0));
    board.add_npc(npc_factory::create_npc("Squishy", "Squirrel", 3, 4));
    board.cycle(5);
    board.get_info();
    return 0;
}
