#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include "../src/csv_reader.h"

int main(int argc, char** argv){
        char* filename;
        bool headLine;
        int avaliableCore;
        char* queryText;
        int column;

        filename = (char*) argv[1];
        stringstream ss(argv[2]);
        if(!(ss >> boolalpha >> headLine)){
                cout << "Error" << endl;
        }
        avaliableCore = atoi(argv[3]);
        queryText = (char*) argv[4];
        column = atoi(argv[5]);
        cout << "|--file_path: " << filename << "--";
        cout << "|--head_line: " << headLine << "--";
        cout << "|--avaliable_core: " << avaliableCore << "--";
        cout << "|--query_text: " << queryText << "--";
        cout << "|--column_number: " << column << "--|" << endl;
        overview_csv(filename, headLine, avaliableCore, queryText, column);
}
