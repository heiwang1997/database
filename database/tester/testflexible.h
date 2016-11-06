#ifndef TESTFLEXIBLE_H
#define TESTFLEXIBLE_H

#include "databasehandler/database.h"
#include "databasehandler/fixedsizetable.h"
#include "databasehandler/flexibletable.h"
#include "managementhandler/iteratorfactory.h"
#include "recordhandler/recordfactory.h"
#include <QTime>
class testflexible {
public:
    testflexible();
    string RandomString();
    string InttoString(int num);
    void begintest();
};

#endif // TESTFLEXIBLE_H
