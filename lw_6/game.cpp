#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <algorithm>
#include <memory>

using namespace std;

enum class npc_type { knight, squirrel, pegasus };
enum class npc_status { dead, alive };

class Knight;
class Squirrel;
class Pegasus;

class npc_visitor {
public:
    virtual void visit(Knight& knight) = 0;
    virtual void visit(Squirrel& squirrel) = 0;
    virtual void visit(Pegasus& pegasus) = 0;
};

class npc {
protected:
    int x, y;
    string name;
    npc_type type;
    npc_status status = npc_status::alive;

public:
    npc(const string& _name, npc_type _type, int _x, int _y) : name(_name), type(_type), x(_x), y(_y) {}
    virtual ~npc() = default;

    virtual void accept(npc_visitor& visitor) = 0;

    string get_name() const { return name; }
    npc_type get_type() const { return type; }
    int get_x() const { return x; }
    int get_y() const { return y; }
    npc_status get_status() const { return status; }

    virtual npc_type get_enemy_type() const = 0;

    void set_status(npc_status effect) { status = effect; }

    static int sqr_distance(const npc& a, const npc& b) {
        return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
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

    void accept(npc_visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Squirrel : public npc {
public:
    Squirrel(const string& name, int x, int y) : npc(name, npc_type::squirrel, x, y) {}
    npc_type get_enemy_type() const override { return npc_type::pegasus; }

    void accept(npc_visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Pegasus : public npc {
public:
    Pegasus(const string& name, int x, int y) : npc(name, npc_type::pegasus, x, y) {}
    npc_type get_enemy_type() const override { return npc_type::knight; }

    void accept(npc_visitor& visitor) override {
        visitor.visit(*this);
    }
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

class fight_visitor;

class board {
    int n, m;
    vector<unique_ptr<npc>> pieces;
    set<string> captured;

public:
    board(int _n, int _m) : n(_n), m(_m) {}
    
    void cycle(int radius);
    
    void get_info() const {
        cout << "Now alive:\n\n";
        for (const auto& piece : pieces)
            cout << *piece << '\n';
    }

    void add_npc(unique_ptr<npc> piece) {
        if (piece->get_x() >= n || piece->get_y() >= m || captured.find(piece->get_name()) != captured.end())
            throw invalid_argument("IMPOSSIBLE_PIECE");
        pieces.push_back(move(piece));
    }

    int get_radius() const { return max(n, m); }

    auto& get_pieces() { return pieces; }
};

class fight_visitor : public npc_visitor {
    board& game;
    int radius;
    vector<string> to_remove;

public:
    fight_visitor(board& b, int r) : game(b), radius(r) {}

    void visit(Knight& knight) override {
        engage_in_battle(knight);
    }

    void visit(Squirrel& squirrel) override {
        engage_in_battle(squirrel);
    }

    void visit(Pegasus& pegasus) override {
        engage_in_battle(pegasus);
    }

    void remove_defeated() {
        for (auto& name : to_remove) {
            auto& pieces = game.get_pieces();
            pieces.erase(remove_if(pieces.begin(), pieces.end(), [&name](const unique_ptr<npc>& piece) {
                return piece->get_name() == name;
            }), pieces.end());
        }
    }

private:
    template <typename T>
    void engage_in_battle(T& aggressor) {
        for (auto& piece : game.get_pieces())
            if (piece->get_status() == npc_status::alive &&
                aggressor.get_enemy_type() == piece->get_type() && 
                npc::sqr_distance(aggressor, *piece) <= radius * radius) {
                to_remove.push_back(piece->get_name());
            }
    }
};

void board::cycle(int radius) {
    fight_visitor visitor(*this, radius);
    
    for (auto& piece : pieces)
        if (piece->get_status() == npc_status::alive)
            piece->accept(visitor);

    visitor.remove_defeated();
}

int main() {
    board game(8, 8);
    game.add_npc(npc_factory::create_npc("Arthur", npc_type::knight, 0, 0));
    game.add_npc(npc_factory::create_npc("Squishy", npc_type::squirrel, 3, 4));
    game.add_npc(npc_factory::create_npc("Cloud", npc_type::pegasus, 5, 5));
    cout << "Before the battle:\n";
    game.get_info();
    game.cycle(5);
    cout << "After the battle:\n";
    game.get_info();
    return 0;
}
