#include "./lib/QueryProcessor.h"
#include "./lib/QueryResult.h"

using namespace std;

void QueryProcessor::serialSampleQuery(BitmapSet* BTMP, char* text, char* file_path, int column){
    bool* col_pos;
    bool* rec_pos;
    int col = 1;
    unsigned long rec = 1;
    unsigned long counter = 0;
    col_pos = BTMP->bitmapComma;
    rec_pos = BTMP->bitmapLineBreak;
    int abs_len = strlen(text);
    int ref_len = 0;
    unsigned long text_len = 0;
    cout << "enter into BTMP" << endl;
    while(counter < BTMP->length){
        if (*col_pos == 0 && *rec_pos == 0){
            ref_len++;
            col_pos++;
            rec_pos++;
        }
        else if(*col_pos == 1 && *rec_pos == 0){
            if(ref_len == abs_len && col == column){
                FILE* fileID = fopen(file_path,"rb+");
                fseek(fileID,text_len,SEEK_SET);
                void* p;
                if (posix_memalign(&p, 64, ref_len*sizeof(char)) != 0) {
                    cout<<"Memory space is not enough! "<<endl;
                }
                char* record_text = (char*) p;
                size_t load_size = fread (record_text, 1, ref_len, fileID);
                if (load_size == 0) {
                    cout<<"Fail to load the input record into memory! "<<endl;
                }
                fclose(fileID);
                if(strcmp(record_text, text) == 0){
                    cout << "column: " << col << ". record: " << rec << ". text: " << text << ". " <<endl;
                }
            }
            col++;
            text_len += ref_len+1;
            ref_len = 0;
            col_pos++;
            rec_pos++;
        }
        else if(*col_pos == 0 && *rec_pos == 1){
            if(ref_len == abs_len && col == column){
                FILE* fileID = fopen(file_path,"rb+");
                fseek(fileID,text_len,SEEK_SET);
                char* record_text;
                size_t load_size = fread (record_text, 1, ref_len, fileID);
                if (load_size == 0) {
                    cout<<"Fail to load the input record into memory! "<<endl;
                }
                fclose(fileID);
                if(strcmp(record_text, text) == 0){
                    cout << "column: " << col << ". record: " << rec << ". text: " << text << ". " <<endl;
                }
            }
            col = 1;
            rec++;
            text_len += ref_len+1;
            ref_len = 0;
            col_pos++;
            rec_pos++;
        }
        counter++;
    }
}

void QueryProcessor::columnSerialQuery(BitmapSet* BTMP, int* column, int colnumbers, char* file, char* out_file_path){
    // get which columns
    int number = sizeof(column)/sizeof(column[0]);
    int whichCol[number];
    for(int i = 0; i < number; i++)
        whichCol[i] = column[i];
    vector<vector<string>> needCol;
    vector<string> tmp;
    bool* comma = BTMP->bitmapComma;
    bool* lb = BTMP->bitmapLineBreak;
    char* pos = file;
    int initCol = 1;
    int length = 0;
    unsigned long text_len = 0;
    char* tmpchar;
    while(1){
        if(*comma == 0 && *lb == 0){
            length++;
            comma++;
            lb++;
        }
        else if(*comma == 1 && *comma == 0){
            for(int i = 0;i < number; i++){
                if(initCol == whichCol[i]){
                    string str(pos+text_len, length);
                    tmp.push_back(str);
                    break;
                }
            }
            length=0;
            comma++;
            lb++;
            text_len += length+1;
            initCol++;
        }
        else if(*comma == 0 && *lb == 1){
            for(int i = 0;i < number; i++){
                if(initCol == whichCol[i]){
                    string str(pos+text_len, length);
                    tmp.push_back(str);
                    break;
                }
            }
            length=0;
            comma++;
            lb++;
            text_len += length+1;
            initCol = 1;
            needCol.push_back(tmp);
            tmp.clear();
        }
    }
    std::ofstream out_file(out_file_path);
    if (!out_file.is_open()) {
        std::cerr << "Error opening file for writing." << std::endl;
    }
    for (const auto& row : needCol) {
        for (const auto& col : row) {
            out_file << col << ',';
        }
        out_file << '\n';
    }
    out_file.close();
}

DeType* QueryProcessor::deserialization(BitmapSet* BTMP, char* text, unsigned int column){
    bool* comma = BTMP->bitmapComma;
    bool* lb = BTMP->bitmapLineBreak;
    char* pos = text;
    unsigned int ref_col = 1;
    // ignore first line
    while(1){
        if(*lb == 1){
            comma++;
            lb++;
            pos++;
            break;
        }
        else{
            comma++;
            lb++;
            pos++;
        }
    }
    // jumpto signed column
    while(ref_col < column){
        if(*comma == true)
            ref_col++;
        comma++;
        lb++;
        pos++;
    }
    // deserialization
    int sign = 1;
    DeType* type = new DeType();
    char* rectify = pos;
    int wordlength = 0;
    while(*comma != true || *lb != true){
        if(sign == 1){
            if(*pos == '0' || *pos == '1' || *pos == '2' || *pos == '3' || *pos == '4' || *pos == '5' || 
                *pos == '6' || *pos == '7' || *pos == '8' || *pos == '9' || *pos == '-'){
                pos++;
                comma++;
                lb++;
                sign = 2;
                wordlength++;
            }
            else{
                sign = 0;
                break;
            }
        }
        else{
            if(*pos == '0' || *pos == '1' || *pos == '2' || *pos == '3' || *pos == '4' || *pos == '5' || 
                *pos == '6' || *pos == '7' || *pos == '8' || *pos == '9' || *pos == '.'){
                pos++;
                comma++;
                lb++;
                wordlength++;
            }
            else{
                sign = 0;
                break;
            }
        }
    }
    if(sign != 0){
        char* word = new char[wordlength];
        char* isItchar = new char[wordlength];
        for(int g = 0; g < wordlength; g++)
            isItchar[g] = '0';
        int isItInt = atoi(word);
        sprintf(isItchar,"%d",isItInt);
        if(strcmp(word,isItchar) == 0){
            type->isInt = 1;
            type->delete_type = false;
            return type;
        }
        else{
            unsigned int isItUnInt = (long)word;
            for(int g = 0; g < wordlength; g++)
                isItchar[g] = '0';
            sprintf(isItchar,"%d",isItUnInt);
            if(strcmp(word,isItchar) == 0){
                type->isUnInt = 1;
                type->delete_type = false;
                return type;
            }
            else{
                long isItLong = (long)word;
                for(int g = 0; g < wordlength; g++)
                    isItchar[g] = '0';
                sprintf(isItchar,"%ld",isItLong);
                if(strcmp(word,isItchar) == 0){
                    type->isLong = 1;
                    type->delete_type = false;
                    return type;
                }
                else{
                    unsigned long isItUnLong = (unsigned long)word;
                    for(int g = 0; g < wordlength; g++)
                        isItchar[g] = '0';
                    sprintf(isItchar,"%ld",isItUnLong);
                    if(strcmp(word,isItchar) == 0){
                        type->isUnLong = 1;
                        type->delete_type = false;
                        return type;
                    }
                    else{
                        float isItFloat = strtof(word,NULL);
                        for(int g = 0; g < wordlength; g++)
                            isItchar[g] = '0';
                        sprintf(isItchar, "%f", isItFloat);
                        if(strcmp(word,isItchar) == 0){
                            type->isFloat = 1;
                            type->delete_type = false;
                            return type;
                        }
                        else{
                            double isItDouble = strtod(word,NULL);
                            for(int g = 0; g < wordlength; g++)
                                isItchar[g] = '0';
                            sprintf(isItchar, "%f", isItDouble);
                            if(strcmp(word,isItchar) == 0){
                                type->isDouble = 1;
                                type->delete_type = false;
                                return type;
                            }
                        }
                    }
                }
            }
        }
    }
    type->delete_type = false;
    return type;
}

bool QueryProcessor::isNumber(const char* str){
    bool hasDecimal = false;
    bool hasDigit = false;

    if (str == nullptr || *str == '\0') return false;
    if (*str == '+' || *str == '-') str++;
    while (*str) {
        if (isdigit(*str)) {
            hasDigit = true;
        } else if (*str == '.') {
            if (hasDecimal) return false;
            hasDecimal = true;
        } else {
            return false;
        }
        str++;
    }

    return hasDigit;
}

bool QueryProcessor::isScientificNotation(const char* str){
    char* endptr;
    strtod(str, &endptr);

    if (*endptr == '\0' && str != endptr) {
        return true;
    }

    while (isspace((unsigned char)*endptr)) endptr++;
    return *endptr == '\0';
}