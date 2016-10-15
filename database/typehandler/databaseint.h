#ifndef DATABASEINT_H
#define DATABASEINT_H
#include "type.h"
#include <cstring>
#include <bitset>
#include <cstdlib>
#include <cstdio>
using namespace std;
#define MAX_INT_SIZE 255
class DatabaseInt : public type
{
public:
    DatabaseInt(int size);
    DatabaseInt(string input);
    DatabaseInt(char* input, int size);
    bool checkRight(string input);
    bool checkRightAndChange(string input);
    bool checkRight(char* input,int size);
    bool checkRightAndChange(char* input,int size);
    int getSize();
    string getType();
    ~DatabaseInt();
    string output();
private:
    void change(string input);
    void change(char* input,int size);
};

#endif // DATABASEINT_H