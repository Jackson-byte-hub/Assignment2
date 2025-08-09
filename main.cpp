/*
* Ho Chi Minh City University of Technology
* Faculty of Computer Science and Engineering
* Initial code for Assignment 2
* Programming Fundamentals Summer 2025
* Date: 28.07.2025
*/

//The library here is concretely set, students are not allowed to include any other libraries.

#include "dragon.h"
#include "sample_tc_0001_0015.h"
using namespace std;

void g_satc_02() { // global
    cout << "----- Sample Testcase 02 -----" << endl;
    Configuration * config = new Configuration("sa_tc_02_config");
    cout << config->str() << endl;
    delete config;
}

// class TestDragonWar {
// public:
//     TestDragonWar() = default;
//     static void satc_01() {
//         cout << "----- Sample Testcase 01 -----" << endl;
//         int num_obstacle = 3;
//         Position arr_obstacle[] = {Position(1, 2), Position(2, 3), Position(3, 4)};
//         int num_ground_obstacle = 1;
//         Position arr_ground_obstacle[] = {Position(2, 0)};
//
//         Map * map = new Map(10, 10, num_obstacle, arr_obstacle, num_ground_obstacle, arr_ground_obstacle);
//
//
//         FlyTeam * flyteam1 = new FlyTeam(1, "RUU", Position(1, 3), map, 250, 450);
//         cout << flyteam1->str() << endl;
//
//         FlyTeam * flyteam2 = new FlyTeam(2, "RUU", Position(1, 3), map, 250, 450);
//         cout << flyteam2->str() << endl;
//
//         GroundTeam * groundteam = new GroundTeam(3, "LU", Position(2, 1), map, 300, 350);
//         cout << groundteam->str() << endl;
//
//         DragonLord * dragonlord = new DragonLord(0, Position(7, 9), map, flyteam1, flyteam2);
//         cout << dragonlord->str() << endl;
//
//         cout << "* FlyTeam1 makes a move" << endl;
//         flyteam1->move();
//         cout << flyteam1->str() << endl;
//
//         cout << "* FlyTeam2 makes a move" << endl;
//         flyteam2->move();
//         cout << flyteam2->str() << endl;
//
//         cout << "* GroundTeam makes a move" << endl;
//         groundteam->move();
//         cout << groundteam->str() << endl;
//
//         cout << "* DragonLord makes a move" << endl;
//         dragonlord->move();
//         cout << dragonlord->str() << endl;
//
//         ArrayMovingObject * arr_mv_objs = new ArrayMovingObject(10);
//         arr_mv_objs->add(dragonlord);
//         arr_mv_objs->add(flyteam1);
//         arr_mv_objs->add(flyteam2);
//         arr_mv_objs->add(groundteam);
//         cout << arr_mv_objs->str() << endl;
//
//         delete arr_mv_objs;
//         delete flyteam1;
//         delete flyteam2;
//         delete groundteam;
//         delete dragonlord;
//         delete map;
//     }
//
//     static void satc_02() {
//         g_satc_02();
//     }
//
//     static void satc_03() {
//         // Sample test for SD1
//         cout << "----- Sample Testcase 03 -----" << endl;
//         int num_obstacle = 3;
//         Position arr_obstacle[] = {Position(4, 6), Position(8, 8), Position(2, 5)};
//         int num_ground_obstacle = 1;
//         Position arr_ground_obstacle[] = {Position(5, 0)};
//
//         Map * map = new Map(10, 10, num_obstacle, arr_obstacle, num_ground_obstacle, arr_ground_obstacle);
//
//         FlyTeam * flyteam1 = new FlyTeam(1, "RUU", Position(7, 6), map, 250, 650);
//         FlyTeam * flyteam2 = new FlyTeam(2, "LLD", Position(1, 3), map, 250, 550);
//         GroundTeam * groundteam = new GroundTeam(3, "LU", Position(8, 8), map, 400, 350);
//         DragonLord * dragonlord = new DragonLord(0, Position(7, 9), map, flyteam1, flyteam2, groundteam);
//         dragonlord->move();
//
//         SmartDragon * sd1 = new SmartDragon(3, Position(7,9), map, DragonType::SD1, flyteam1, 200);
//         cout << "Current position of DragonLord: " << dragonlord->str() << endl;
//         cout << "DragonLord makes a move" << endl;
//         dragonlord->move();
//         cout << "New position of DragonLord: " << dragonlord->str() << endl;
//
//         cout << "Current position of SD1: " << sd1->str() << endl;
//         cout << "SD1 makes a move" << endl;
//         sd1->move();
//         cout << "New position of SD1: " << sd1->str() << endl;
//
//         delete flyteam1;
//         delete flyteam2;
//         delete groundteam;
//         delete dragonlord;
//         delete map;
//         delete sd1;
//     }
// };

static void satc_0001() {
    cout << "----- Sample Testcase 01 -----" << endl;
    MapElement * myMapCell_1 = new MapElement(ElementType::PATH, 0);
    cout << "Cell_1 [type=" << myMapCell_1->getType() << ",req_dmg=" << myMapCell_1->getReqDmg() << "]" << endl;
}

int main(int argc, const char * argv[]) {
    TestDragonWar::satc_0001();
    return 0;
}