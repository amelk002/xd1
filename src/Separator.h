#ifndef __SEPARATOR_H__
#define __SEPARATOR_H__
#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <cmath>
#include <sstream>
#include <stdio.h>
#include <unistd.h>
#include <cstring>
#include <sys/wait.h>
#include <algorithm>
#include "Base.h"
#include "CMD.h"
#include "And.h"
#include "Or.h"

using namespace std;

class Separator {
    private:
        std::vector<Base*> currCmds;

    public:
        Separator();
        ~Separator();
        void parse(std::string parse);
        int execute();
        void Delete();
};
#endif
