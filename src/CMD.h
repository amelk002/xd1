#ifndef __CMD_H__
#define __CMD_H__
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

using namespace std;


class Command: public Base {

  protected:
    int runStat;
    std::vector<std::string> cmdVec;
  public:
    Command();
    Command(std::vector<std::string> currCommand);
    int execute();
    int getrunstat();
    int redirection(bool pipes, int numpipes);
};
#endif
