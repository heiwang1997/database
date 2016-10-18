#include "fixedsizetable.h"

FixedSizeTable::FixedSizeTable()
{
    this->PageNum = 0;
    this->MaxRecordSize = 0;
    this->MaxRowNum = 0;
    this->RowSize = 0;
    this->RowNumInPage = NULL;
}
int FixedSizeTable::getPageNum() { return PageNum; }
int FixedSizeTable::getMaxRowNum() { return MaxRowNum; }
int FixedSizeTable::getRowSize() { return RowSize; }

bool FixedSizeTable::Initialize()
{
    FM = new FileManager;
    bool canopen = FM->openFile(filename.data(), fileid);
    if (!canopen) {
        bool cancreate = FM->createFile(filename.data());
        if (cancreate) {
            canopen = FM->openFile(filename.data(), fileid);
            if (!canopen)
                return false;
        } else {
            return false;
        }
    }
    BPM = new BufPageManager(FM);
    int index;
    BufType b = BPM->allocPage(fileid, 0, index, true);
    /*
cout<<"Now begin "<<"testment"<<endl;
for (int i=0;i<PAGE_INT_NUM;i++)
{
    int k=b[i];
    cout<<"No."<<i<<" is: "<<k<<endl;
}
cout<<"Now end "<<"testment"<<endl;
*/
    char* temp = (char*)malloc(4);
    memcpy(temp, b, 4);
    string ob1 = "HEAD";
    if (!UIC::equal(temp, ob1.data(), 4) || havecreatetable) {
        PackageHeadFile(b);
        BPM->markDirty(index);
    } else {
        PackageFromHeadFile(b);
        BPM->access(index);
    }
    free(temp);
    haveinitialize = true;
    return true;
}

void FixedSizeTable::PackageFromHeadFile(BufType b)
{
    FileIterator position = 4;
    RowSize = UIC::readint(b, position);
    int namelen = UIC::readint(b, position);
    name = UIC::readstringandjump(b, position, namelen, MAX_NAME_SIZE);
    PageNum = UIC::readint(b, position);
    clearcolumn();
    columncount = UIC::readint(b, position);
    columnname = new string[this->columncount];
    column = new DataBaseType*[this->columncount];
    order = new int[this->columncount];
    for (int i = 0; i < this->columncount; i++) {
        int namelen = UIC::readint(b, position);
        columnname[i] = UIC::readstringandjump(b, position, namelen, MAX_NAME_SIZE);
        char* temptype = (char*)malloc(4);
        UIC::readchar(b, position, temptype, 4);
        int tempsize = UIC::readint(b, position);
        DataBaseType* t = UIC::reconvert(temptype, tempsize);
        column[i] = t;
        free(temptype);
        order[i] = UIC::readint(b, position);
    }

    MaxRowNum = UIC::readint(b, position);
    MaxRecordSize = UIC::readint(b, position);
    if (RowNumInPage != NULL)
        delete[] RowNumInPage;
    RowNumInPage = new int[MaxRecordSize];
    for (int i = 0; i < MaxRecordSize; i++)
        RowNumInPage[i] = UIC::readint(b, position);
}
void FixedSizeTable::PackageHeadFile(BufType b)
{
    FileIterator position = 0;
    string temp = "HEAD";
    UIC::writechar(b, position, temp.data(), 4);
    UIC::writeint(b, position, RowSize);
    int namelen = name.length();
    UIC::writeint(b, position, namelen);
    UIC::writecharandjump(b, position, name.data(), namelen, MAX_NAME_SIZE);
    UIC::writeint(b, position, PageNum);
    UIC::writeint(b, position, columncount);
    for (int i = 0; i < columncount; i++) {
        int namelen = columnname[i].length();
        UIC::writeint(b, position, namelen);
        UIC::writecharandjump(b, position, columnname[i].data(), namelen, MAX_NAME_SIZE);
        char* temptype = (char*)malloc(4);
        UIC::convert(column[i], temptype);
        UIC::writechar(b, position, temptype, 4);
        free(temptype);
        UIC::writeint(b, position, column[i]->getSize());
        UIC::writeint(b, position, order[i]);
    }
    UIC::writeint(b, position, MaxRowNum);
    UIC::writeint(b, position, MaxRecordSize);
    for (int i = 0; i < MaxRecordSize; i++)
        UIC::writeint(b, position, RowNumInPage[i]);
}

void FixedSizeTable::createTable(vector<string> clname, vector<string> cltype,
    vector<int> clsize)
{
    remove(this->filename.c_str());
    int totalheadsize = 4 * 4 + 4 * 3 + MAX_NAME_SIZE;
    this->clearcolumn();
    this->RowSize = 0;
    this->columncount = clname.size();
    columnname = new string[this->columncount];
    column = new DataBaseType*[this->columncount];
    order = new int[this->columncount];
    for (int i = 0; i < columncount; i++) {
        totalheadsize += MAX_NAME_SIZE + 4 * 4;
        columnname[i] = clname[i];
        if (cltype[i] == "CHAR") {
            DataBaseType* temp = new DatabaseChar(clsize[i]);
            column[i] = temp;
        }
        if (cltype[i] == "INT") {
            DataBaseType* temp = new DatabaseInt(clsize[i]);
            column[i] = temp;
        }
        order[i] = i;
        this->RowSize += clsize[i];
    }
    this->MaxRowNum = (PAGE_SIZE - 8) / this->RowSize;
    this->PageNum = 0;
    this->MaxRecordSize = max((PAGE_SIZE - totalheadsize - 16) / 4, 0);
    if (this->RowNumInPage != NULL)
        delete[] RowNumInPage;
    RowNumInPage = new int[MaxRecordSize];
    for (int i = 0; i < this->MaxRecordSize; i++)
        this->RowNumInPage[i] = 0;
    havecreatetable = true;
}
char* FixedSizeTable::Packager()
{
    char* temp = (char*)malloc(this->RowSize);
    int nowsize = 0;
    for (int i = 0; i < columncount; i++) {
        memcpy(temp + nowsize, column[i]->getdata(), column[i]->getSize());
        nowsize += column[i]->getSize();
    }
    return temp;
}
bool FixedSizeTable::InsertAt(int pagenum, char* insertdata, int& rownum)
{
    if (pagenum == 0)
        return false;
    int nowindex = 0;
    BufType b = BPM->getPage(fileid, pagenum, nowindex);
    int position = 0;
    int pagerownum = 0;
    char* temp = (char*)malloc(4);
    string option1 = "PAGE";
    char* option2 = (char*)malloc(4);
    memset(option2, 0, 4);
    memcpy(temp, b + position, 4);
    if (UIC::equal(temp, option1.data(), 4)) {
        pagerownum = UIC::chartoint(b + position + 4);
    } else if (UIC::equal(temp, option2, 4)) {
        memcpy(b + position, option1.data(), 4);
        UIC::inttochar(0, b + position + 4);
        pagerownum = 0;
    } else {
        // cout<<"ERROR:: ????"<<endl;
        memcpy(b + position, option1.data(), 4);
        UIC::inttochar(0, b + position + 4);
        pagerownum = 0;
    }
    free(temp);
    free(option2);
    if (pagerownum == this->MaxRowNum)
        return false;
    position += 8 + this->RowSize * pagerownum;
    rownum = pagerownum;
    memcpy(b + position, insertdata, this->RowSize);
    pagerownum++;
    UIC::inttochar(pagerownum, b + 4);
    if (pagenum < this->MaxRecordSize) {
        this->RowNumInPage[pagenum]++;
    }
    BPM->markDirty(nowindex);
    return true;
}
bool FixedSizeTable::Insert()
{
    char* insertdata = Packager();
    int rownum;
    bool can = false;
    for (int i = 1; i < this->MaxRecordSize; i++)
        if (this->RowNumInPage[i] < this->MaxRowNum) {
            can = InsertAt(i, insertdata, rownum);
            if (!can) {
                cout << "ERROR:: ?????" << endl;
            }
            if (can) {
                if (this->PageNum < i)
                    this->PageNum = i;
                return true;
            }
        }
    for (int i = this->MaxRecordSize; i <= this->PageNum; i++) {
        can = InsertAt(i, insertdata, rownum);
        if (can)
            return true;
    }
    this->PageNum++;
    can = InsertAt(this->PageNum, insertdata, rownum);
    return can;
}
bool FixedSizeTable::modifypd(int pagenum, int rownum, BufType& ct,
    int& newindex, int& pagenewnum)
{
    if (pagenum > this->PageNum || pagenum == 0)
        return false;
    int nowindex = 0;
    BufType b = BPM->getPage(fileid, pagenum, nowindex);
    int position = 0;
    int pagerownum = 0;
    char* temp = (char*)malloc(4);
    string option1 = "PAGE";
    char* option2 = (char*)malloc(4);
    memset(option2, 0, 4);
    memcpy(temp, b + position, 4);
    if (UIC::equal(temp, option1.data(), 4)) {
        pagerownum = UIC::chartoint(b + position + 4);
    } else if (UIC::equal(temp, option2, 4)) {
        free(temp);
        free(option2);
        pagerownum = 0;
        return false;
    } else {
        cout << "ERROR:: ???" << endl;
        free(temp);
        free(option2);
        pagerownum = 0;
        return false;
    }
    free(temp);
    free(option2);
    if (rownum >= pagerownum)
        return false;
    ct = b;
    newindex = nowindex;
    pagenewnum = pagerownum;
    return true;
}

bool FixedSizeTable::DeleteAt(int pagenum, int rownum)
{
    BufType b;
    int nowindex, pagerownum;
    if (!modifypd(pagenum, rownum, b, nowindex, pagerownum))
        return false;
    int position1 = 8 + this->RowSize * (pagerownum - 1);
    int position2 = 8 + this->RowSize * rownum;
    char* temp = (char*)malloc(this->RowSize);
    memcpy(temp, b + position1, this->RowSize);
    memcpy(b + position2, temp, this->RowSize);
    pagerownum--;
    UIC::inttochar(pagerownum, b + 4);
    if (pagenum < this->MaxRecordSize) {
        this->RowNumInPage[pagenum]--;
    }
    BPM->markDirty(nowindex);
    free(temp);
    return true;
}
bool FixedSizeTable::FindAt(int pagenum, int rownum)
{
    BufType b;
    int nowindex, pagerownum;
    if (!modifypd(pagenum, rownum, b, nowindex, pagerownum))
        return false;
    UnPackager(b, 8 + this->RowSize * rownum);
    BPM->access(nowindex);
    return true;
}
void FixedSizeTable::UnPackager(BufType b, int position)
{
    // cout<<"unpack position="<<position<<endl;
    char* temp = (char*)malloc(this->RowSize);
    memcpy(temp, b + position, this->RowSize);
    int totalsize = 0;
    for (int i = 0; i < columncount; i++) {
        // cout<<"unpacker at:"<<totalsize<<endl;
        int size = column[i]->getSize();
        column[i]->checkRightAndChange(temp + totalsize, size);
        totalsize += size;
    }
    free(temp);
}
FixedSizeTable::~FixedSizeTable()
{
    int index = 0;
    BufType b = BPM->getPage(fileid, 0, index);
    PackageHeadFile(b);
    BPM->markDirty(index);
    if (RowNumInPage != NULL)
        delete[] RowNumInPage;
}
bool FixedSizeTable::Modify(int pagenum, int rownum)
{
    BufType b;
    int nowindex, pagerownum;
    if (!modifypd(pagenum, rownum, b, nowindex, pagerownum))
        return false;
    char* temp = Packager();
    int position2 = 8 + this->RowSize * rownum;
    memcpy(b + position2, temp, this->RowSize);
    return true;
}
int FixedSizeTable::getPageRowNum(int pagenum)
{
    //cout << "maxrecordsize=" << this->MaxRecordSize << endl;
    if (pagenum > this->PageNum)
        return 0;
    if (pagenum < this->MaxRecordSize)
        return RowNumInPage[pagenum];
    int index = 0;
    BufType b = BPM->getPage(fileid, pagenum, index);
    int nowrownum = UIC::chartoint(b + 4);
    return nowrownum;
}
bool FixedSizeTable::FastModify(int pagenum, int pageposition, Record* rec)
{
    int index;
    BufType b = BPM->getPage(fileid, pagenum, index);
    memcpy(b + pageposition, rec->getData(), this->RowSize);
    return true;
}
bool FixedSizeTable::FastInsert(int& pagenum, int& pageposition, Record* rec)
{
    int rownum;
    bool can = InsertAt(pagenum, rec->getData(), rownum);
    if (!can)
        return false;
    pageposition = rownum * this->RowSize + 8;
    return true;
}
bool FixedSizeTable::FastAllInsert(int& pagenum, int& pageposition, Record* rec)
{
    bool can = false;
    for (int i = 1; i < this->MaxRecordSize; i++)
        if (this->RowNumInPage[i] < this->MaxRowNum) {
            pagenum = i;
            can = FastInsert(pagenum, pageposition, rec);
            if (!can) {
                cout << "ERROR:: ?????" << endl;
            }
            if (can) {
                if (this->PageNum < i)
                    this->PageNum = i;
                return true;
            }
        }
    for (int i = this->MaxRecordSize; i <= this->PageNum; i++) {
        pagenum = i;
        can = FastInsert(pagenum, pageposition, rec);
        if (can)
            return true;
    }
    this->PageNum++;
    pagenum = this->PageNum;
    can = FastInsert(pagenum, pageposition, rec);
    return can;
}

Record* FixedSizeTable::FastOutput(int pagenum, int pageposition)
{
    int index;
    BufType b = BPM->getPage(fileid, pagenum, index);
    Record* temp = new FixedSizeRecord();
    DataBaseType** t = UIC::copytype(this->column, columncount);
    temp->Initialize(t, columncount);
    temp->Input(b + pageposition);
    return temp;
}
void FixedSizeTable::FastOutput(int pagenum, int pageposition, char* output, int& outputsize)
{
    int index;
    BufType b = BPM->getPage(fileid, pagenum, index);
    outputsize = this->RowSize;
    memcpy(output, b + pageposition, outputsize);
}
string FixedSizeTable::gettabletype()
{
    return "Fixed";
}
