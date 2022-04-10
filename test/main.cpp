#include "Tests.h"

#include <QTest>

int main(int argc, char **argv)
{
    Tests tests;

    QTest::qExec(&tests);

    return 0;

}
