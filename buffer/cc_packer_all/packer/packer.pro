#-------------------------------------------------
#
# Project created by QtCreator 2015-05-30T10:46:33
#
#-------------------------------------------------

QT       += core gui concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets



TARGET = packer
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp #\
    #C:\job\tarlib.src\tarlib\*.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

linux {
LIBS += -lcrypto -lssl
}


win32 {
# Win
INCLUDEPATH += "C:\job\openssl-1.0.2a\dist\include"
#"C:\job\tarlib.src"
LIBS += -L"C:\job\openssl-1.0.2a\dist\lib" -lcrypto -lssl -lgdi32
#-lshell32 -lshlwapi

release {
    # Tar - stopped - user mingw tar
    QMAKE_POST_LINK += $$PWD\\action.bat $$OUT_PWD/release/packer.exe c:\\packer_release.exe
}
}
