#include "mainwindow.h"
#include "tester/testdatabase.h"
#include "tester/testiterator.h"
#include "tester/testtable.h"
#include "tester/testtype.h"
#include <QApplication>
#include <QQmlExtensionPlugin>
#include <iostream>
using namespace std;
int main(int argc, char* argv[])
{
    testtype t;
    t.begintest(false);
    testtable u;
    u.begintest();
    testdatabase v;
    v.begintest();
    testiterator w;
    w.begintest();
    return 0;
}
