#include "Tests.h"

#include <QTest>

int main()
{
    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();

    Tests tests;

    QTest::qExec(&tests);

    return 0;

}
