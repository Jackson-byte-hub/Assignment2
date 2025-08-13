/*
* Ho Chi Minh City University of Technology
* Faculty of Computer Science and Engineering
* Initial code for Assignment 2
* Programming Fundamentals Summer 2025
* Author: Luong Minh Hien
* Date: 05.08.2025
*/

//The library here is concretely set, students are not allowed to include any other libraries.

#include "dragon.h"

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

    // static void satc_0015() {
    //     cout << "\n----- Sample Testcase 15 -----" << endl;
    //     DragonWarriorsProgram * myProgram = new DragonWarriorsProgram("sa_tc_01_config");
    //     cout << myProgram->flyteam1->str() << endl;
    //     cout << myProgram->flyteam2->str() << endl;
    //     delete myProgram;
    // }
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
        // TestDragonWar::satc_0015();
};