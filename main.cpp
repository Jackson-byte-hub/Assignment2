/*
* Ho Chi Minh City University of Technology
* Faculty of Computer Science and Engineering
* Initial code for Assignment 2
* Programming Fundamentals Summer 2025
* Date: 28.07.2025
*/

//The library here is concretely set, students are not allowed to include any other libraries.

#include "dragon.h"
// #include "sample_tc_0001_0015.h"
using namespace std;

void g_satc_02() { // global
    cout << "----- Sample Testcase 02 -----" << endl;
    Configuration * config = new Configuration("sa_tc_02_config");
    cout << config->str() << endl;
    delete config;
}

class TestDragonWar {
public:
    TestDragonWar() = default;
        // SIMPLE CLASS IMPLEMENTATION TESTS
    static void satc_0001() {
        cout << "----- Sample Testcase 01 -----" << endl;
        MapElement * myMapCell_1 = new MapElement(ElementType::PATH, 0);
        cout << "Cell_1 [type=" << myMapCell_1->getType() << ",req_dmg=" << myMapCell_1->getReqDmg() << "]" << endl;
    }

    static void satc_0002() {
        cout << "\n----- Sample Testcase 02 -----" << endl;
        MapElement * myMapCell_1 = new MapElement(ElementType::PATH, 0);
        MapElement * myMapCell_2 = new MapElement(ElementType::GROUND_OBSTACLE, 100);
        cout << "Cell_1 [type=" << myMapCell_1->getType() << ",req_dmg=" << myMapCell_1->getReqDmg() << "]" << endl;
        cout << "Cell_2 [type=" << myMapCell_2->getType() << ",req_dmg=" << myMapCell_2->getReqDmg() << "]" << endl;
        cout << (myMapCell_1->getType() == myMapCell_2->getType()) << endl;
        delete myMapCell_1;
        delete myMapCell_2;
    }

    static void satc_0003() {
        cout << "\n----- Sample Testcase 03 -----" << endl;
        Map * map = new Map(5, 15, 0, nullptr, 0, nullptr);
        cout << map->getNumCols() << endl;
        cout << map->getNumRows() << endl;
        delete map;
    }

    static void satc_0004() {
        cout << "\n----- Sample Testcase 04 -----" << endl;
        Map * map = new Map(10, 10, 0, nullptr, 0, nullptr);
        FlyTeam * flyteam = new FlyTeam(0, "RR", Position(0, 0), map, 100, 100);
        cout << flyteam->getCurrentPosition().str() << endl;
        delete map;
        delete flyteam;
    }

    static void satc_0005() {
        cout << "\n----- Sample Testcase 05 -----" << endl;
        Map * map = new Map(10, 10, 0, nullptr, 0, nullptr);
        FlyTeam * flyteam = new FlyTeam(0, "RRDD", Position(1, 1), map, 100, 100);
        cout << flyteam->str() << endl;
        delete map;
        delete flyteam;
    }

    static void satc_0006() {
        cout << "\n----- Sample Testcase 06 -----" << endl;
        Map * map = new Map(10, 10, 0, nullptr, 0, nullptr);
        FlyTeam * flyteam = new FlyTeam(0, "RRDD", Position(1, 1), map, 1000, 1000);
        cout << "FlyTeam1[damage="<<flyteam->getDamage(); // should be 900
        cout << ",HP="<< flyteam->getHp() << "]" << endl; // should be 500
        cout << flyteam->str() << endl;
        delete map;
        delete flyteam;
    }

    static void satc_0007() {
        cout << "\n----- Sample Testcase 07 -----" << endl;
        Map * map = new Map(5, 5, 0, nullptr, 0, nullptr);
        FlyTeam * flyteam = new FlyTeam(1, "R", Position(0, 0), map, 100, 100);

        cout << "Initial positions:" << endl;
        cout << flyteam->getCurrentPosition().str() << endl;
        cout << "After first move:" << endl;
        flyteam->move();
        cout << flyteam->getCurrentPosition().str() << endl;
        cout << "After second move:" << endl;
        flyteam->move();
        cout << flyteam->getCurrentPosition().str() << endl;
        cout << "Final Information" << endl;
        cout  << flyteam->str() << endl;

        delete flyteam;
        delete map;
    }

    // MOVEMENT TESTS
    static void satc_0008() {
        cout << "\n----- Sample Testcase 08 -----" << endl;
        Map * map = new Map(10, 10, 0, nullptr, 0, nullptr);
        FlyTeam * flyteam = new FlyTeam(1, "RDDUL", Position(0, 0), map, 100, 100);

        cout << "Initial positions:" << endl;
        cout << flyteam->getCurrentPosition().str() << endl;
        cout << "After first move:" << endl;
        flyteam->move();
        cout << flyteam->getCurrentPosition().str() << endl;
        cout << "After second move:" << endl;
        flyteam->move();
        cout << flyteam->getCurrentPosition().str() << endl;
        cout << "Final Information" << endl;
        cout  << flyteam->str() << endl;

        delete flyteam;
        delete map;
    }

    static void satc_0009() {
        cout << "\n----- Sample Testcase 09 -----" << endl;
        Map * map = new Map(10, 10, 0, nullptr, 0, nullptr);
        FlyTeam * flyteam = new FlyTeam(1, "RD", Position(0, 0), map, 100, 100);
        GroundTeam * groundteam = new GroundTeam(2, "UL", Position(9, 9), map, 100, 100);

        cout << "Initial positions:" << endl;
        cout << flyteam->str() << endl;
        cout << groundteam->str() << endl;

        cout << "After first move:" << endl;
        flyteam->move();
        groundteam->move();
        cout << flyteam->str() << endl;
        cout << groundteam->str() << endl;

        cout << "After second move:" << endl;
        flyteam->move();
        groundteam->move();
        cout << flyteam->str() << endl;
        cout << groundteam->str() << endl;

        delete flyteam;
        delete groundteam;
        delete map;
    }

    static void satc_0010() {
        cout << "\n----- Sample Testcase 10 -----" << endl;
        Map * map = new Map(10, 10, 0, nullptr, 0, nullptr);
        FlyTeam * flyteam1 = new FlyTeam(1, "RRRR", Position(1, 1), map, 100, 100);
        FlyTeam * flyteam2 = new FlyTeam(2, "LLLL", Position(5, 5), map, 100, 100);
        GroundTeam * groundteam = new GroundTeam(2, "UURR", Position(8, 9), map, 400, 100);
        DragonLord * dragonlord = new DragonLord(0, Position(3, 3), map, flyteam1, flyteam2, groundteam);

        cout << "Initial positions:" << endl;
        cout << "FlyTeam1: " << flyteam1->str() << endl;
        cout << "FlyTeam2: " << flyteam2->str() << endl;
        cout << "DragonLord: " << dragonlord->str() << endl;

        cout << "After moving:" << endl;
        flyteam1->move();
        flyteam2->move();
        dragonlord->move();
        cout << "FlyTeam1: " << flyteam1->str() << endl;
        cout << "FlyTeam2: " << flyteam2->str() << endl;
        cout << "DragonLord: " << dragonlord->str() << endl;

        delete flyteam1;
        delete flyteam2;
        delete dragonlord;
        delete map;
    }

    // CONFIGURATION TEST
    static void satc_0011() {
        cout << "\n----- Sample Testcase 11 -----" << endl;
        Configuration * config = new Configuration("sa_tc_01_config");
        cout << config->getFlyTeam1InitHP() << endl;
        cout << config->str() << endl;
        delete config;
    }

    static void satc_0012() {
        cout << "\n----- Sample Testcase 12 -----" << endl;
        Configuration * config = new Configuration("sa_tc_02_config");
        cout << config->str() << endl;
        delete config;
    }

    static void satc_0013() {
        cout << "\n----- Sample Testcase 13 -----" << endl;
        Configuration * config = new Configuration("sa_tc_01_config");
        cout << config->getMapNumCols() << endl;
        cout << config->getMapNumRows() << endl;
        delete config;
    }

    static void satc_0014() {
        cout << "\n----- Sample Testcase 14 -----" << endl;
        Configuration * config = new Configuration("sa_tc_01_config");
        cout << config->getFlyTeam1InitHP() << endl;
        cout << config->getFlyTeam2InitHP() << endl;
        delete config;
    }

    static void satc_0015() {
        cout << "\n----- Sample Testcase 15 -----" << endl;
        DragonWarriorsProgram * myProgram = new DragonWarriorsProgram("sa_tc_01_config");
        cout << myProgram->flyteam1->str() << endl;
        cout << myProgram->flyteam2->str() << endl;
        delete myProgram;
    }

    static void satc_01() {
        cout << "----- Sample Testcase 01 -----" << endl;
        int num_obstacle = 3;
        Position arr_obstacle[] = {Position(1, 2), Position(2, 3), Position(3, 4)};
        int num_ground_obstacle = 1;
        Position arr_ground_obstacle[] = {Position(2, 0)};

        Map * map = new Map(10, 10, num_obstacle, arr_obstacle, num_ground_obstacle, arr_ground_obstacle);


        FlyTeam * flyteam1 = new FlyTeam(1, "RUU", Position(1, 3), map, 250, 450);
        cout << flyteam1->str() << endl;

        FlyTeam * flyteam2 = new FlyTeam(2, "RUU", Position(1, 3), map, 250, 450);
        cout << flyteam2->str() << endl;

        GroundTeam * groundteam = new GroundTeam(3, "LU", Position(2, 1), map, 300, 350);
        cout << groundteam->str() << endl;

        DragonLord * dragonlord = new DragonLord(0, Position(7, 9), map, flyteam1, flyteam2, groundteam);
        cout << dragonlord->str() << endl;

        cout << "* FlyTeam1 makes a move" << endl;
        flyteam1->move();
        cout << flyteam1->str() << endl;

        cout << "* FlyTeam2 makes a move" << endl;
        flyteam2->move();
        cout << flyteam2->str() << endl;

        cout << "* GroundTeam makes a move" << endl;
        groundteam->move();
        cout << groundteam->str() << endl;

        cout << "* DragonLord makes a move" << endl;
        dragonlord->move();
        cout << dragonlord->str() << endl;

        ArrayMovingObject * arr_mv_objs = new ArrayMovingObject(10);
        arr_mv_objs->add(dragonlord);
        arr_mv_objs->add(flyteam1);
        arr_mv_objs->add(flyteam2);
        arr_mv_objs->add(groundteam);
        cout << arr_mv_objs->str() << endl;

        delete arr_mv_objs;
        // delete flyteam1;
        // delete flyteam2;
        // delete groundteam;
        // delete dragonlord;
        delete map;
    }

    static void satc_02() {
        g_satc_02();
    }

    static void satc_03() {
        // Sample test for SD1
        cout << "----- Sample Testcase 03 -----" << endl;
        int num_obstacle = 3;
        Position arr_obstacle[] = {Position(4, 6), Position(8, 8), Position(2, 5)};
        int num_ground_obstacle = 1;
        Position arr_ground_obstacle[] = {Position(5, 0)};

        Map * map = new Map(10, 10, num_obstacle, arr_obstacle, num_ground_obstacle, arr_ground_obstacle);

        FlyTeam * flyteam1 = new FlyTeam(1, "RUU", Position(7, 6), map, 250, 650);
        FlyTeam * flyteam2 = new FlyTeam(2, "LLD", Position(1, 3), map, 250, 550);
        GroundTeam * groundteam = new GroundTeam(3, "LU", Position(8, 8), map, 400, 350);
        DragonLord * dragonlord = new DragonLord(0, Position(7, 9), map, flyteam1, flyteam2, groundteam);
        dragonlord->move();

        SmartDragon * sd1 = new SmartDragon(3, Position(7,9), map, DragonType::SD1, flyteam1, 200);
        cout << "Current position of DragonLord: " << dragonlord->str() << endl;
        cout << "DragonLord makes a move" << endl;
        dragonlord->move();
        cout << "New position of DragonLord: " << dragonlord->str() << endl;

        cout << "Current position of SD1: " << sd1->str() << endl;
        cout << "SD1 makes a move" << endl;
        sd1->move();
        cout << "New position of SD1: " << sd1->str() << endl;

        delete flyteam1;
        delete flyteam2;
        delete groundteam;
        delete dragonlord;
        delete map;
        delete sd1;
    }
    static void satc_19() {
        cout << "\n----- Sample Testcase 19 -----" << endl;
        string config_path = "sa_tc_06_config";
        bool verbose = true;
        DragonWarriorsProgram * program = new DragonWarriorsProgram(config_path);
        cout << "------------------------------------------------------!BEGIN!------------------------------------------------------" << endl;
        cout << "Step:START--" << program->flyteam1->str() << "--|--" << program->flyteam2->str() << "--|--" << program->groundteam->str() << "--|--" << program->dragonlord->str() << endl;
        program->run(verbose);
        cout << "-------------------------------------------------------!END!-------------------------------------------------------" << endl;
        delete program;
    }

    static void satc_20() {
        cout << "\n----- Sample Testcase 20 -----" << endl;
        string config_path = "sa_tc_07_config";
        bool verbose = true;
        DragonWarriorsProgram * program = new DragonWarriorsProgram(config_path);
        cout << "------------------------------------------------------!BEGIN!------------------------------------------------------" << endl;
        cout << "Step:START--" << program->flyteam1->str() << "--|--" << program->flyteam2->str() << "--|--" << program->groundteam->str() << "--|--" << program->dragonlord->str() << endl;
        program->run(verbose);
        cout << "-------------------------------------------------------!END!-------------------------------------------------------" << endl;
        delete program;
    }

    static void satc_21() {
        cout << "\n----- Sample Testcase 21 -----" << endl;
        string config_path = "sa_tc_08_config";
        bool verbose = true;
        DragonWarriorsProgram * program = new DragonWarriorsProgram(config_path);
        cout << "------------------------------------------------------!BEGIN!------------------------------------------------------" << endl;
        cout << "Step:START--" << program->flyteam1->str() << "--|--" << program->flyteam2->str() << "--|--" << program->groundteam->str() << "--|--" << program->dragonlord->str() << endl;
        program->run(verbose);
        cout << "-------------------------------------------------------!END!-------------------------------------------------------" << endl;
        delete program;
    }

    static void satc_22() {
        cout << "\n----- Sample Testcase 22 -----" << endl;
        string config_path = "sa_tc_09_config";
        bool verbose = true;
        DragonWarriorsProgram * program = new DragonWarriorsProgram(config_path);
        cout << "------------------------------------------------------!BEGIN!------------------------------------------------------" << endl;
        cout << "Step:START--" << program->flyteam1->str() << "--|--" << program->flyteam2->str() << "--|--" << program->groundteam->str() << "--|--" << program->dragonlord->str() << endl;
        program->run(verbose);
        cout << "-------------------------------------------------------!END!-------------------------------------------------------" << endl;
        delete program;
    }
};

void test() {
    TestDragonWar::satc_0001();
    TestDragonWar::satc_0002();
    TestDragonWar::satc_0003();
    TestDragonWar::satc_0004();
    TestDragonWar::satc_0005();
    TestDragonWar::satc_0006();
    TestDragonWar::satc_0007();
    TestDragonWar::satc_0008();
    TestDragonWar::satc_0009();
    TestDragonWar::satc_0010();
    TestDragonWar::satc_0011();
    TestDragonWar::satc_0012();
    TestDragonWar::satc_0013();
    TestDragonWar::satc_0014();
    TestDragonWar::satc_0015();
}

int main(int argc, const char * argv[]) {
    // TestDragonWar::satc_01();
    // TestDragonWar::satc_02();
    // TestDragonWar::satc_03();
    TestDragonWar::satc_19();
    TestDragonWar::satc_20();
    TestDragonWar::satc_21();
    TestDragonWar::satc_22();
    // test();
    return 0;
}