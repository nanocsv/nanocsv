#ifndef INDEXSET_H
#define INDEXSET_H

#include <stdlib.h>
#include <vector>
#include "BitmapSet.h"
#include "ewah.h"

using namespace ewah;
using namespace std;

struct indexSet{
    vector<vector<string>> index;
    bool delete_index;

    indexSet(int n){
        index = vector<vector<string>>(1,vector<string>(n));
        delete_index = true;
    }

    ~indexSet(){
        if(delete_index == true){
            vector<vector<string>> myVector;
            myVector.swap(index);
            delete_index = false;
        }
    }
};

struct bitmapCompressSet{

};

class index{
public:
    static indexSet* sampleIndex();
    static bitmapCompressSet* bitmapCompress(BitmapSet* BTMP, unsigned long length);
};

#endif