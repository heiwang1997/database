#ifndef TABLE_H
#define TABLE_H
#include "../DBIndex/db_index.h"
#include "../managementhandler/uic.h"
#include "../pagefilesystem/BufPageManager.h"
#include "../recordhandler/record.h"
#include "../typehandler/databasechar.h"
#include "../typehandler/databaseint.h"
#include "../typehandler/databasetype.h"
#include "../DBIndex/comparealgo.h"
#include "bulbfile.h"
#include <string>
#include <vector>
using namespace std;
#define MAX_NAME_SIZE 256
#define make_triple(x, y, z) make_pair(x, make_pair(y, z))
class Table {
public:
    Table();
    Table(string name, string filename);
    string getname();
    string getfilename();
    BufPageManager* getBPM();
    int getfileid();
    int getmajornum();
    void setmajornum(int nownum);
    db_index** getindexes();
    DataBaseType* getcolumn(int i);
    string getcolumnname(int i);
    DataBaseType** getcolumns();
    void setmultivalue(int i, bool istrue);
    bool getmultivalue(int i);
    int getcolumncount();
    bool setname(string name);
    void setfilename(string filename);
    bool checkInsert(vector<string> data);
    void readindex();
    void Nullindex();
    void createemptyindex(int i);
    void deleteindex(int i);
    int getColumnIndexByName(const std::string& name);
    vector<string> checkOutput();
    virtual bool FastAllInsert(int& pagenum, int& rownum, Record* rec) = 0;
    virtual bool FastOutput(int pagenum, int rownum, Record* rec) = 0;
    virtual void FastOutput(int pagenum, int rownum, char* output, int& outputsize) = 0;
    virtual bool Initialize() = 0;
    virtual string gettabletype() = 0;
    bool havecreatetable = false;
    bool haveinitialize = false;
    virtual int getPageNum() = 0;
    virtual int getPageRowNum(int pagenum) = 0;
    virtual int getRowSize(int rownum) = 0;
    virtual int getMaxRowNum() = 0;
    virtual bool DeleteAt(int pagenum, int rownum) = 0;
    virtual void createTable(vector<string> clname, vector<DataBaseType*> cltype) = 0;
    vector<string> gettype();
    virtual ~Table();
    virtual void createindex(vector<int> columnnum)
    {
        cout << "Please don't use it in non-in-file table" << endl;
        for (int i : columnnum) {
            createemptyindex(i);
        }
    }

    virtual int getinfo(int reqhashnum, int pagenum, int rownum, vector<int>* infovec) = 0;

    virtual bool FastFind(Record* rec)
    {
        cout << "Please don't use it in non-hashflexible table" << endl;
        return false;
    }

    virtual pair<int,int> FastFindPosition(string input)
    {
        cout << "Please don't use it in non-hashflexible table" << endl;
        return make_pair(0,0);
    }
    vector<pair<int, pair<int, string> > >* gettablecondition();
    vector<vector<pair<string, int> > >* getlinkedcolumn();
    vector<pair<string,int>>* getforeignkeys();

    virtual unsigned long getTraverseCost() = 0;

protected:
    void clearcolumn();
    void clearindex();
    void InsertindexAt(int num, char* insertdata, int datalen, int pagenum, int rownum);
    void ModifyindexAt(int num, char* modifydata, int datalen, int prepagenum, int prerownum, int newpagenum, int newrownum);
    void DeleteindexAt(int num, char* deletedata, int datalen, int pagenum, int rownum);
    string name;
    string filename;
    int fileid;
    int columncount;
    DataBaseType** column; //per size if 8
    string* columnname;
    int majornum;
    BufPageManager* BPM;
    FileManager* FM;
    db_index** DBindex;
    bool* multivalue;
    vector<pair<int, pair<int, string> > > tablecondition;
    vector<vector<pair<string, int> > > linkedcolumn;
    vector<pair<string,int>> foreignkeys;
};

#endif // TABLE_H
