#include "clangutils.h"

#include <QString>
#include <QtTest>

#include <clang-c/Index.h>

class LibClangTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void parseFile();
};

void LibClangTest::initTestCase()
{
    clang_toggleCrashRecovery(1);
    clang_enableStackTraces();
}

void LibClangTest::cleanupTestCase()
{
}

void LibClangTest::parseFile()
{
    // excludeDeclarationsFromPCH allows to remove redundant call backs
    const int excludeDeclarationsFromPCH = 1; // TODO: Test later!
    const int displayDiagnostics = 1;
    CXIndex index = clang_createIndex(excludeDeclarationsFromPCH, displayDiagnostics);

    // Collect information for options
    const QString testFilePath = SRCDIR "testdata/main.cpp";
    QVERIFY(QFile::exists(testFilePath));
    const QString optionsString = testFilePath;
    const QStringList optionsStringList = optionsString.split(QLatin1Char(' '),
                                                              QString::SkipEmptyParts);
    const Utils::ScopedClangOptions options(optionsStringList);

    // Parse
    CXTranslationUnit tu;
    const CXErrorCode errorCode
        = clang_parseTranslationUnit2(index, NULL, options.data(), options.size(), 0, 0,
                                      CXTranslationUnit_None, &tu);
    QCOMPARE(errorCode, CXError_Success);

    // Process diagnostics
    for (unsigned I = 0, N = clang_getNumDiagnostics(tu); I != N; ++I) {
        const CXDiagnostic diagnostic = clang_getDiagnostic(tu, I);
        const CXString diagnosticString = clang_formatDiagnostic(diagnostic,
                                                                 clang_defaultDiagnosticDisplayOptions());
        fprintf(stderr, "%s\n", clang_getCString(diagnosticString));
        clang_disposeString(diagnosticString);
    }

    // Clean up
    clang_disposeTranslationUnit(tu);
    clang_disposeIndex(index);
}

QTEST_APPLESS_MAIN(LibClangTest)

#include "tst_libclangtest.moc"
