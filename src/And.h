#ifndef __ANDAND_H__
#define __ANDAND_H__
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

using namespace std;

class And: public Base
{
    private:
        Base * lhs;
        Base * rhs;
    public:
        And();
        And(Base* lhs, Command* rhs);
        And(Base* lhs, Base* rhs);
        int execute();
};
#endif
