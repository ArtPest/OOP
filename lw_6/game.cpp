#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<set>
#include<cmath>

using namespace std;

enum class npc_type { knight, squirrel, pegasus };
enum class npc_status { dead, alive };

class npc;

class npc_factory {
public:
    static npc create_npc(const string& name, npc_type type, int x, int y);
};

class npc {
    int x, y;
    string name;
    npc_type type, enemy;
    npc_status status = npc_status::alive;

public:
    npc() = default;
    npc(const string& _name, npc_type _type, int _x, int _y) : name(_name), type(_type), x(_x), y(_y) {
        if (type == npc_type::knight)
            enemy = npc_type::squirrel;
        else if (type == npc_type::squirrel)
            enemy = npc_type::pegasus;
    }
    ~npc() = default;

    string get_name() const { return name; }
    npc_type get_type() const { return type; }
    int get_x() const { return x; }
    int get_y() const { return y; }
    npc_status get_status() const { return status; }

    void set_status(npc_status effect) {
        status = effect;
    }

    int sqr_distance(const npc& other) const {
        return (x - other.x) * (x - other.x) + (y - other.y) * (y - other.y);
    }

    void capture(npc& other, int radius) {
        if ((other.get_type() == enemy) and sqr_distance(other) <= radius * radius)
            other.set_status(npc_status::dead);
    }

    friend ostream& operator <<(ostream& os, const npc& piece) {
        os << "Name: " << piece.name << '\n';
        os << "Type: " << static_cast<int>(piece.type) << '\n';
        os << "Coordinates: (" << piece.x << ", " << piece.y << ")" << '\n';
        return os;
    }

    friend npc npc_factory::create_npc(const string& name, npc_type type, int x, int y);
};

npc npc_factory::create_npc(const string& name, npc_type type, int x, int y) {
    return npc(name, type, x, y);
}

class Board {
    int n, m;
    vector<npc> pieces;
    set<string> captured;

public:
    Board() = default;
    Board(int _n, int _m) : n(_n), m(_m) {}
    ~Board() = default;

    void cycle(int radius) {
        for(size_t i = 0; i < pieces.size() - 1; ++i)
            for(size_t j = i + 1; j < pieces.size(); ++j) {
                pieces[i].capture(pieces[j], radius);
                pieces[j].capture(pieces[i], radius);
            }
        for(size_t i = 0; i < pieces.size(); ++i)
            if(pieces[i].get_status() == npc_status::dead) {
                captured.insert(pieces[i].get_name());
                pieces.erase(pieces.begin() + i--);
            }
    }

    void get_info() {
        cout << "Now alive:\n\n";
        for(npc piece: pieces)
            cout << piece << '\n';
    }

    void add_npc(const npc& piece) {
        if(piece.get_x() >= n || piece.get_y() >= m || 
        !(piece.get_type() != npc_type::knight 
        || piece.get_type() != npc_type::squirrel 
        || piece.get_type() != npc_type::pegasus)
        || captured.find(piece.get_name()) != captured.end())
            throw invalid_argument("IMPOSSIBLE_PIECE");
        pieces.push_back(piece);
    }
};

int main() {
    Board board(8, 8);
    board.add_npc(npc_factory::create_npc("Arthur", npc_type::knight, 0, 0));
    board.add_npc(npc_factory::create_npc("Squishy", npc_type::squirrel, 3, 4));
    board.cycle(5);
    board.get_info();
    return 0;
}
