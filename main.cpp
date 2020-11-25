// Copyright 2020 VMware, Inc.
// SPDX-License-Identifier: MIT

#include "homewindow.h"
#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setOrganizationName("Carbon Black");
    QApplication::setApplicationName("cb");

    QTranslator qtTranslator;
    if (qtTranslator.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath))) {
        app.installTranslator(&qtTranslator);
    } else {
        qDebug("Failed to load a translation for QT in your local language");
    }

    QTranslator myTranslator;
    if (myTranslator.load("cb_" + QLocale::system().name(), QCoreApplication::applicationDirPath())) {
        app.installTranslator(&myTranslator);
    } else {
        // qDebug() << "Failed to load a translation for the language:" << QLocale::system().name();
    }

    HomeWindow w;
    w.show();

    return app.exec();
}
