#include <QString>
#include <QtTest>

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
