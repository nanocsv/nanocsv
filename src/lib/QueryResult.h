#ifndef QUERYPRESULT_H
#define QUERYPRESULT_H

#include <vector>
#include <string>

using namespace std;

struct queryResult{
    vector<unsigned int> col;
    vector<unsigned int> rec;
    vector<string> text;
    bool sig;
    queryResult(){
        col.resize(1);
        rec.resize(1);
        text.resize(1);
        sig = false;
    }
};

#endif