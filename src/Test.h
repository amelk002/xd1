#ifndef TEST_H
#define TEST_H
#include "CMD.h"
class Test : public Base {
    private:
        int runStat;
        std::vector<std::string> cmdVec;
    public:
        Test(std::vector<std::string> commandVec);
        int execute();
        int getrunstat();
};
#endif
