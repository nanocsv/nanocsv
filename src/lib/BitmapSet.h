#ifndef BITMAPSET_H
#define BITMAPSET_H

#include <stdlib.h>

using namespace std;

struct BitmapSet{
    bool* bitmapComma;
    bool* bitmapLineBreak;
    unsigned long length;
    bool delete_bitmap;

    BitmapSet(){
        bitmapComma = NULL;
        bitmapLineBreak = NULL;
        length = 0;
        delete_bitmap = true;
    }

    ~BitmapSet(){
        if(delete_bitmap == true && (bitmapComma != NULL || bitmapLineBreak != NULL)){
            free(bitmapComma);
            free(bitmapLineBreak);
            bitmapComma = NULL;
            bitmapLineBreak = NULL;
            length = 0;
            delete_bitmap = false;
        }
    }
};
#endif