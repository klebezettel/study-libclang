include(detect_libclang.pri)

message("Using libclang from $$LLVM_INSTALL_DIR (change via LLVM_INSTALL_DIR):")
message("  INCLUDEPATH += $$LLVM_INCLUDEPATH")
message("  LIBS += $$LLVM_LIBS")

LIBS += $$LLVM_LIBS
INCLUDEPATH += $$LLVM_INCLUDEPATH

QT       += testlib
QT       -= gui

TARGET = tst_libclangtest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += tst_libclangtest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"
