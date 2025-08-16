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
    bag = new TeamBag(this,10);
}

Warrior::~Warrior() {
    delete bag;  // Clean memory
}

int Warrior::getHp() const {
    return hp;
}

void Warrior::setHp(int hp) {
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
    switch (dir) {
        case 'L': c--; break;
        case 'R': c++; break;
        case 'U': r--; break;
        case 'D': r++; break;
        default:  return Position::npos;
    }

    // Advance to next move in the rule (wrap around)
    moving_index = (moving_index + 1) % moving_rule.size();

    Position next(r, c);
    if (map && map->isValid(next, this))
        return next;

    return Position::npos;
}

void FlyTeam::move() {
    for (size_t i = 0; i < moving_rule.size(); i++) {
        Position next = getNextPosition();
        if (next != Position::npos) {
            pos = next;
        }
    }
}

string FlyTeam::str() const {
    if (index != 0) {
        return "FlyTeam" + to_string(index) +
               "[index=" + to_string(index) +
               ";pos=" + pos.str() +
               ";moving_rule=" + moving_rule + "]";
    } else {
        return "FlyTeam1[index=" + to_string(index) +
               ";pos=" + pos.str() +
               ";moving_rule=" + moving_rule + "]";
    }
}

bool FlyTeam::attack(DragonLord *dragon) {
    if (!dragon) return false;

    // Must be in same position to attack
    if (pos != dragon->getCurrentPosition()) {
        return false;
    }

    // Apply damage
    int remaining_hp = dragon->getHp() - this->getDamage();
    dragon->setHp(remaining_hp);

    // Return true if DragonLord is defeated
    return dragon->getHp() <= 0;
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
    switch (dir) {
        case 'L': c--; break;
        case 'R': c++; break;
        case 'U': r--; break;
        case 'D': r++; break;
        default:  return Position::npos;
    }

    Position next(r, c);

    // If valid → normal move
    if (map && map->isValid(next, this)) {
        moving_index = (moving_index + 1) % moving_rule.size();
        Groundteam_stuckCounter = 0; // reset stuck state
        lastStuckPattern.clear();
        return next;
    }

    // --- Blocked → try opposite ---
    cout << "MSG: GroundTeam at " << pos.str()
         << " got blocked when moving " << dir
         << " to " << next.str() << "\n";

    char opposite;
    switch (dir) {
        case 'L': opposite = 'R'; break;
        case 'R': opposite = 'L'; break;
        case 'U': opposite = 'D'; break;
        case 'D': opposite = 'U'; break;
        default: return Position::npos;
    }

    r = pos.getRow();
    c = pos.getCol();
    switch (opposite) {
        case 'L': c--; break;
        case 'R': c++; break;
        case 'U': r--; break;
        case 'D': r++; break;
    }
    Position reversed(r, c);

    if (map && map->isValid(reversed, this)) {
        cout << "MSG: GroundTeam moved to the opposite direction "
             << opposite << " to " << reversed.str() << "\n";

        moving_index = (moving_index + 1) % moving_rule.size();
        return reversed;
    }

    return Position::npos;
}


void GroundTeam::move() {
    for (size_t i = 0; i < moving_rule.size(); i++) {
        Position next = getNextPosition();
        if (next != Position::npos) pos = next;
    }
}


string GroundTeam::str() const {
    return "GroundTeam[index=" + to_string(index) +
           ";pos=" + pos.str() +
           ";moving_rule=" + moving_rule +"]";
}
bool GroundTeam::trap(DragonLord* dragon) {
    if (!dragon) return false;

    // Must be on the same tile
    if (pos == dragon->getCurrentPosition()) {
        if (trap_turns > 0) {
            dragon->setTrapped(trap_turns); // immobilize DragonLord
            trap_turns--; // consume one turn of trap ability
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
    // If trapped, skip move and decrement counter
    if (trapped_counter > 0) {
        trapped_counter--;

        // If trap just expired, teleport GroundTeam and deduct HP
        if (trapped_counter == 0 && groundteam) {
            Position oldPos = groundteam->getCurrentPosition();
            Position newPos(oldPos.getCol(), oldPos.getRow());
            groundteam->setPosition(newPos);
            groundteam->setHp(groundteam->getHp() - 200);
        }
        return;
    }

    Position prev = pos;
    Position next = getNextPosition();

    // Only move if the position is valid and different
    if (next != Position::npos && next != pos) {
        pos = next;
        step_counter++;

        // Only spawn SmartDragon if not trapped and moving
        if (step_counter % 5 == 0 && arr_mv_objs && !arr_mv_objs->isFull()) {
            spawnSmartDragon(prev);
        }
    }
}



void DragonLord::setTrapped(int turns) {
    trapped_counter = max(trapped_counter, turns);
}

void DragonLord::setHp(int new_hp) {
    hp = max(0, min(new_hp, 5000));
}

int DragonLord::getHp() const {
    return hp;
}

string DragonLord::str() const {
    return "DragonLord[index=" + to_string(index) +
           ";pos=" + pos.str() + "]";
}


////////////////////////////////////////////////////////////////////////
/// Smart Dragon
////////////////////////////////////////////////////////////////////////

// Default
Position SmartDragon::getNextPosition() {
    return Position::npos;
}

// spawnSmartDragon
void DragonLord::spawnSmartDragon(const Position& spawnPos) {
    if (!arr_mv_objs || arr_mv_objs->isFull()) return;

    // Count existing Smart Dragons
    int countSD1 = 0, countSD2 = 0, countSD3 = 0;
    for (int i = 0; i < arr_mv_objs->size(); ++i) {
        SmartDragon* sd = dynamic_cast<SmartDragon*>(arr_mv_objs->get(i));
        if (sd) {
            if (sd->getType() == SD1) countSD1++;
            else if (sd->getType() == SD2) countSD2++;
            else if (sd->getType() == SD3) countSD3++;
        }
    }

    DragonType typeToCreate;

    // Rule 1: First Smart Dragon is SD1
    if (countSD1 + countSD2 + countSD3 == 0) {
        typeToCreate = SD1;
    }
    else {
        // Manhattan distances
        int dist1 = abs(spawnPos.getRow() - flyteam1->getCurrentPosition().getRow()) +
                    abs(spawnPos.getCol() - flyteam1->getCurrentPosition().getCol());
        int dist2 = abs(spawnPos.getRow() - flyteam2->getCurrentPosition().getRow()) +
                    abs(spawnPos.getCol() - flyteam2->getCurrentPosition().getCol());
        int dist3 = abs(spawnPos.getRow() - groundteam->getCurrentPosition().getRow()) +
                    abs(spawnPos.getCol() - groundteam->getCurrentPosition().getCol());

        int minDist = dist1;
        if (dist2 < minDist) minDist = dist2;
        if (dist3 < minDist) minDist = dist3;

        bool tie1 = (dist1 == minDist);
        bool tie2 = (dist2 == minDist);
        bool tie3 = (dist3 == minDist);

        if ((tie1 + tie2 + tie3) == 1) {
            // Only one closest
            if (tie1) typeToCreate = SD1;
            else if (tie2) typeToCreate = SD2;
            else typeToCreate = SD3;
        } else {
            // Tie: pick one with least count
            typeToCreate = SD1;
            int minCount = 1000000;

            if (tie1 && countSD1 < minCount) {
                minCount = countSD1;
                typeToCreate = SD1;
            }
            if (tie2 && countSD2 < minCount) {
                minCount = countSD2;
                typeToCreate = SD2;
            }
            if (tie3 && countSD3 < minCount) {
                minCount = countSD3;
                typeToCreate = SD3;
            }
        }
    }

    // Create new SmartDragon
    MovingObject* newDragon = nullptr;
    if (typeToCreate == SD1)
        newDragon = new SmartDragonSD1(arr_mv_objs->size(), spawnPos, map, SD1, flyteam1, 100);
    else if (typeToCreate == SD2)
        newDragon = new SmartDragonSD2(arr_mv_objs->size(), spawnPos, map, SD2, flyteam2, 100);
    else
        newDragon = new SmartDragonSD3(arr_mv_objs->size(), spawnPos, map, SD3, groundteam, 100);
    cout << typeToCreate << " created at (" << flyteam1->getCurrentPosition().getRow() << "," << flyteam1->
            getCurrentPosition().getCol() << ")" << endl;
    if (newDragon) arr_mv_objs->add(newDragon);

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
    if (isFull())
        cout << "MSG: ArrayMovingObjects is full. Cannot create more SmartDragon" << endl;
            return false;
    arr_mv_objs[count++] = mv_obj;
    return true;
}

bool ArrayMovingObject::removeAt(int index) {
    if (index < 0 || index >= count) return false;
    delete arr_mv_objs[index];
    for (int i = index; i < count - 1; ++i) {
        arr_mv_objs[i] = arr_mv_objs[i + 1];
    }
    count--;
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

////////////////////////////////////////////////////////////////////////
/// CONFIGURATION
////////////////////////////////////////////////////////////////////////

Configuration::Configuration(const string &filepath) {
    // Initialize defaults
    map_num_rows = map_num_cols = max_num_moving_objects = 0;
    num_obstacles = num_ground_obstacles = 0;
    arr_obstacles = nullptr;
    arr_ground_obstacles = nullptr;
    flyteam1_hp = flyteam1_dmg = 0;
    flyteam2_hp = flyteam2_dmg = 0;
    groundteam_hp = groundteam_dmg = groundteam_trap_turns = 0;
    num_steps = 0;

    // Temporary storage for array strings
    string obst_str, gro_obst_str;

    ifstream fin(filepath.c_str());
    if (!fin.is_open()) {
        cerr << "Cannot open config file: " << filepath << endl;
        return;
    }

    string line;
    while (getline(fin, line)) {
        // Remove spaces
        string cleanLine;
        for (size_t i = 0; i < line.length(); ++i) {
            char c = line[i];
            if (c != ' ' && c != '\t' && c != '\r' && c != '\n')
                cleanLine += c;
        }
        if (cleanLine.empty()) continue;

        // Find '='
        size_t eqPos = cleanLine.find('=');
        if (eqPos == string::npos) continue;

        string key = cleanLine.substr(0, eqPos);
        string val = cleanLine.substr(eqPos + 1);

        if (key == "MAP_NUM_ROWS") map_num_rows = atoi(val.c_str());
        else if (key == "MAP_NUM_COLS") map_num_cols = atoi(val.c_str());
        else if (key == "MAX_NUM_MOVING_OBJECTS") max_num_moving_objects = atoi(val.c_str());

        else if (key == "NUM_OBSTACLE") num_obstacles = atoi(val.c_str());
        else if (key == "ARRAY_OBSTACLE") {
            obst_str = val;
            if (num_obstacles == 0 && val.size() > 2) { // Auto-count
                int count = 0;
                for (size_t i = 0; i < val.size(); ++i)
                    if (val[i] == '(') count++;
                num_obstacles = count;
            }
        }

        else if (key == "NUM_GROUND_OBSTACLE") num_ground_obstacles = atoi(val.c_str());
        else if (key == "ARRAY_GROUND_OBSTACLE") {
            gro_obst_str = val;
            if (num_ground_obstacles == 0 && val.size() > 2) { // Auto-count
                int count = 0;
                for (size_t i = 0; i < val.size(); ++i)
                    if (val[i] == '(') count++;
                num_ground_obstacles = count;
            }
        }

        else if (key == "FLYTEAM1_MOVING_RULE") flyteam1_rule = val;
        else if (key == "FLYTEAM1_INIT_POS") flyteam1_init_pos = Position(val);
        else if (key == "FLYTEAM1_INIT_HP") flyteam1_hp = atoi(val.c_str());
        else if (key == "FLYTEAM1_INIT_DAMAGE") flyteam1_dmg = atoi(val.c_str());

        else if (key == "FLYTEAM2_MOVING_RULE") flyteam2_rule = val;
        else if (key == "FLYTEAM2_INIT_POS") flyteam2_init_pos = Position(val);
        else if (key == "FLYTEAM2_INIT_HP") flyteam2_hp = atoi(val.c_str());
        else if (key == "FLYTEAM2_INIT_DAMAGE") flyteam2_dmg = atoi(val.c_str());

        else if (key == "GROUNDTEAM_MOVING_RULE") groundteam_rule = val;
        else if (key == "GROUNDTEAM_INIT_POS") groundteam_init_pos = Position(val);
        else if (key == "GROUNDTEAM_INIT_HP") groundteam_hp = atoi(val.c_str());
        else if (key == "GROUNDTEAM_INIT_DAMAGE") groundteam_dmg = atoi(val.c_str());
        else if (key == "GROUNDTEAM_TRAP_TURNS") groundteam_trap_turns = atoi(val.c_str());

        else if (key == "DRAGONLORD_INIT_POS") dragonlord_init_pos = Position(val);

        else if (key == "NUM_STEPS") num_steps = atoi(val.c_str());
    }
    fin.close();

    // Allocate obstacles
    if (num_obstacles > 0 && obst_str.size() >= 2) {
        arr_obstacles = new Position[num_obstacles];
        string content = obst_str.substr(1, obst_str.size() - 2); // strip []
        int idx = 0;
        size_t start = 0;
        while (start < content.size() && idx < num_obstacles) {
            size_t sep = content.find(';', start);
            if (sep == string::npos) sep = content.size();
            arr_obstacles[idx++] = Position(content.substr(start, sep - start));
            start = sep + 1;
        }
    }

    if (num_ground_obstacles > 0 && gro_obst_str.size() >= 2) {
        arr_ground_obstacles = new Position[num_ground_obstacles];
        string content = gro_obst_str.substr(1, gro_obst_str.size() - 2);
        int idx = 0;
        size_t start = 0;
        while (start < content.size() && idx < num_ground_obstacles) {
            size_t sep = content.find(';', start);
            if (sep == string::npos) sep = content.size();
            arr_ground_obstacles[idx++] = Position(content.substr(start, sep - start));
            start = sep + 1;
        }
    }
}


string Configuration::str() const {
    stringstream ss;
    ss << "Configuration[" << "\n";

    ss << "MAP_NUM_ROWS=" << map_num_rows << "\n";
    ss << "MAP_NUM_COLS=" << map_num_cols << "\n";
    ss << "MAX_NUM_MOVING_OBJECTS=" << max_num_moving_objects << "\n";
    ss << "NUM_OBSTACLE=" << num_obstacles << "\n";

    ss << "ARRAY_OBSTACLE=[";
    for (int i = 0; i < num_obstacles; i++) {
        ss << arr_obstacles[i].str();
        if (i != num_obstacles - 1) ss << ";";
    }
    ss << "]\n";

    ss << "NUM_GROUND_OBSTACLE=" << num_ground_obstacles << "\n";
    ss << "ARRAY_GROUND_OBSTACLE=[";
    for (int i = 0; i < num_ground_obstacles; i++) {
        ss << arr_ground_obstacles[i].str();
        if (i != num_ground_obstacles - 1) ss << ";";
    }
    ss << "]\n";

    ss << "FLYTEAM1_MOVING_RULE=" << flyteam1_rule << "\n";
    ss << "FLYTEAM1_INIT_POS=" << flyteam1_init_pos.str() << "\n";
    ss << "FLYTEAM1_INIT_HP=" << flyteam1_hp << "\n";
    ss << "FLYTEAM1_INIT_DAMAGE=" << flyteam1_dmg << "\n";

    ss << "FLYTEAM2_MOVING_RULE=" << flyteam2_rule << "\n";
    ss << "FLYTEAM2_INIT_POS=" << flyteam2_init_pos.str() << "\n";
    ss << "FLYTEAM2_INIT_HP=" << flyteam2_hp << "\n";
    ss << "FLYTEAM2_INIT_DAMAGE=" << flyteam2_dmg << "\n";

    ss << "GROUNDTEAM_MOVING_RULE=" << groundteam_rule << "\n";
    ss << "GROUNDTEAM_INIT_POS=" << groundteam_init_pos.str() << "\n";
    ss << "GROUNDTEAM_INIT_HP=" << groundteam_hp << "\n";
    ss << "GROUNDTEAM_INIT_DAMAGE=" << groundteam_dmg << "\n";

    ss << "DRAGONLORD_INIT_POS=" << dragonlord_init_pos.str() << "\n";

    ss << "NUM_STEPS=" << num_steps << "\n";
    ss << "]";
    return ss.str();
}

Configuration::~Configuration() {
    if (arr_obstacles) delete[] arr_obstacles;
    if (arr_ground_obstacles) delete[] arr_ground_obstacles;
}

// ===== Configuration getters =====
int Configuration::getMapNumRows() const { return map_num_rows; }
int Configuration::getMapNumCols() const { return map_num_cols; }
int Configuration::getMaxNumMovingObjects() const {return max_num_moving_objects;}
int Configuration::getNumObstacles() const { return num_obstacles; }
int Configuration::getNumGroundObstacles() const { return num_ground_obstacles; }
const Position* Configuration::getObstacles() const { return arr_obstacles; }
const Position* Configuration::getGroundObstacles() const { return arr_ground_obstacles; }

const string& Configuration::getFlyTeam1Rule() const { return flyteam1_rule; }
const Position& Configuration::getFlyTeam1Pos() const { return flyteam1_init_pos; }
int Configuration::getFlyTeam1InitHP() const { return flyteam1_hp; }
int Configuration::getFlyTeam1DMG() const { return flyteam1_dmg; }

const string& Configuration::getFlyTeam2Rule() const { return flyteam2_rule; }
const Position& Configuration::getFlyTeam2Pos() const { return flyteam2_init_pos; }
int Configuration::getFlyTeam2InitHP() const { return flyteam2_hp; }
int Configuration::getFlyTeam2DMG() const { return flyteam2_dmg; }

const string& Configuration::getGroundTeamRule() const { return groundteam_rule; }
const Position& Configuration::getGroundTeamPos() const { return groundteam_init_pos; }
int Configuration::getGroundTeamHP() const { return groundteam_hp; }
int Configuration::getGroundTeamDMG() const { return groundteam_dmg; }
int Configuration::getGroundTeamTrapTurns() const { return groundteam_trap_turns; }

const Position Configuration::getDragonlordInitPos() const {return dragonlord_init_pos;}
int Configuration::getNumSteps() const { return num_steps; }


//// ================== DragonWarriorsProgram ==================
DragonWarriorsProgram::DragonWarriorsProgram(const string &filepath) {
    // 1. Load configuration
    config = new Configuration(filepath);

    // 2. Build the map
    map = new Map(
        config->getMapNumRows(),
        config->getMapNumCols(),
        config->getNumObstacles(),
        const_cast<Position*>(config->getObstacles()),
        config->getNumGroundObstacles(),
        const_cast<Position*>(config->getGroundObstacles())
    );

    // 3. Create moving object array
    arr_mv_objs = new ArrayMovingObject(config->getMaxNumMovingObjects());

    // 4. Create teams
    flyteam1 = new FlyTeam(
        1,
        config->getFlyTeam1Rule(),
        config->getFlyTeam1Pos(),
        map,
        config->getFlyTeam1InitHP(),
        config->getFlyTeam1DMG()
    );

    flyteam2 = new FlyTeam(
        2,
        config->getFlyTeam2Rule(),
        config->getFlyTeam2Pos(),
        map,
        config->getFlyTeam2InitHP(),
        config->getFlyTeam2DMG()
    );

    groundteam = new GroundTeam(
        3,
        config->getGroundTeamRule(),
        config->getGroundTeamPos(),
        map,
        config->getGroundTeamHP(),
        config->getGroundTeamDMG()
    );
    groundteam->setTrapTurns(config->getGroundTeamTrapTurns());

    // 5. Create DragonLord
    dragonlord = new DragonLord(
        4,
        config->getDragonlordInitPos(),
        map,
        flyteam1,
        flyteam2,
        groundteam
    );

    // Give DragonLord access to moving object array
    dragonlord->setArrayMovingObject(arr_mv_objs);

    // 6. Add all starting objects to array
    arr_mv_objs->add(flyteam1);
    arr_mv_objs->add(flyteam2);
    arr_mv_objs->add(groundteam);
    arr_mv_objs->add(dragonlord);
}



bool DragonWarriorsProgram::isStop() const {
    // Victory: DragonLord defeated
    if (dragonlord->getHp() <= 0) return true;

    // Loss conditions from spec
    if (flyteam1->getHp() <= 1 && flyteam2->getHp() <= 1) return true;
    if (flyteam1->getHp() <= 1 && flyteam2->getHp() > 1) return true;

    return false;
}

void DragonWarriorsProgram::run(bool verbose) {
    if (groundteam) {
        groundteam->setTrapTurns(3);
    }

    for (int istep = 0; istep < config->getNumSteps(); ++istep) {

        auto doWarriorTurn = [&](Warrior* wr, const string& name) {
            if (!wr) return;

            // Use pending item from previous turn
            if (wr->hasPendingItemUse()) {
                BaseItem* item = wr->getBag()->getUsableItem();
                if (item) {
                    item->use(wr);
                    delete item;
                }
                wr->setPendingItemUse(false);
            }

            if (verbose) cout << "MSG: " << name << " moved" << endl;
            wr->move();

            // === Check Dragon Lord encounter ===
            if (wr->getCurrentPosition().isEqual(dragonlord->getCurrentPosition())) {
                if (auto ft = dynamic_cast<FlyTeam*>(wr)) {
                    if (ft->attack(dragonlord)) {
                        if (verbose) printStep(istep);
                        printResult();
                        return; // game ends
                    }
                } else if (auto gt = dynamic_cast<GroundTeam*>(wr)) {
                    if (verbose) cout << "MSG: " << name << " encouters DragonLord" << endl;
                    dragonlord->setTrapped(gt->getTrapTurns());
                }
            }

            // === SmartDragon combat ===
            for (int j = 0; j < arr_mv_objs->size(); ++j) {
                SmartDragon* sd = dynamic_cast<SmartDragon*>(arr_mv_objs->get(j));
                if (!sd) continue;
                if (!sd->getCurrentPosition().isEqual(wr->getCurrentPosition())) continue;

                bool sdDefeated = false;
                switch (sd->getType()) {
                    case SD1: sdDefeated = (wr->getDamage() >= sd->getDamage()); break;
                    case SD2: sdDefeated = (wr->getDamage() > sd->getDamage()); break;
                    case SD3: sdDefeated = (wr->getDamage() >= sd->getDamage()); break;
                }

                if (sdDefeated) {
                    BaseItem* loot = nullptr;
                    if (sd->getType() == SD3) {
                        if (wr == groundteam) {
                            loot = new TrapEnhancer();
                        } else if (wr == flyteam1) {
                            loot = new HealingHerb();
                        } else if (wr == flyteam2) {
                            loot = new DragonScale();
                        }
                    } else {
                        loot = sd->drop(wr);
                    }

                    if (loot && wr->getBag() && wr->getBag()->insert(loot)) {
                        wr->setPendingItemUse(true);
                        if (verbose) cout << wr->getName() << " obtained " << typeid(*loot).name() << "\n";
                    } else {
                        delete loot;
                    }

                    arr_mv_objs->removeAt(j);
                    j--;
                } else {
                    wr->setHp(wr->getHp() - 100);
                    if (wr->getHp() <= 0) {
                        for (int k = 0; k < arr_mv_objs->size(); ++k) {
                            if (arr_mv_objs->get(k) == wr) {
                                arr_mv_objs->removeAt(k);
                                break;
                            }
                        }
                        break;
                    }
                }
            }
        };

        // ---- Movement Order ----
        doWarriorTurn(flyteam1, "FlyTeam1");
        if (isStop()) { if (verbose) printStep(istep); printResult(); return; }

        doWarriorTurn(flyteam2, "FlyTeam2");
        if (isStop()) { if (verbose) printStep(istep); printResult(); return; }

        doWarriorTurn(groundteam, "GroundTeam");
        if (isStop()) { if (verbose) printStep(istep); printResult(); return; }

        // DragonLord turn
        if (dragonlord) {
            if (verbose) cout << "MSG: DragonLord moved" << endl;
            dragonlord->move();
            for (int j = 0; j < arr_mv_objs->size(); ++j) {
                Warrior* wr = dynamic_cast<Warrior*>(arr_mv_objs->get(j));
                if (wr && wr->getCurrentPosition().isEqual(dragonlord->getCurrentPosition())) {
                    if (auto ft = dynamic_cast<FlyTeam*>(wr)) {
                        if (ft->attack(dragonlord)) {
                            if (verbose) printStep(istep);
                            printResult();
                            return;
                        }
                    }
                }
            }
        }

        // SmartDragon turn
        for (int i = 0; i < arr_mv_objs->size(); ++i) {
            SmartDragon* sd = dynamic_cast<SmartDragon*>(arr_mv_objs->get(i));
            if (!sd) continue;
            sd->move();
            for (int j = 0; j < arr_mv_objs->size(); ++j) {
                Warrior* wr = dynamic_cast<Warrior*>(arr_mv_objs->get(j));
                if (wr && wr->getCurrentPosition().isEqual(sd->getCurrentPosition())) {
                    wr->setHp(wr->getHp() - sd->getDamage());
                    if (wr->getHp() <= 0) {
                        arr_mv_objs->removeAt(j);
                        if (j < i) i--;
                        j--;
                    }
                }
            }
        }

        if (verbose) {
            printStep(istep);
        }
    }
    printResult();
}



////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////