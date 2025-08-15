/*
* Ho Chi Minh City University of Technology
* Faculty of Computer Science and Engineering
* Initial code for Assignment 2
* Programming Fundamentals Summer 2025
* Date: 28.07.2025
*/

//The library here is concretely set, students are not allowed to include any other libraries.
#ifndef H_DRAGON_WARRIORS_H
#define H_DRAGON_WARRIORS_H

#include "main.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

// Forward declaration
class MovingObject;
class ArrayMovingObject;
class DragonWarriorsProgram;
class Position;
class Configuration;
class Map;

class DragonLord;
class SmartDragon;
class Warrior;
class FlyTeam;
class GroundTeam;

class BaseItem;
class BaseBag;
class TeamBag;

class TestDragonWar;

// ——— Enums ———
enum ElementType     { PATH, OBSTACLE, GROUND_OBSTACLE };
enum DragonType      { SD1, SD2, SD3 };
enum ItemType        { DRAGONSCALE, HEALINGHERB, TRAPENHANCER };

// ——— Position ———
class Position {
private:
    int r_, c_;
public:
    static const Position npos;
    Position(int r = 0, int c = 0);
    Position(const string &str_pos);

    int     getRow()  const;
    int     getCol()  const;
    void    setRow(int r);
    void    setCol(int c);
    string  str()     const;
    bool    isEqual(int in_r, int in_c) const;
    bool isEqual(const Position &other) const {
        return r_ == other.r_ && c_ == other.c_;
    }
    bool operator==(const Position& other) const;
    bool operator!=(const Position& other) const;
};

// ——— MapElement ———
class MapElement {
private:
    ElementType type;
    int         req_dmg; 
public:
    MapElement(ElementType t, int r = 0);
    virtual ~MapElement();
    ElementType getType() const;
    int getReqDmg()       const;
};

// ——— Map ———
class Map {
private:
    int           num_rows;
    int           num_cols;
    MapElement ***grid;

public:
    Map(int rows, int cols,
        int num_obst,       Position *obst,
        int num_gro_obst,   Position *gro_obst);
    ~Map();
    bool isValid(const Position & pos, MovingObject * map) const;
    int  getNumRows()     const;
    int  getNumCols()     const;
};


// ——— MovingObject ———
class MovingObject {
protected:
    int      index;
    Position   pos;
    Map     *  map;
    string    name;

public:
    MovingObject(int index, const Position & pos, Map *map, const string & name = "");
    virtual ~MovingObject();
    virtual Position getNextPosition() = 0;

    const Position &getCurrentPosition() const;
    void setPosition(const Position &p) { pos = p; }
    virtual void      move()          = 0;
    virtual string    str()    const = 0;

};

// ——— Warrior ———
class Warrior: public MovingObject {
protected:
    int hp;
    int damage;
    BaseBag* bag;
public:
    Warrior(int index, const Position & pos, Map * map,
            const string & name, int hp, int damage);
    virtual ~Warrior();
    // TODO
    // getter and setter
    int getHp() const;
    int getDamage() const;
    void setHp(int hp);
    void setDamage(int dmg);
    // getBag()
    BaseBag* getBag() const;
    string getName() const { return name; }
};

// ——— FlyTeam  ———
class FlyTeam : public Warrior {
private:
    // TODO
    string moving_rule;
    int moving_index;
public:
    FlyTeam(int index, const string & moving_rule,
        const Position & pos, Map * map, int hp, int damage);
    // TODO
    // getNextPosition()
    Position getNextPosition() override;
    // move()
    void move() override;
    // str()
    string str() const override;
    // attack()
       bool attack(DragonLord *dragon);
};

// ——— GroundTeam  ———
class GroundTeam : public Warrior {
private:
    // TODO
    string moving_rule;
    int moving_index;
    int trap_turns;
public:
    GroundTeam(int index, const string & moving_rule,
        const Position & pos, Map * map, int hp, int damage);
    // TODO
    // getNextPosition()
    Position getNextPosition() override;
    // move
    void move() override;
    // str
    string str() const override;
    // trap
    bool trap(DragonLord* dragon);
    // getter and setter
    int getTrapTurns() const;
    void setTrapTurns(int turns);
};

// ——— DragonLord ———
class DragonLord : public MovingObject {
private:
    // TODO
    int hp;
    int trapped_counter;
    GroundTeam* groundteam;
    int step_counter = 0; // count only valid moves
    ArrayMovingObject* arr_mv_objs; // needs to be passed in from outside
    void spawnSmartDragon(const Position& prevPos);
public:
    FlyTeam* flyteam1;
    FlyTeam* flyteam2;
    DragonLord(int index, const Position & pos, Map * map,
               FlyTeam *flyteam1, FlyTeam *flyteam2, GroundTeam * groundteam);
    // TODO
    // getNextPosition()
    Position getNextPosition() override;
    // move
    void move() override;
    // str
    string str() const override;
    void setTrapped(int turns);
    void setHp(int hp);
    int getHp() const;
    void setArrayMovingObject(ArrayMovingObject* arr) { arr_mv_objs = arr; }
};

// ...................
// ——— BaseItem ———
class BaseItem {
public:
    virtual ~BaseItem() {}
    virtual bool canUse(Warrior* w) = 0;
    virtual void use(Warrior* w) = 0;
};
// ================= DragonScale =================
class DragonScale : public BaseItem {
public:
    bool canUse(Warrior* w) {
        return w && w->getDamage() <= 400;
    }
    void use(Warrior* w) {
        if (canUse(w)) {
            int dmg = w->getDamage();
            int bonus = dmg * 25 / 100; // +25%
            w->setDamage(dmg + bonus);
        }
    }
};

// ================= HealingHerb =================
class HealingHerb : public BaseItem {
public:
    bool canUse(Warrior* w) {
        return w && w->getHp() <= 100;
    }
    void use(Warrior* w) {
        if (canUse(w)) {
            int hp = w->getHp();
            int bonus = hp * 20 / 100; // +20%
            w->setHp(hp + bonus);
        }
    }
};

// ================= TrapEnhancer =================
class TrapEnhancer : public BaseItem {
public:
    bool canUse(Warrior* w) {
        // Works only for GroundTeam, but no condition
        return dynamic_cast<GroundTeam*>(w) != nullptr;
    }
    void use(Warrior* w) {
        GroundTeam* gt = dynamic_cast<GroundTeam*>(w);
        if (gt) {
            gt->setTrapTurns(gt->getTrapTurns() + 1);
        }
    }
};
// ——— SmartDragon ———
class SmartDragon : public MovingObject {
protected:
    DragonType smartdragon_type;
    int damage;
    BaseItem* item;            // Item dropped on defeat
    MovingObject* target;      // The object this Smart Dragon targets

public:
    DragonType getType() const { return smartdragon_type; }
    SmartDragon(int index, const Position &init_pos, Map *map,
                DragonType type, MovingObject *target, int damage)
        : MovingObject(index, init_pos, map, "SmartDragon"),
          smartdragon_type(type), damage(damage), item(nullptr), target(target) {}

    virtual Position getNextPosition() = 0; // pure virtual
    virtual void move() {
        Position next = getNextPosition();
        if (next != Position::npos) {
            pos = next;
        }
    }

    string str() const override {
        string typeStr;
        if (smartdragon_type == SD1) typeStr = "SD1";
        else if (smartdragon_type == SD2) typeStr = "SD2";
        else if (smartdragon_type == SD3) typeStr = "SD3";

        return "smartdragon[pos=" + pos.str() +
               ";type=" + typeStr +
               ";tg=" + target->str() + "]";
    }

    int getDamage() const { return damage; }
    void setDamage(int dmg) { damage = max(0, min(dmg, 900)); }

    virtual BaseItem* drop(Warrior* killer) {
        return item;
    }

};

// ================= SD1 =================
class SmartDragonSD1 : public SmartDragon {
public:
    SmartDragonSD1(int index, const Position &init_pos, Map *map,
              DragonType type, MovingObject *flyteam1, int damage)
        : SmartDragon(index, init_pos, map, type, flyteam1, damage) {
        item = new DragonScale();
    }

    Position getNextPosition() override {
        if (!target) return Position::npos;
        Position tpos = target->getCurrentPosition();
        int dr = tpos.getRow() - pos.getRow();
        int dc = tpos.getCol() - pos.getCol();

        // Movement priority: UP, RIGHT, DOWN, LEFT
        Position next = pos;
        if (dr < 0) next.setRow(pos.getRow() - 1);        // UP
        else if (dc > 0) next.setCol(pos.getCol() + 1);   // RIGHT
        else if (dr > 0) next.setRow(pos.getRow() + 1);   // DOWN
        else if (dc < 0) next.setCol(pos.getCol() - 1);   // LEFT

        return (map && map->isValid(next, this)) ? next : Position::npos;
    }
};

// ================= SD2 =================
class SmartDragonSD2 : public SmartDragon {
public:
    SmartDragonSD2(int index, const Position &init_pos, Map *map,
              DragonType type, MovingObject *flyteam2, int damage)
        : SmartDragon(index, init_pos, map, type, flyteam2, damage) {
        item = new HealingHerb();
    }

    Position getNextPosition() override {
        if (!target) return Position::npos;
        Position tpos = target->getCurrentPosition();
        int dr = tpos.getRow() - pos.getRow();
        int dc = tpos.getCol() - pos.getCol();

        // Movement priority: UP, RIGHT, DOWN, LEFT
        Position next = pos;
        if (dr < 0) next.setRow(pos.getRow() - 1);        // UP
        else if (dc > 0) next.setCol(pos.getCol() + 1);   // RIGHT
        else if (dr > 0) next.setRow(pos.getRow() + 1);   // DOWN
        else if (dc < 0) next.setCol(pos.getCol() - 1);   // LEFT

        return (map && map->isValid(next, this)) ? next : Position::npos;
    }
};

// ================= SD3 =================
class SmartDragonSD3 : public SmartDragon {
public:
    SmartDragonSD3(int index, const Position &init_pos, Map *map,
              DragonType type, MovingObject *groundteam, int damage)
        : SmartDragon(index, init_pos, map, type, groundteam, damage) {
        item = new TrapEnhancer();
    }

    Position getNextPosition() override {
        return Position::npos;
        // SD3 does not move
    }
    BaseItem* Drop(MovingObject* killer) {
        // If killed by GroundTeam → TrapEnhancer
        if (dynamic_cast<GroundTeam*>(killer)) {
            return new TrapEnhancer();
        }
        // If killed by FlyTeam1 → HealingHerb
        if (auto ft = dynamic_cast<FlyTeam*>(killer)) {
            if (ft->str().find("FlyTeam1") != string::npos) {
                return new HealingHerb();
            }
            // If killed by FlyTeam2 → DragonScale
            else if (ft->str().find("FlyTeam2") != string::npos) {
                return new DragonScale();
            }
        }
        // Fallback
        return nullptr;
    }
};

// ——— BaseBag ———
class BaseBag {
protected:
    BaseItem** items;   // dynamic array of BaseItem*
    int capacity;       // maximum capacity
    int count;          // current number of items
    Warrior* owner;     // the Warrior that owns this bag

public:
    BaseBag(Warrior* w, int cap) : owner(w), capacity(cap), count(0) {
        items = new BaseItem*[capacity];
        for (int i = 0; i < capacity; i++) {
            items[i] = nullptr;
        }
    }

    virtual ~BaseBag() {
        for (int i = 0; i < count; i++) {
            delete items[i];
        }
        delete[] items;
    }

    virtual bool insert(BaseItem* item) {
        if (count >= capacity) return false;
        items[count++] = item;
        return true;
    }

    virtual BaseItem* getUsableItem() {
        for (int i = 0; i < count; i++) {
            if (items[i] && items[i]->canUse(owner)) {
                BaseItem* usable = items[i];

                // Shift the rest left
                for (int j = i; j < count - 1; j++) {
                    items[j] = items[j + 1];
                }
                items[count - 1] = nullptr;
                count--;

                return usable;
            }
        }
        return nullptr;
    }
    virtual string getName() const {
        return typeid(*this).name();
    }
    virtual string str() const {
        string s = "Bag[";
        for (int i = 0; i < count; i++) {
            if (items[i]) {
                s += typeid(*items[i]).name();
            } else {
                s += "null";
            }
            if (i != count - 1) s += ",";
        }
        s += "]";
        return s;
    }
};

class TeamBag : public BaseBag {
public:
    TeamBag(Warrior* w, int)
        : BaseBag(w, (dynamic_cast<FlyTeam*>(w) ? 5 : 7)) {
        // capacity is set automatically: 5 for FlyTeam, 7 for GroundTeam
    }
};
// ...................

// ——— ArrayMovingObject ———
class ArrayMovingObject {
private:
    // TODO
    MovingObject** arr_mv_objs;
    int count;
    int capacity;

public:
    ArrayMovingObject(int capacity);
    ~ArrayMovingObject() ;
    // TODO
    bool isFull() const;
    bool add(MovingObject* mv_obj);
    string str() const;
    int size() const { return count; }  // assuming count is element counter
    MovingObject* get(int index) const { return arr_mv_objs[index]; } // assuming arr holds pointers

    bool removeAt(int index);
};

// ——— Configuration ———
class Configuration {
    friend class DragonWarProgram;

private:
    // TODO
    int map_num_rows;
    int map_num_cols;
    int num_obstacles;
    Position* arr_obstacles;
    int num_ground_obstacles;
    Position* arr_ground_obstacles;

    Position flyteam1_init_pos;
    string flyteam1_rule;
    int flyteam1_hp;
    int flyteam1_dmg;

    Position flyteam2_init_pos;
    string flyteam2_rule;
    int flyteam2_hp;
    int flyteam2_dmg;

    Position groundteam_init_pos;
    string groundteam_rule;
    int groundteam_hp;
    int groundteam_dmg;
    int groundteam_trap_turns;

    int max_num_moving_objects;
    Position dragonlord_init_pos;

    int num_steps;
public:
    Configuration(const string & filepath);
    ~Configuration();
    string str() const;
    // TODO
    // Getters
    int getMapNumRows() const;
    int getMapNumCols() const;
    int getNumObstacles() const;
    int getNumGroundObstacles() const;
    const Position* getObstacles() const;
    const Position* getGroundObstacles() const;

    const string& getFlyTeam1Rule() const;
    const Position& getFlyTeam1Pos() const;
    int getFlyTeam1InitHP() const;
    int getFlyTeam1DMG() const;

    const string& getFlyTeam2Rule() const;
    const Position& getFlyTeam2Pos() const;
    int getFlyTeam2InitHP() const;
    int getFlyTeam2DMG() const;

    const string& getGroundTeamRule() const;
    const Position& getGroundTeamPos() const;
    int getGroundTeamHP() const;
    int getGroundTeamDMG() const;
    int getGroundTeamTrapTurns() const;

    int getMaxNumMovingObjects() const;
    const Position getDragonlordInitPos() const;

    int getNumSteps() const;
};

// ——— DragonWarriorsProgram ———
class DragonWarriorsProgram {
private:


public:
    // Sample attributes

    Configuration      *config;

    FlyTeam            *flyteam1;
    FlyTeam            *flyteam2;

    GroundTeam         *groundteam;
    DragonLord         *dragonlord;

    Map                *map;
    ArrayMovingObject  *arr_mv_objs;

    DragonWarriorsProgram(const string &config_file_path);

    bool   isStop() const;
    FlyTeam* getFlyTeam1() const { return flyteam1; }
    FlyTeam* getFlyTeam2() const { return flyteam2; }
    void printResult() const {
        if (flyteam1->getCurrentPosition().isEqual(dragonlord->getCurrentPosition())) {
            cout << "FlyTeam1 defeated the DragonLord!" << endl;
        }
        else if (flyteam2->getCurrentPosition().isEqual(dragonlord->getCurrentPosition())) {
            cout << "FlyTeam2 defeated the DragonLord!" << endl;
        }
        else {
            cout << "The warrior lost the battle!" << endl;
        }
    }

    void printStep(int si) const {
        cout << "Step: " << setw(4) << setfill('0') << si
            << "--"
            << flyteam1->str() << "--|--" << flyteam2->str()<< "--|--" << groundteam->str() << "--|--" << dragonlord->str() << endl;
    }

    void run(bool verbose);

};

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
#endif /* H_DRAGON_WARRIORS_H */
