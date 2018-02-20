#ifndef __BASE_H__
#define __BASE_H__
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

using namespace std;

class Base {

    public:
        Base();
        virtual ~Base();
        virtual int execute() = 0;
};
#endif
