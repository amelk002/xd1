#ifndef __OR_H__
#define __OR_H__
#include "Base.h"
#include "CMD.h"

using namespace std;

class Or: public Base
{

    private:
        Base * lhs;
        Base * rhs;
    public:
        Or();
        Or(Base* lhs, Command* rhs);
        Or(Base* lhs, Base* rhs);
        int execute();
};
#endif
