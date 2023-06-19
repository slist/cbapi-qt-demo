// Copyright 2020-2023 VMware, Inc.
// SPDX-License-Identifier: MIT

#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QDebug>

#include "homewindow.h"
#include "version.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setOrganizationName("VMware Carbon Black");
    QApplication::setApplicationName("cbapi-qt-demo");
    QApplication::setWindowIcon(QIcon(":/img/qt-cbapi.png"));
    QApplication::setApplicationVersion(VERSION);

#if QT_VERSION_MAJOR == 5
    QTranslator qtTranslator;
    if (qtTranslator.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath))) {
        app.installTranslator(&qtTranslator);
    } else {
        qDebug("Failed to load a translation for QT in your local language");
    }
#else
    QTranslator qtTranslator;
    if (qtTranslator.load("qt_" + QLocale::system().name(), QLibraryInfo::path(QLibraryInfo::TranslationsPath))) {
        app.installTranslator(&qtTranslator);
    } else {
        qDebug("Failed to load a translation for QT in your local language");
    }
#endif
    QTranslator myTranslator;
    if (myTranslator.load("cbapi-qt-demo_" + QLocale::system().name(), QCoreApplication::applicationDirPath())) {
        app.installTranslator(&myTranslator);
    } else {
        qDebug() << "Failed to load a translation for the language:" << QLocale::system().name();
    }

    HomeWindow w;
    w.show();

    return app.exec();
}
