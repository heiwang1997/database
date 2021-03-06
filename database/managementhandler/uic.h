#ifndef UIC_H
#define UIC_H

#include "../typehandler/databasebulb.h"
#include "../typehandler/databasechar.h"
#include "../typehandler/databasedate.h"
#include "../typehandler/databaseint.h"
#include "../typehandler/databaselint.h"
#include "../typehandler/databasereal.h"
#include "../typehandler/databasetext.h"
#include "../typehandler/databasetype.h"
#include "../typehandler/databasevarchar.h"
#include "../sqlengine/sqlstruct.h"
#include "../typehandler/chrono_io.h"
#include "../typehandler/date.h"
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
using namespace date;
using namespace std::chrono;
#include <sstream>
#include <vector>

typedef int FileIterator;
typedef char* BufType;
class UIC {
public:
    UIC();
    static bool equal(char* s1, char* s2, int len);
    static bool equal(char* s1, const char* s2, int len);
    static bool equal(const char* s1, char* s2, int len);
    static void convert(DataBaseType* t1, char* s1, char* s2);
    static DataBaseType* reconvertc(char* s1, int size, bool cannull);
    static DataBaseType* reconvert(string s1, int size, bool cannull);
    static DataBaseType* realreconvertc(char* s1, int size, bool cannull);
    static DataBaseType* realreconvert(string s1, int size, bool cannull);
    static void convertmulti(bool can, char* s1);
    static int chartoint(char* s1); //length = 4
    static void inttochar(int i1, char* s1); //length = 4
    static void readchar(BufType b, FileIterator& iterator, char* dest, int size_t);
    static int readint(BufType b, FileIterator& iterator);
    static void writechar(BufType b, FileIterator& iterator, char* source, int size_t);
    static void writechar(BufType b, FileIterator& iterator, const char* source, int size_t);
    static void writecharandjump(BufType b, FileIterator& iterator, char* source, int size_t, int jumpsize);
    static void writecharandjump(BufType b, FileIterator& iterator, const char* source, int size_t, int jumpsize);
    static void writeint(BufType b, FileIterator& iterator, int source);
    static string readstring(BufType b, FileIterator& iterator, int size_t);
    static string readstringandjump(BufType b, FileIterator& iterator, int size_t, int jumpsize);
    static DataBaseType** copytype(DataBaseType** input, int inputlen);
    static int stringtoint(string text);
    static string inttostring(int text);
    static DataBaseType *copyType(DataBaseType *oriType);
    static std::vector<std::string> stringSplit(const std::string &s,
                                              const std::string &delim);
    static string getUserOutput(char type, string input, bool isNull);
    static string getUserOutput(SQLValue::LiteralType type, string input);
};

#endif // UIC_H
