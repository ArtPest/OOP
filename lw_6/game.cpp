#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <algorithm>
#include <memory>
#include <list>
#include <sstream>

using namespace std;

enum class npc_type { knight, squirrel, pegasus };
enum class npc_status { alive, beaten, dead };

ostream& operator <<(ostream& os, const npc_type& type) {
    switch(type) {
        case npc_type::knight: return os << "knight";
        case npc_type::squirrel: return os << "squirrel";
        case npc_type::pegasus: return os << "pegasus";
        default: throw invalid_argument("IMPOSSIBLE_TYPE");
    }
}

class observer {
public:
    virtual ~observer() = default;
    virtual void update(const string& message) = 0;
};

class subject {
public:
    virtual ~subject() = default;
    virtual void attach(observer* o) = 0;
    virtual void detach(observer* o) = 0;
    virtual void notify(const string& message) = 0;
};

class terminal_observer: public observer {
public:
    void update(const string& message) override {
        cout << message;
    }
};

class file_observer : public observer {
private:
    ofstream log_file;

public:
    file_observer(const string& filename) {
        log_file.open(filename, ios_base::app);
    }

    ~file_observer() {
        if (log_file.is_open())
            log_file.close();
    }

    void update(const string& message) override {
        if (log_file.is_open())
            log_file << message;
    }
};

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
    
    void move(int _x, int _y) {
        x = _x;
        y = _y;
    }

    static int sqr_distance(const npc& a, const npc& b) {
        return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
    }

    friend ostream& operator <<(ostream& os, const npc& piece) {
        os << "Name: " << piece.name << '\n';
        os << "Type: " << piece.type << '\n';
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
            default: throw invalid_argument("IMPOSSIBLE_TYPE");
        }
    }
};

class fight_visitor;

class board: public subject {
    int n, m;
    vector<unique_ptr<npc>> pieces;
    set<string> captured;
    list<observer*> observers;

public:
    board(int _n, int _m) : n(_n), m(_m) {}
    
    void cycle(int radius);
    
    void get_info() const {
        cout << "\nNow alive:\n";
        for (const auto& piece : pieces)
            cout << *piece << '\n';
    }
    
    void add_npc(unique_ptr<npc> piece) {
        if (piece->get_x() >= n || piece->get_y() >= m ||
            piece->get_x() < 0 || piece->get_y() < 0 ||
            captured.find(piece->get_name()) != captured.end())
            throw invalid_argument("IMPOSSIBLE_PIECE");
        pieces.push_back(move(piece));
    }
    
    void move_npc(const string& npc_name, int new_x, int new_y) {
        if (new_x < 0 || new_x >= n || new_y < 0 || new_y >= m) 
            throw invalid_argument("IMPOSSIBLE_MOVEMENT");
        for (auto& piece: pieces) {
            if (piece->get_name() == npc_name && piece->get_status() == npc_status::alive) {
                if (captured.find(npc_name) != captured.end())
                    throw invalid_argument("DEAD_MOVEMENT");
                ostringstream message;
                message << piece->get_name() << " moves from (" << piece->get_x() << ", " << piece->get_y()
                        << ") to (" << new_x << ", " << new_y << ").\n";
                piece->move(new_x, new_y);
                notify(message.str());
                return;
            }
        }
        throw invalid_argument("NAME_ERROR");
    }

    auto& get_pieces() { return pieces; }
    
        void attach(observer* o) override {
        observers.push_back(o);
    }

    void detach(observer* o) override {
        observers.remove(o);
    }

    void notify(const string& message) override {
        for (observer* i: observers)
            i->update(message);
    }
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
                piece->set_status(npc_status::beaten);    
                to_remove.push_back(piece->get_name());
            }
    }
};

void board::cycle(int radius) {
    fight_visitor visitor(*this, radius);
    
    for (auto& piece : pieces)
        if (piece->get_status() != npc_status::dead)
            piece->accept(visitor);
    
    ostringstream message;
    
    for (auto it = pieces.begin(); it != pieces.end(); ++it) {
        auto& piece = *it;
        if (piece->get_status() == npc_status::beaten) {
            message << piece->get_name() << " has been killed.\n";
            piece->set_status(npc_status::dead);
        }
    }
    
    visitor.remove_defeated();
    
    message << "\nAfter the battle:\n";
    for (const auto& piece : pieces)
        message << *piece << '\n';
        
    notify(message.str());
}

int main() {
    board game(8, 8);
    terminal_observer terminal_record;
    file_observer file_record("log.txt");
    game.attach(&terminal_record);
    game.attach(&file_record);
    game.add_npc(npc_factory::create_npc("Arthur", npc_type::knight, 0, 0));
    game.add_npc(npc_factory::create_npc("Squishy", npc_type::squirrel, 3, 4));
    game.add_npc(npc_factory::create_npc("Cloud", npc_type::pegasus, 5, 5));
    game.move_npc("Arthur", 1, 1);
    game.get_info();
    game.cycle(5);
    game.detach(&terminal_record);
    game.detach(&file_record);
    return 0;
}
