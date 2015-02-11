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

static QString toQString(const CXDiagnosticSeverity severity)
{
    switch (severity) {
    case CXDiagnostic_Ignored:
        return QLatin1String("ignored");
        break;
    case CXDiagnostic_Note:
        return QLatin1String("note");
        break;
    case CXDiagnostic_Warning:
        return QLatin1String("warning");
        break;
    case CXDiagnostic_Error:
        return QLatin1String("error");
        break;
    case CXDiagnostic_Fatal:
        return QLatin1String("fatal");
        break;
    default:
        return QString("<Unknown>");
        break;
    }
}

static QString toQString(const CXString &string)
{
    return QString::fromUtf8(clang_getCString(string)); // TOOD: Leak
}

static QString toQString(const CXFile &file)
{
    return toQString(clang_getFileName(file)); // TODO: Leak
}

static QString toQString(const CXSourceLocation &sourceLocation)
{
    unsigned line, column, offset;
    CXFile file;
    clang_getSpellingLocation(sourceLocation, &file, &line, &column, &offset);
    return QString::fromUtf8("%1:%2:%3:%4").arg(toQString(file),
                                                QString::number(line),
                                                QString::number(column),
                                                QString::number(offset));
}

static void printDiagnosticsAndFixIts(const CXTranslationUnit &translationUnit)
{
    for (unsigned i = 0, n = clang_getNumDiagnostics(translationUnit); i != n; ++i) {
        const CXDiagnostic diagnostic = clang_getDiagnostic(translationUnit, i);
        const CXDiagnosticSeverity diagnosticSeverity = clang_getDiagnosticSeverity(diagnostic);
        const CXSourceLocation diagnosticLocation = clang_getDiagnosticLocation(diagnostic);
        const CXString diagnosticSpelling = clang_getDiagnosticSpelling(diagnostic);
        qDebug() << "Diagnostic"
                 << toQString(diagnosticLocation)
                 << toQString(diagnosticSeverity)
                 << toQString(diagnosticSpelling);


        for (int i = 0, n = clang_getDiagnosticNumFixIts(diagnostic); i < n; ++i) {
            CXSourceRange replacementRange;
            const CXString text = clang_getDiagnosticFixIt(diagnostic, i, &replacementRange);
            CXSourceLocation startLocation = clang_getRangeStart(replacementRange);
            CXSourceLocation endLocation = clang_getRangeEnd(replacementRange);
            qDebug() << "FIX-IT:" << toQString(startLocation) << toQString(endLocation) << toQString(text);
        }

//        const CXString diagnosticString = clang_formatDiagnostic(diagnostic,
//                                                                 clang_defaultDiagnosticDisplayOptions());
//        fprintf(stderr, "%s\n", clang_getCString(diagnosticString));
//        clang_disposeString(diagnosticString);
    }
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
    CXTranslationUnit translationUnit;
    const CXErrorCode errorCode
        = clang_parseTranslationUnit2(index, NULL, options.data(), options.size(), 0, 0,
                                      CXTranslationUnit_None, &translationUnit);
    QCOMPARE(errorCode, CXError_Success);

    // Process diagnostics
    printDiagnosticsAndFixIts(translationUnit);

    // Clean up
    clang_disposeTranslationUnit(translationUnit);
    clang_disposeIndex(index);
}

QTEST_APPLESS_MAIN(LibClangTest)

#include "tst_libclangtest.moc"
