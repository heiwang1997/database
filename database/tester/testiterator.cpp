#include "testiterator.h"

testiterator::testiterator()
{
}
string testiterator::RandomString()
{
    int aa = rand();
    stringstream ss;
    ss << aa;
    string s1 = ss.str();
    return s1;
}
string testiterator::InttoString(int num)
{
    int aa = num;
    stringstream ss;
    ss << aa;
    string s1 = ss.str();
    return s1;
}
void testiterator::testindex(Table* onetable, string input)
{
    cout << "test index begin" << endl;
    Iterator* it = IteratorFactory::getiterator(onetable);
    string temp = it->compile(input, 0);
    cout << "try to search " << temp << endl;
    index_key k(temp.c_str(), temp.length());
    index_value v;
    int answer = onetable->getindexes()[0]->search(k, &v);
    if (answer == 0) {
        cout << "has found" << endl;
        cout << v.pagenum << ' ' << v.pageposition << endl;
    } else
        cout << "has not found" << endl;
    cout << "test index end" << endl;
    delete it;
}
void testiterator::begintest()
{
    string filename = "onetable.tb";
    cout << "test iterator table begin..." << endl;
    Table* onetable = new FixedSizeTable();
    onetable->setfilename(filename);
    onetable->setname("testiterator");
    vector<string> clname;
    vector<DataBaseType*> cltype;
    clname.push_back("userid");
    clname.push_back("password");
    clname.push_back("age");
    string t1 = "INTE";
    string t2 = "CHAR";
    string t3 = "INTE";
    string* conditions = new string[3];
    conditions[0] = "FRTO";
    conditions[1] = "0";
    conditions[2] = "900000";
    DataBaseType* type1 = UIC::reconvert(t1.data(), 6, true);
    type1->readcondition(conditions);
    DataBaseType* type2 = UIC::reconvert(t2.data(), 20, true);
    DataBaseType* type3 = UIC::reconvert(t3.data(), 2, true);
    cltype.push_back(type1);
    cltype.push_back(type2);
    cltype.push_back(type3);
    onetable->createTable(clname, cltype);
    onetable->Initialize();
    delete[] conditions;
    onetable->setmajornum(0);
    onetable->setmultivalue(0, false);
    onetable->createemptyindex(0);
    string* aaa = new string[3];
    int pagenum, rownum;
    Record* t = RecordFactory::getrecord(onetable);
    Record* temp = t;
    //    QTime time;
    //    time.start();
    for (int i = 0; i < 200000; i++) {
        aaa[0] = InttoString(i);
        aaa[1] = "12345678";
        aaa[2] = "58";
        bool can = t->set(aaa);
        if (can) {
            can = t->setAt(1, "", true);
            t->update();
        }
        if (can) {
            onetable->FastAllInsert(pagenum, rownum, t);
            //cout<<pagenum<<' '<<rownum<<endl;
        }
    }
    delete t;
    delete[] aaa;
    testindex(onetable, "5");
    testindex(onetable, "0");
    cout << onetable->getPageNum() << endl;
    cout << onetable->getPageRowNum(1030) << endl;
    //    int time_Diff = time.elapsed();
    //    float f = time_Diff / 1000.0;
    //    cout << "Time table: " << f << endl;

    cout << "test table end" << endl;
    cout << "test iterator begin" << endl;

    Iterator* iterator = IteratorFactory::getiterator(onetable);
    Record* record = RecordFactory::getrecord(onetable);
    if (iterator->available()) {
        iterator->getdata(record);

        for (int i = 0; i < 3; i++) {
            cout << "yes: " << i << ' ' << record->getAt(i) << endl;
        }
    }

    ++(*iterator);
    if (iterator->available()) {
        iterator->getdata(record);
        for (int i = 0; i < 3; i++) {
            cout << "yes: " << i << ' ' << record->getAt(i) << endl;
        }
    }
    iterator->access(300, 178);
    if (iterator->available()) {
        iterator->getdata(record);
        for (int i = 0; i < 3; i++) {
            cout << "yes: " << i << ' ' << record->getAt(i) << endl;
        }
    }
    iterator->access(1, 0);
    cout << "try to delete" << endl;
    iterator->deletedata();
    if (iterator->available()) {
        iterator->getdata(record);
        for (int i = 0; i < 3; i++) {
            cout << "yes: " << i << ' ' << record->getAt(i) << endl;
        }
    }
    testindex(onetable, "0");
    testindex(onetable, "19");
    delete iterator;
    delete record;
    delete onetable;

    cout << "Open again" << endl;
    onetable = new FixedSizeTable();
    onetable->setfilename(filename);
    onetable->Initialize();
    iterator = IteratorFactory::getiterator(onetable);
    record = RecordFactory::getrecord(onetable);
    if (iterator->available()) {
        iterator->getdata(record);
        for (int i = 0; i < 3; i++) {
            cout << "yes: " << i << ' ' << record->getAt(i) << endl;
        }
    }
    ++(*iterator);
    if (iterator->available()) {
        iterator->getdata(record);
        for (int i = 0; i < 3; i++) {
            cout << "yes: " << i << ' ' << record->getAt(i) << endl;
        }
    }
    iterator->access(300, 179);
    if (iterator->available()) {
        iterator->getdata(record);
        for (int i = 0; i < 3; i++) {
            cout << "yes: " << i << ' ' << record->getAt(i) << endl;
        }
    }
    delete onetable;
    delete iterator;
    delete record;
    cout << onetable->getname() << endl;
    cout << "test table end" << endl;
}
