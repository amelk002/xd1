#include <iostream>
#include <pwd.h>
#include <sys/types.h>
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
#include "Separator.h"

using namespace std;



int main()
{
    char *userName = getlogin();
        if(!userName) {
            perror("getlogin() error");
        }
    char hostName[1000];
    gethostname(hostName, sizeof hostName);
    cout << userName << "@" << hostName << "$ ";
    string input;
    getline(cin, input);
    Separator mainProcess;
    while(input != "exit")
    {
        mainProcess.parse(input);
        mainProcess.execute();
        mainProcess.Delete();
        cout << userName << "@" << hostName << "$ ";
        getline(cin, input);
    }
    return 0;
}
