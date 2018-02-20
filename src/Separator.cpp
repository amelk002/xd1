#ifndef __SEPARATOR_CPP__
#define __SEPARATOR_CPP__
#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <cmath>
#include <sstream>
#include <stdio.h>
#include <unistd.h>
#include <algorithm>
#include "Separator.h"
#include "Base.h"
#include "CMD.h"
#include "And.h"
#include "Or.h"
#include "Test.h"

using namespace std;

Separator::Separator()
{
    currCmds.resize(0);
}
Separator::~Separator()
{
    for(unsigned i = 0; i < currCmds.size(); ++i)
    {
        delete currCmds.at(i);
    }
}
int Separator::execute()
{
    int temp = 0;
    for(unsigned i = 0; i < currCmds.size(); ++i)
    {
        temp = currCmds.at(i)->execute();
    }
    this->Delete();
    return temp;
}

void Separator::parse(string input)
{
    vector<string> currCs;
    istringstream inSS(input);
    string currString;
    if(count(input.begin(), input.end(), ';') >= 1)
    {
        while(input.find(";") != string::npos)
        {
            if(count(input.begin(), input.end(), ';') == 1
                && input.find(";") != input.size() - 1
                && (input.at(input.find(";") + 1)) == '"')
            {
                break;
            }
            if(count(input.begin(), input.end(), ';') == 1 && input.find(";") == input.size() - 1)
            {
                break;
            }
            bool hashtag = false;
            bool semicolon = false;
            bool lhs = false;
            currCs.resize(0);
            while(!semicolon)
            {
                inSS >> currString;
                input.erase(0, currString.size() + 1);
                if(currString.find("#") != string::npos)
                {
                    hashtag = true;
                    if(currString.find("#") != 0 && (!lhs))
                    {
                        currCs.push_back(currString);
                        lhs = true;
                    }
                }
                if(currString.find(";") != string::npos)
                {
                    semicolon = true;
                }
                string temp = "";
                for(unsigned g = 0; g < currString.size(); ++g)
                {
                    if(currString.at(g) == '"')
                    {
                        if(g > 0 && currString.at(g - 1) == '\\')
                        {
                            temp += currString.at(g);
                        }
                    }
                    else if(currString.at(g) != '\\')
                    {
                        temp += currString.at(g);
                    }
                }
                currString = temp;
                if(!hashtag)
                {
                    if(!semicolon)
                    {
                        currCs.push_back(currString);
                    }
                    else
                    {
                        currString.erase(currString.size() - 1, 1);
                        currCs.push_back(currString);
                    }
                }
            }
            bool detected = false;
            bool parenthesis = true;
            for(unsigned j = 0; j < currCs.size(); ++j)
            {
                if(currCs.at(j) == "||" || currCs.at(j) == "&&")
                {
                    detected = true;
                    break;
                }
                if(currCs.at(j).find("(") != string::npos)
                {
                    parenthesis = true;
                }
            }

            if(detected && !parenthesis)
            {
                string prevConnector;
                string nextConnector;
                vector<string> firstCommand;
                unsigned i = 0;
                while(currCs.at(i) != "&&" && currCs.at(i) != "||")
                {
                    firstCommand.push_back(currCs.at(i));
                    ++i;
                }
                prevConnector = currCs.at(i);
                Base * temp3;
                if(firstCommand.at(0) == "test")
                {
                    temp3 = new Test(firstCommand);
                }
                else
                {
                    temp3 = new Command(firstCommand);
                }
                currCmds.push_back(temp3);
                ++i;
                vector<string> currCommand;
                for(;i < currCs.size(); ++i)
                {
                    currCommand.push_back(currCs.at(i));
                    if(currCs.at(i) == "&&" || currCs.at(i) == "||")
                    {
                        currCommand.pop_back();
                        if(currCommand.at(0) == "test")
                        {
                            Base *temp = new Test(currCommand);
                            nextConnector = currCs.at(i);
                            if(prevConnector == "&&")
                            {
                                Base *temp2 = new And(currCmds.at(
                                    currCmds.size() - 1), temp);
                                currCmds.pop_back();
                                currCmds.push_back(temp2);
                                currCommand.resize(0);
                            }
                            else
                            {
                                Base * temp2 = new Or(currCmds.at(
                                    currCmds.size() - 1), temp);
                                currCmds.pop_back();
                                currCmds.push_back(temp2);
                                currCommand.resize(0);
                            }
                        }
                        else {
                            Base *temp = new Command(currCommand);
                            nextConnector = currCs.at(i);
                            if(prevConnector == "&&")
                            {
                                Base *temp2 = new And(currCmds.at(
                                    currCmds.size() - 1), temp);
                                currCmds.pop_back();
                                currCmds.push_back(temp2);
                                currCommand.resize(0);
                            }
                            else
                            {
                                Base * temp2 = new Or(currCmds.at(
                                    currCmds.size() - 1), temp);
                                currCmds.pop_back();
                                currCmds.push_back(temp2);
                                currCommand.resize(0);
                            }
                        }
                    }
                }
            }
            else if(detected && parenthesis)
            {
                string prevConnector;
                string nextConnector;
                vector<string> currCommand;
                for(unsigned i = 0; i < currCs.size(); ++i)
                {
                    if(currCs.at(i).find("(") != string::npos)
                    {
                        bool found = false;
                        vector<string> split1;
                        vector<string> split2;
                        if(currCs.at(i).find("(") == 0)
                        {
                            currCs.at(i).erase(0, 1);
                        }
                        else
                        {
                            currCs.erase(currCs.begin());
                        }
                        currCommand.push_back(currCs.at(i));
                        ++i;
                        for(; currCs.at(i).find(")") == string::npos; ++i)
                        {
                            currCommand.push_back(currCs.at(i));
                            if(currCs.at(i) == "&&" || currCs.at(i) == "||")
                            {
                                found = true;
                            }
                        }
                        if(found)
                        {
                            if(currCs.at(i).at(0) != ')')
                            {
                                string temp = currCs.at(i);
                                temp.erase(temp.size() - 1, 1);
                                currCs.at(i) = temp;
                                currCommand.push_back(currCs.at(i));
                            }
                            ++i;
                            if(i < currCs.size() - 1)
                            {
                                nextConnector = currCs.at(i);
                            }
                            unsigned t = 0;
                            while(currCommand.at(t) != "&&" && currCommand.at(t) != "||")
                            {
                                ++t;
                            }
                            for(unsigned g = 0; g < t; ++g)
                            {
                                split1.push_back(currCommand.at(g));
                            }
                            for(unsigned g = (t + 1); g < currCommand.size(); ++g)
                            {
                                split2.push_back(currCommand.at(g));
                            }
                            Base* temp1;
                            Base* temp2;
                            if(split1.at(0) == "test")
                            {
                                temp1 = new Test(split1);
                            }
                            else
                            {
                                temp1 = new Command(split1);
                            }
                            if(split2.at(0) == "test")
                            {
                                temp2 = new Test(split2);
                            }
                            else
                            {
                                temp2 = new Command(split2);
                            }
                            Base * temp3;
                            if(currCommand.at(t) == "&&")
                            {
                                temp3 = new And(temp1, temp2);
                            }
                            else
                            {
                                temp3 = new Or(temp1, temp2);
                            }
                            Base* temp4;
                            if(prevConnector == "")
                            {
                                currCmds.push_back(temp3);
                                prevConnector = nextConnector;
                            }
                            else if(prevConnector == "&&")
                            {
                                temp4 = new And(
                                    currCmds.at(currCmds.size() - 1), temp3);
                                currCmds.pop_back();
                                currCmds.push_back(temp4);
                                prevConnector = nextConnector;
                            }
                            else
                            {
                                temp4 = new Or(
                                    currCmds.at(currCmds.size() - 1), temp3);
                                currCmds.pop_back();
                                currCmds.push_back(temp4);
                                prevConnector = nextConnector;
                            }
                            currCommand.resize(0);
                        }
                        else
                        {
                            Base *temp;
                            if(currCommand.at(0) == "(")
                            {
                                currCommand.erase(currCommand.begin());
                            }
                            else if(currCommand.at(0).find("(") != string::npos)
                            {
                                currCommand.at(0).erase(0, 1);
                            }
                            if(currCommand.at(currCommand.size() - 1) == ")")
                            {
                                currCommand.pop_back();
                            }
                            else if(currCommand.at(currCommand.size() - 1).find(")") != string::npos)
                            {
                                currCommand.at(currCommand.size() - 1).erase(currCommand.size() - 1, 1);
                            }
                            if(currCommand.at(0) == "test")
                            {
                                temp = new Test(currCommand);
                            }
                            else {
                                temp = new Command(currCommand);
                            }
                            currCmds.push_back(temp);
                        }
                    }
                    else
                    {
                        currCommand.push_back(currCs.at(i));
                        if(currCs.at(i) == "&&" || currCs.at(i) == "||")
                        {
                            nextConnector = currCs.at(i);
                            currCommand.pop_back();
                            if(prevConnector == "")
                            {
                                Base* temp5;
                                if(currCommand.at(0) == "test")
                                {
                                    temp5 = new Test(currCommand);
                                }
                                else
                                {
                                    temp5 = new Command(currCommand);
                                }
                                currCmds.push_back(temp5);
                                currCommand.resize(0);
                            }
                            else if(prevConnector == "&&")
                            {
                                Base* temp6;
                                if(currCommand.at(0) == "test")
                                {
                                    temp6 = new Test(currCommand);
                                }
                                else
                                {
                                    temp6 = new Command(currCommand);
                                }
                                Base * temp5 = new And(
                                    currCmds.at(currCmds.size() - 1), temp6);
                                currCmds.pop_back();
                                currCmds.push_back(temp5);
                                currCommand.resize(0);
                            }
                            else
                            {
                                Base* temp6;
                                if(currCommand.at(0) == "test")
                                {
                                    temp6 = new Test(currCommand);
                                }
                                else
                                {
                                    temp6 = new Command(currCommand);
                                }
                                Base * temp5 = new Or(
                                    currCmds.at(currCmds.size() - 1), temp6);
                                currCmds.pop_back();
                                currCmds.push_back(temp5);
                                currCommand.resize(0);
                            }
                            prevConnector = nextConnector;
                            currCommand.resize(0);
                        }
                    }
                }
            }
            else {
                vector<string> currCommand;
                for(unsigned k = 0; k < currCs.size(); ++k)
                {
                    currCommand.push_back(currCs.at(k));
                }
                Base *temp;
                if(currCommand.at(0) == "(")
                {
                    currCommand.erase(currCommand.begin());
                }
                else if(currCommand.at(0).find("(") != string::npos)
                {
                    currCommand.at(0).erase(0, 1);
                }
                if(currCommand.at(currCommand.size() - 1) == ")")
                {
                    currCommand.pop_back();
                }
                else if(currCommand.at(currCommand.size() - 1).find(")") != string::npos)
                {
                    currCommand.at(currCommand.size() - 1).erase(currCommand.size() - 1, 1);
                }
                if(currCommand.at(0) == "test")
                {
                    temp = new Test(currCommand);
                }
                else {
                    temp = new Command(currCommand);
                }
                currCmds.push_back(temp);
            }
        }
    }

    currCs.resize(0);
    while(inSS >> currString)
    {
        if(currString.at(currString.size() - 1) == ';')
        {
            currString.erase(currString.size() - 1, 1);
        }
        string temp = "";
        for(unsigned g = 0; g < currString.size(); ++g)
        {
            if(currString.at(g) == '"')
            {
                if(g > 0 && currString.at(g - 1) == '\\')
                {
                    temp += currString.at(g);
                }
            }
            else if(currString.at(g) != '\\')
            {
                temp += currString.at(g);
            }
        }
        currString = temp;
        if(currString.find("#") != string::npos)
        {
            if(currString.find("#") != 0)
            {
                currCs.push_back(currString);
            }
            break;
        }
        currCs.push_back(currString);
    }
    bool detected = false;
    bool parenthesis = false;
    for(unsigned j = 0; j < currCs.size(); ++j)
    {
        if(currCs.at(j) == "&&" || currCs.at(j) == "||")
        {
            detected = true;
        }
        if(currCs.at(j).find("(") != string::npos)
        {
            parenthesis = true;
        }
    }

    if(detected && !parenthesis)
    {
        string prevConnector;
        string nextConnector;
        vector<string> firstCommand;
        unsigned i = 0;
        while(currCs.at(i) != "&&" && currCs.at(i) != "||")
        {
            firstCommand.push_back(currCs.at(i));
            ++i;
        }
        prevConnector = currCs.at(i);
        Base *temp3;
        if(firstCommand.at(0) == "test")
        {
            temp3 = new Test(firstCommand);
        }
        else {
            temp3 = new Command(firstCommand);
        }
        currCmds.push_back(temp3);
        ++i;
        vector<string> currCommand;
        for(; i < currCs.size(); ++i)
        {
            currCommand.push_back(currCs.at(i));
            if(currCs.at(i) == "&&" || currCs.at(i) == "||")
            {
                currCommand.pop_back();
                Base *temp;
                if(currCommand.at(0) == "test")
                {
                    temp = new Test(currCommand);
                }
                else {
                    temp = new Command(currCommand);
                }
                nextConnector = currCs.at(i);
                if(prevConnector == "&&")
                {
                    Base *temp2 = new And(currCmds.at(currCmds.size() - 1),
                        temp);
                    currCmds.pop_back();
                    currCmds.push_back(temp2);
                    prevConnector = nextConnector;
                    currCommand.resize(0);
                }
                else
                {
                    Base * temp2 = new Or(currCmds.at(currCmds.size() - 1),
                        temp);
                    currCmds.pop_back();
                    currCmds.push_back(temp2);
                    prevConnector = nextConnector;
                    currCommand.resize(0);
                }
            }
        }
        Base * temp;
        if(currCommand.at(0) == "test")
        {
            temp = new Test(currCommand);
        }
        else {
            temp = new Command(currCommand);
        }
        if(prevConnector == "&&")
            {
                Base *temp2 = new And(currCmds.at(currCmds.size() - 1),
                    temp);
                currCmds.pop_back();
                currCmds.push_back(temp2);
                currCommand.resize(0);
            }
        else
        {
            Base * temp2 = new Or(currCmds.at(currCmds.size() - 1), temp);
            currCmds.pop_back();
            currCmds.push_back(temp2);
        }
    }
    else if(detected && parenthesis)
    {
        string prevConnector;
        string nextConnector;
        vector<string> currCommand;
        for(unsigned i = 0; i < currCs.size(); ++i)
        {
            if(currCs.at(i).find("(") != string::npos)
            {
                bool found = false;
                vector<string> split1;
                vector<string> split2;
                if(currCs.at(i).find("(") == 0)
                {
                    currCs.at(i).erase(0, 1);
                }
                else
                {
                    currCs.erase(currCs.begin());
                }
                currCommand.push_back(currCs.at(i));
                ++i;
                for(; currCs.at(i).find(")") == string::npos; ++i)
                {
                    currCommand.push_back(currCs.at(i));
                    if(currCs.at(i) == "&&" || currCs.at(i) == "||")
                    {
                        found = true;
                    }
                }
                if(found)
                {
                    if(currCs.at(i).at(0) != ')')
                    {
                        string temp = currCs.at(i);
                        temp.erase(temp.size() - 1, 1);
                        currCs.at(i) = temp;
                        currCommand.push_back(currCs.at(i));
                    }
                    ++i;
                    if(i < currCs.size() - 1)
                    {
                        nextConnector = currCs.at(i);
                    }
                    unsigned t = 0;
                    while(currCommand.at(t) != "&&" && currCommand.at(t) != "||")
                    {
                        ++t;
                    }
                    for(unsigned g = 0; g < t; ++g)
                    {
                        split1.push_back(currCommand.at(g));
                    }
                    for(unsigned g = (t + 1); g < currCommand.size(); ++g)
                    {
                        split2.push_back(currCommand.at(g));
                    }
                    Base* temp1;
                    Base* temp2;
                    if(split1.at(0) == "test")
                    {
                        temp1 = new Test(split1);
                    }
                    else
                    {
                        temp1 = new Command(split1);
                    }
                    if(split2.at(0) == "test")
                    {
                        temp2 = new Test(split2);
                    }
                    else
                    {
                        temp2 = new Command(split2);
                    }
                    Base * temp3;
                    if(currCommand.at(t) == "&&")
                    {
                        temp3 = new And(temp1, temp2);
                    }
                    else
                    {
                        temp3 = new Or(temp1, temp2);
                    }
                    Base* temp4;
                    if(prevConnector == "")
                    {
                        currCmds.push_back(temp3);
                        prevConnector = nextConnector;
                    }
                    else if(prevConnector == "&&")
                    {
                        temp4 = new And(
                            currCmds.at(currCmds.size() - 1), temp3);
                        currCmds.pop_back();
                        currCmds.push_back(temp4);
                        prevConnector = nextConnector;
                    }
                    else
                    {
                        temp4 = new Or(
                            currCmds.at(currCmds.size() - 1), temp3);
                        currCmds.pop_back();
                        currCmds.push_back(temp4);
                        prevConnector = nextConnector;
                    }
                    currCommand.resize(0);
                }
                else
                {
                    Base *temp;
                    if(currCommand.at(0) == "(")
                    {
                        currCommand.erase(currCommand.begin());
                    }
                    else
                    {
                        currCommand.at(0).erase(0, 1);
                    }
                    if(currCommand.at(currCommand.size() - 1) == ")")
                    {
                        currCommand.pop_back();
                    }
                    else if(currCommand.at(currCommand.size() - 1).find(")") != string::npos)
                    {
                        currCommand.at(currCommand.size() - 1).erase(currCommand.size() - 1, 1);
                    }
                    if(currCommand.at(0) == "test")
                    {
                        temp = new Test(currCommand);
                    }
                    else {
                        temp = new Command(currCommand);
                    }
                    currCmds.push_back(temp);
                }
            }
            else
            {
                currCommand.push_back(currCs.at(i));
                if(currCs.at(i) == "&&" || currCs.at(i) == "||")
                {
                    nextConnector = currCs.at(i);
                    currCommand.pop_back();
                    if(prevConnector == "")
                    {
                        Base* temp5;
                        if(currCommand.at(0) == "test")
                        {
                            temp5 = new Test(currCommand);
                        }
                        else
                        {
                            temp5 = new Command(currCommand);
                        }
                        currCmds.push_back(temp5);
                        currCommand.resize(0);
                    }
                    else if(prevConnector == "&&")
                    {
                        Base* temp6;
                        if(currCommand.at(0) == "test")
                        {
                            temp6 = new Test(currCommand);
                        }
                        else
                        {
                            temp6 = new Command(currCommand);
                        }
                        Base * temp5 = new And(
                            currCmds.at(currCmds.size() - 1), temp6);
                        currCmds.pop_back();
                        currCmds.push_back(temp5);
                        currCommand.resize(0);
                    }
                    else
                    {
                        Base* temp6;
                        if(currCommand.at(0) == "test")
                        {
                            temp6 = new Test(currCommand);
                        }
                        else
                        {
                            temp6 = new Command(currCommand);
                        }
                        Base * temp5 = new Or(
                            currCmds.at(currCmds.size() - 1), temp6);
                        currCmds.pop_back();
                        currCmds.push_back(temp5);
                        currCommand.resize(0);
                    }
                    prevConnector = nextConnector;
                    currCommand.resize(0);
                }
            }
        }
    }
    else {
        bool yoyo = false;
        vector<string> currCommand;
        if(currCs.at(0) == "(")
        {
            yoyo = true;
            currCs.erase(currCs.begin());
        }
        if(currCs.at(0).at(0) == '(')
        {
            yoyo = true;
            currCs.at(0).erase(0, 1);
        }
        if(currCs.at(currCs.size() - 1) == ")" && yoyo)
        {
            currCs.erase(currCs.end());
        }
        if(currCs.at(currCs.size() - 1).at(currCs.at(currCs.size() - 1).size() - 1) == ')' && yoyo)
        {
            currCs.at(currCs.size() - 1).erase(currCs.at(currCs.size() - 1).size() - 1, 1);
        }
        for(unsigned k = 0; k < currCs.size(); ++k)
        {
            currCommand.push_back(currCs.at(k));
        }
        Base *temp;
        if(currCommand.at(0) == "test")
        {
            temp = new Test(currCommand);
        }
        else {
            temp = new Command(currCommand);
        }
        currCmds.push_back(temp);
    }
}

void Separator::Delete()
{
    for(unsigned i = 0; i < currCmds.size(); ++i)
    {
        delete currCmds.at(i);
    }
    this->currCmds.resize(0);

}
#endif
