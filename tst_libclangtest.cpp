#include <QString>
#include <QtTest>

#include <clang-c/Index.h>

class LibClangTest : public QObject
{
    Q_OBJECT

public:
    LibClangTest();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
};

LibClangTest::LibClangTest()
{
}

void LibClangTest::initTestCase()
{
    clang_toggleCrashRecovery(1);
    clang_enableStackTraces();
}

void LibClangTest::cleanupTestCase()
{
}

void LibClangTest::testCase1()
{
    QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(LibClangTest)

#include "tst_libclangtest.moc"
