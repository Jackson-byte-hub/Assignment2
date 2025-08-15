#include "dragons.h"

class TestDragonWar {
public:
    TestDragonWar() = default;

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