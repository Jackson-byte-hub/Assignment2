#include "dragon.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
/// POSITION
////////////////////////////////////////////////////////////////////////

const Position Position::npos = Position(-1, -1); // Required by spec

Position::Position(int r, int c) : r_(r), c_(c) {
}

Position::Position(const string &str_pos) {
    int r = 0, c = 0;
    sscanf(str_pos.c_str(), "(%d,%d)", &r, &c);
    r_ = r;
    c_ = c;
}

// Getter
int Position::getRow() const {
    return r_;
}

int Position::getCol() const {
    return c_;
}

// Setter
void Position::setRow(int r) {
    r_ = r;
}

void Position::setCol(int c) {
    c_ = c;
}

// str method
string Position::str() const {
    return "(" + to_string(r_) + "," + to_string(c_) + ")";
}

// isEqual method
bool Position::isEqual(int in_r, int in_c) const {
    return (r_ == in_r && c_ == in_c);
}
bool Position::operator==(const Position& other) const {
    return r_ == other.r_ && c_ == other.c_;
}

bool Position::operator!=(const Position& other) const {
    return !(*this == other);
}
////////////////////////////////////////////////////////////////////////
/// MAP ELEMENT
////////////////////////////////////////////////////////////////////////

MapElement::MapElement(ElementType t, int r) {
    type = t;

    if (t == GROUND_OBSTACLE) {
        // r here is treated as encoded (see Map class will pass calculated req_dmg)
        req_dmg = r;
    } else {
        req_dmg = 0;
    }
}

MapElement::~MapElement() = default;

ElementType MapElement::getType() const {
    return type;
}

int MapElement::getReqDmg() const {
    return req_dmg;
}

////////////////////////////////////////////////////////////////////////
/// MAP
////////////////////////////////////////////////////////////////////////

Map::Map(int rows, int cols, int num_obst, Position *obst, int num_gro_obst, Position *gro_obst) {
    num_rows = rows;
    num_cols = cols;

    // Allocate grid
    grid = new MapElement**[num_rows];
    for (int i = 0; i < num_rows; ++i) {
        grid[i] = new MapElement*[num_cols];
        for (int j = 0; j < num_cols; ++j) {
            grid[i][j] = new MapElement(PATH);
        }
    }

    // Set OBSTACLES
    for (int i = 0; i < num_obst; ++i) {
        int r = obst[i].getRow();
        int c = obst[i].getCol();
        if (r >= 0 && r < num_rows && c >= 0 && c < num_cols) {
            delete grid[r][c];
            grid[r][c] = new MapElement(OBSTACLE);
        }
    }

    // Set GROUND_OBSTACLES with damage requirements
    for (int i = 0; i < num_gro_obst; ++i) {
        int r = gro_obst[i].getRow();
        int c = gro_obst[i].getCol();
        int req_dmg = (r * 257 + c * 139 + 89) % 900 + 1;

        if (r >= 0 && r < num_rows && c >= 0 && c < num_cols) {
            delete grid[r][c];
            grid[r][c] = new MapElement(GROUND_OBSTACLE, req_dmg);
        }
    }
}

Map::~Map() {
    for (int i = 0; i < num_rows; ++i) {
        for (int j = 0; j < num_cols; ++j) {
            delete grid[i][j];
        }
        delete[] grid[i];
    }
    delete[] grid;
}

int Map::getNumRows() const {
    return num_rows;
}

int Map::getNumCols() const {
    return num_cols;
}

bool Map::isValid(const Position &pos, MovingObject *mv_obj) const {
    int r = pos.getRow();
    int c = pos.getCol();

    if (r < 0 || r >= num_rows || c < 0 || c >= num_cols) return false;

    MapElement* cell = grid[r][c];

    ElementType type = cell->getType();

    if (type == OBSTACLE) return false;

    if (type == PATH) return true;

    if (type == GROUND_OBSTACLE) {
        // Check object type
        if (dynamic_cast<class FlyTeam*>(mv_obj)) return true;

        if (auto ground = dynamic_cast<class GroundTeam*>(mv_obj)) {
            // Access protected damage value via friend or getter (to be implemented in GroundTeam)
            // TEMPORARY: assume getDamage() exists
            return ground->getDamage() >= cell->getReqDmg();
        }

        return false; // others can't move through it
    }

    return false; // unknown type
}

////////////////////////////////////////////////////////////////////////
/// MOVING OBJECT
////////////////////////////////////////////////////////////////////////

MovingObject::MovingObject(int index, const Position &pos, Map *map, const string &name)
: index(index), pos(pos), map(map), name(name) {}

MovingObject::~MovingObject() = default;

const Position &MovingObject::getCurrentPosition() const {
    return pos;
}

////////////////////////////////////////////////////////////////////////
/// Warrior (Base Class)
////////////////////////////////////////////////////////////////////////

Warrior::Warrior(int index, const Position &pos, Map *map, const string &name, int hp, int damage)
: MovingObject(index, pos, map, name) {
    this->hp = max(0, min(hp, 500));       // HP [0, 500]
    this->damage = max(0, min(damage, 900)); // DMG [0, 900]
    this->bag = nullptr; // Bag will be assigned later
}

Warrior::~Warrior() {
    delete bag;  // Clean up memory
}

int Warrior::getHP() const {
    return hp;
}

void Warrior::setHP(int hp) {
    this->hp = max(0, min(hp, 500));
}

int Warrior::getDamage() const {
    return damage;
}

void Warrior::setDamage(int dmg) {
    this->damage = max(0, min(dmg, 900));
}

BaseBag* Warrior::getBag() const {
    return bag;
}

////////////////////////////////////////////////////////////////////////
/// FlyTeam
////////////////////////////////////////////////////////////////////////

FlyTeam::FlyTeam(int index, const string& moving_rule, const Position& pos, Map* map, int init_hp, int init_damage)
: Warrior(index, pos, map, "FLY", init_hp, init_damage), moving_rule(moving_rule), moving_index(0) {}

Position FlyTeam::getNextPosition() {
    if (moving_rule.empty()) return Position::npos;

    char dir = moving_rule[moving_index];
    int r = pos.getRow();
    int c = pos.getCol();
    Position next;

    switch (dir) {
        case 'L': next = Position(r, c - 1); break;
        case 'R': next = Position(r, c + 1); break;
        case 'U': next = Position(r - 1, c); break;
        case 'D': next = Position(r + 1, c); break;
        default:  next = Position::npos; break; // invalid input
    }

    // Update index circularly
    moving_index = (moving_index + 1) % moving_rule.length();

    if (map->isValid(next, this))
        return next;
    return Position::npos;
}

void FlyTeam::move() {
    Position next = getNextPosition();
    if (next != Position::npos) {
        pos = next;
    }
}

string FlyTeam::str() const {
    return "FlyTeam[index=" + to_string(index) +
           ";pos=" + pos.str() +
           ";HP=" + to_string(hp) +
           ";DMG=" + to_string(damage) +
           ";rule=" + moving_rule +
           ";idx=" + to_string(moving_index) + "]";
}

////////////////////////////////////////////////////////////////////////
/// GroundTeam
////////////////////////////////////////////////////////////////////////

GroundTeam::GroundTeam(int index, const string& moving_rule, const Position& pos, Map* map, int init_hp, int init_damage)
: Warrior(index, pos, map, "GND", init_hp, init_damage), moving_rule(moving_rule), moving_index(0) {}

Position GroundTeam::getNextPosition() {
    if (moving_rule.empty()) return Position::npos;

    char dir = moving_rule[moving_index];
    int r = pos.getRow();
    int c = pos.getCol();
    Position next;

    switch (dir) {
        case 'L': next = Position(r, c - 1); break;
        case 'R': next = Position(r, c + 1); break;
        case 'U': next = Position(r - 1, c); break;
        case 'D': next = Position(r + 1, c); break;
        default:  next = Position::npos; break;
    }

    // Circular update of index
    moving_index = (moving_index + 1) % moving_rule.length();

    if (map->isValid(next, this))
        return next;
    return Position::npos;
}

void GroundTeam::move() {
    Position next = getNextPosition();
    if (next != Position::npos) {
        pos = next;
    }
}

string GroundTeam::str() const {
    return "GroundTeam[index=" + to_string(index) +
           ";pos=" + pos.str() +
           ";HP=" + to_string(hp) +
           ";DMG=" + to_string(damage) +
           ";rule=" + moving_rule +
           ";idx=" + to_string(moving_index) + "]";
}
bool GroundTeam::trap(DragonLord* dragon) {
    // Check if on same tile
    if (dragon == nullptr) return false;

    if (pos == dragon->getCurrentPosition()) {
        if (trap_turns > 0) {
            // Trap successful
            trap_turns--;
            return true;
        }
    }

    return false;
}
// getter and setter
int GroundTeam::getTrapTurns() const {
    return trap_turns;
}

void GroundTeam::setTrapTurns(int turns) {
    trap_turns = max(0, turns);
}

////////////////////////////////////////////////////////////////////////
/// DragonLord (Rule-Based Movement)
////////////////////////////////////////////////////////////////////////

DragonLord::DragonLord(int index, const Position& init_pos, Map* map,
                       FlyTeam* flyteam1, FlyTeam* flyteam2, GroundTeam* groundteam)
: MovingObject(index, init_pos, map, "DragonLord"),
  flyteam1(flyteam1), flyteam2(flyteam2), groundteam(groundteam),
  trapped_counter(0)
{
    hp = 5000;
}

Position DragonLord::getNextPosition() {
    if (trapped_counter > 0 || !flyteam1 || !flyteam2) {
        return Position::npos;
    }

    Position pos1 = flyteam1->getCurrentPosition();
    Position pos2 = flyteam2->getCurrentPosition();

    int x1 = pos1.getRow(), y1 = pos1.getCol();
    int x2 = pos2.getRow(), y2 = pos2.getCol();

    int x = abs(x1 - x2);
    int y = abs(y1 - y2);

    int d1 = abs(x - x1) + abs(y - y1); // Manhattan to FlyTeam1
    int d2 = abs(x - x2) + abs(y - y2); // Manhattan to FlyTeam2

    Position newDLpos;

    if (d1 > 5 && d2 > 5) {
        newDLpos = Position(x, y);
    } else {
        newDLpos = Position(y, x); // Swap
    }

    // Must check map validity
    return map->isValid(newDLpos, this) ? newDLpos : Position::npos;
}

void DragonLord::move() {
    if (trapped_counter > 0) {
        trapped_counter--;
        return;
    }

    Position next = getNextPosition();

    if (next != Position::npos) {
        pos = next;
    }
}

void DragonLord::setTrapped(int turns) {
    trapped_counter = max(trapped_counter, turns);
}

void DragonLord::setHP(int new_hp) {
    hp = max(0, min(new_hp, 5000));
}

int DragonLord::getHP() const {
    return hp;
}

string DragonLord::str() const {
    return "DragonLord[index=" + to_string(index) +
           ";pos=" + pos.str() + "]";
}

////////////////////////////////////////////////////////////////////////
/// ArrayMovingObject
////////////////////////////////////////////////////////////////////////

ArrayMovingObject::ArrayMovingObject(int capacity)
    : capacity(capacity), count(0) {
    arr_mv_objs = new MovingObject*[capacity];
}

ArrayMovingObject::~ArrayMovingObject() {
    for (int i = 0; i < count; ++i) {
        delete arr_mv_objs[i]; // delete each MovingObject
    }
    delete[] arr_mv_objs; // delete array of pointers
}

bool ArrayMovingObject::isFull() const {
    return count == capacity;
}

bool ArrayMovingObject::add(MovingObject* mv_obj) {
    if (isFull()) return false;
    arr_mv_objs[count++] = mv_obj;
    return true;
}

string ArrayMovingObject::str() const {
    string result = "ArrayMovingObject[count=" + to_string(count) +
                    ";capacity=" + to_string(capacity) + ";";

    for (int i = 0; i < count; ++i) {
        result += arr_mv_objs[i]->str();
        if (i != count - 1) result += "; ";
    }
    result += "]";
    return result;
}



////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////