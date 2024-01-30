#ifndef INDEXSET_H
#define INDEXSET_H

#include <stdlib.h>

using namespace std;

struct DeType{
    bool isBool;
    int isInt;
    unsigned int isUnInt;
    long isLong;
    unsigned long isUnLong;
    float isFloat;
    double isDouble;
    bool delete_type;

    DeType(){
        bool isBool = false;
        int isInt = 0;
        unsigned int isUnInt = 0;
        long isLong = 0;
        unsigned long isUnLong = 0;
        float isFloat = 0;
        double isDouble = 0;
        delete_type = true;
    }

    ~DeType(){
        if(delete_type == true){
            bool isBool = false;
            int isInt = 0;
            unsigned int isUnInt = 0;
            long isLong = 0;
            unsigned long isUnLong = 0;
            float isFloat = 0;
            double isDouble = 0;
            delete_type = false;
        }
    }
};

#endif