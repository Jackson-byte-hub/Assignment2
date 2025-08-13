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
// class MovingObject;
// class ArrayMovingObject
// class DragonWarriorsProgram
// class Position;
// class Configuration;
// class Map;

// class DragonLord;
// class SmartDragon;
// class Warrior;
// class FlyTeam;
// class GroundTeam;

// class BaseItem;
// class BaseBag;
// class TeamBag;

class DragonLord;
class MovingObject;
class BaseBag;
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
};

// ...................
// ——— SmartDragon ———
// ——— BaseItem ———
// ——— BaseBag ———
class BaseBag;
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

    int getNumSteps() const;
};

// ——— DragonWarriorsProgram ———
class DragonWarriorsProgram {
private:
    // Sample attributes

    Configuration      *config;

    FlyTeam            *flyteam1;
    FlyTeam            *flyteam2;
    GroundTeam         *groundteam;
    DragonLord         *dragonlord;

    Map                *map;
    ArrayMovingObject  *arr_mv_objs;
    
public:
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

    void run(bool verbose) {
        // Note: This is a sample code. You can change the implementation as you like.
        // TODO
        for (int istep = 0; istep < config->getNumSteps(); ++istep) {
            for (int i = 0; i < arr_mv_objs->size(); ++i) {
                arr_mv_objs->get(i)->move();
                if (isStop()) {
                    printStep(istep);
                    break;
                }
                if (verbose) {
                    printStep(istep);
                }
            }
        }
        printResult();
    }

    ~DragonWarriorsProgram();
};

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
#endif /* H_DRAGON_WARRIORS_H */
