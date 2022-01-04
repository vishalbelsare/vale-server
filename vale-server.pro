VERSION = 2.0.0

TARGET = "Vale Server"

HEADERS       = src/MainWindow.h \
                src/ManagedProcess.h \
                src/StyledLabel.h \
                src/DisplayDialog.h \
                src/HTTPServer.h \
                src/INIHighlighter.h \
                src/TextLogger.h \
                src/ValeManager.h \
                src/AboutDialog.h

SOURCES       = src/main.cpp \
                src/DisplayDialog.cpp \
                src/ManagedProcess.cpp \
                src/StyledLabel.cpp \
                src/INIHighlighter.cpp \
                src/HTTPServer.cpp \
                src/ValeManager.cpp \
                src/TextLogger.cpp \
                src/MainWindow.cpp \
                src/AboutDialog.cpp

RESOURCES     = static/main.qrc

include(src/3rdparty/QtAwesome/QtAwesome.pri)
include(src/3rdparty/SingleApplication/singleapplication.pri)
include(src/3rdparty/api/httplib.pri)

DEFINES += APP_VERSION='\\"$${VERSION}\\"'
DEFINES += QAPPLICATION_CLASS=QApplication

FORMS += \
    src/aboutdialog.ui

QT += widgets
QT -= opengl svg qml dbus

CONFIG += c++11 release
CONFIG -= qtquickcompiler

# The Vale and Dashboard files.
#
# See https://bit.ly/2QXNlgY
RESDIR = $$shell_quote($$clean_path($$PWD/build))

unix:!macx {
DESDIR = $$shell_quote($$clean_path($$OUT_PWD))
copydata.commands = $(COPY_DIR) $$RESDIR $$DESDIR
first.depends = $(first) copydata

export(first.depends)
export(copydata.commands)

QMAKE_EXTRA_TARGETS += first copydata
}

win32 {
# NOTE: Windows can't compile with " " in name?
TARGET = "Vale-Server"

RC_FILE = static/app.rc

# WinSparkle (added to C:\Qt\5.12.2\mingw73_64\bin)
INCLUDEPATH += $$_PRO_FILE_PWD_/../WinSparkle-0.7.0/include

LIBS += -L$$_PRO_FILE_PWD_/../WinSparkle-0.7.0/x64/Release -lWinSparkle

# MinGW
LIBS += -lws2_32
# Visual Studio
# LIBS += ws2_32.lib

DESDIR = $$shell_quote($$clean_path($$OUT_PWD/release))
RESDIR = $$system_path($$RESDIR)
DESDIR = $$system_path($$DESDIR)

copydata.commands = $(COPY_DIR) $$RESDIR $$DESDIR
first.depends = $(first) copydata

export(first.depends)
export(copydata.commands)

QMAKE_EXTRA_TARGETS += first copydata
}

macx {
ICON = static/app.icns

QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.15
QMAKE_INFO_PLIST = pkg/osx/Info.plist

LIBS += -framework ApplicationServices
LIBS += -framework Cocoa
LIBS += -framework Sparkle

HEADERS += src/darwin/mac.h
HEADERS += src/darwin/AutoUpdater.h
HEADERS += src/darwin/CocoaInitializer.h
HEADERS += src/darwin/SparkleAutoUpdater.h

SOURCES += src/darwin/mac.mm
SOURCES += src/darwin/AutoUpdater.cpp
SOURCES += src/darwin/CocoaInitializer.mm
SOURCES += src/darwin/SparkleAutoUpdater.mm

APP_QML_FILES.files = build/dashboard
APP_QML_FILES.path = Contents/Resources

APP_BIN_FILES.files = lib/bin/valelib
APP_BIN_FILES.path = Contents/MacOS

QMAKE_BUNDLE_DATA += APP_QML_FILES APP_BIN_FILES
}
