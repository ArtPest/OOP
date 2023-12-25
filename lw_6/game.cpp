#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <cmath>
#include <memory>

using namespace std;

enum class npc_type { knight, squirrel, pegasus };
enum class npc_status { dead, alive };

class npc {
protected:
    int x, y;
    string name;
    npc_type type;
    npc_status status = npc_status::alive;

public:
    npc(const string& _name, npc_type _type, int _x, int _y) : name(_name), type(_type), x(_x), y(_y) {}
    virtual ~npc() = default;

    string get_name() const { return name; }
    npc_type get_type() const { return type; }
    int get_x() const { return x; }
    int get_y() const { return y; }
    npc_status get_status() const { return status; }

    virtual npc_type get_enemy_type() const = 0;

    void set_status(npc_status effect) {
        status = effect;
    }

    int sqr_distance(const npc& other) const {
        return pow(x - other.x, 2) + pow(y - other.y, 2);
    }

    void capture(npc& other, int radius) {
        if ((other.get_type() == get_enemy_type()) && sqr_distance(other) <= pow(radius, 2))
            other.set_status(npc_status::dead);
    }

    friend ostream& operator<<(ostream& os, const npc& piece) {
        os << "Name: " << piece.name << '\n';
        os << "Type: " << static_cast<int>(piece.type) << '\n';
        os << "Coordinates: (" << piece.x << ", " << piece.y << ")" << '\n';
        return os;
    }
};

class Knight : public npc {
public:
    Knight(const string& name, int x, int y) : npc(name, npc_type::knight, x, y) {}
    npc_type get_enemy_type() const override { return npc_type::squirrel; }
};

class Squirrel : public npc {
public:
    Squirrel(const string& name, int x, int y) : npc(name, npc_type::squirrel, x, y) {}
    npc_type get_enemy_type() const override { return npc_type::pegasus; }
};

class Pegasus : public npc {
public:
    Pegasus(const string& name, int x, int y) : npc(name, npc_type::pegasus, x, y) {}
    npc_type get_enemy_type() const override { return npc_type::knight; }
};

class npc_factory {
public:
    static unique_ptr<npc> create_npc(const string& name, npc_type type, int x, int y) {
        switch (type) {
            case npc_type::knight: return make_unique<Knight>(name, x, y);
            case npc_type::squirrel: return make_unique<Squirrel>(name, x, y);
            case npc_type::pegasus: return make_unique<Pegasus>(name, x, y);
            default: throw invalid_argument("Unknown NPC type");
        }
    }
};

class Board {
    int n, m;
    vector<unique_ptr<npc>> pieces;
    set<string> captured;

public:
    Board(int _n, int _m) : n(_n), m(_m) {}

    void cycle(int radius) {
        for (size_t i = 0; i < pieces.size(); ++i) {
            for (size_t j = 0; j < pieces.size(); ++j) {
                if (i != j) pieces[i]->capture(*pieces[j], radius);
            }
        }

        for (size_t i = 0; i < pieces.size();) {
            if (pieces[i]->get_status() == npc_status::dead) {
                captured.insert(pieces[i]->get_name());
                pieces.erase(pieces.begin() + i);
            } else {
                ++i;
            }
        }
    }

    void get_info() {
        cout << "Now alive:\n\n";
        for (const auto& piece : pieces) {
            cout << *piece << '\n';
        }
    }

    void add_npc(unique_ptr<npc> piece) {
        if(piece->get_x() >= n || piece->get_y() >= m || captured.find(piece->get_name()) != captured.end())
            throw invalid_argument("IMPOSSIBLE_PIECE");
        pieces.push_back(std::move(piece));
    }
};

int main() {
    Board board(8, 8);
    board.add_npc(npc_factory::create_npc("Arthur", npc_type::knight, 0, 0));
    board.add_npc(npc_factory::create_npc("Squishy", npc_type::squirrel, 3, 4));
    board.add_npc(npc_factory::create_npc("Cloud", npc_type::pegasus, 5, 5));
    board.cycle(5);
    board.get_info();
    
    return 0;
}
