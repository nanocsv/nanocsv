#include "./lib/RecordSet.h"
#include "./lib/RecordLoader.h"
#include "./lib/CSVPathParser.h"
#include "./lib/QueryProcessor.h"

using namespace std;

struct timespec time_start = {0, 0}, time_io = {0, 0}, time_sample = {0, 0}, time_parse = {0, 0}, time_query = {0, 0};

class overview{
public:
        void overview_csv(char* filename, bool headline, int avcore, char* keyword, int column);
};

void overview_csv(char* filename, bool headline, int avcore, char* keyword, int column){
        RecordSet* fl = new RecordSet();
        clock_gettime(CLOCK_REALTIME, &time_start);
        fl = RecordLoader::loadRecords(filename, headline);
        clock_gettime(CLOCK_REALTIME, &time_io);
        blSize* block;
        block = blSize::matchingBlock(fl->rec_size, avcore);
        BitmapSet* btmp;
	int len = strlen(keyword);
        cout << "bitmap build" << endl;
        btmp = bitmap::bmAlloc(fl->rec_size, block->getBlNum(), block->getBlSize(), fl->quotesample, fl->text, avcore);
        clock_gettime(CLOCK_REALTIME, &time_parse);

        if(strcmp(keyword,"none")!=0){
                QueryProcessor::serialSampleQuery(btmp, keyword, filename, column);
                cout << "Query Sucess!" << endl;
        }
        else
                cout << "none means not query" << endl;
        double timeStart, timeIO, timeParse;
        timeStart = time_start.tv_sec + time_start.tv_nsec*0.000000001;
        timeIO = time_io.tv_sec + time_io.tv_nsec*0.000000001;
        timeParse = time_parse.tv_sec + time_parse.tv_nsec*0.000000001;
        cout << "|--IO time: " << timeIO-timeStart << "--";
        cout << "|--Parse time: " << timeParse-timeIO << "--";
        cout << "|--Total time: " << timeParse-timeStart  << "--|" << endl;
}
