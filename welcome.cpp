// Copyright 2020 VMware, Inc.
// SPDX-License-Identifier: MIT

#include "welcome.h"
#include "ui_welcome.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QSettings>

#include "version.h"

Welcome::Welcome(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Welcome)
{
    ui->setupUi(this);
    ui->label_version->setText(tr("By Stéphane List <a href=\"mailto:slist@vmware.com\">&lt;slist@vmware.com&gt;</a>"
                                  "<p>Version cb: %1"
                                  "<p>Version %2: %3.%4"
                                  "<p>OS version: %5"
                                  "<p>License: MIT"

                                  "</p>")
                               .arg(VERSION)
                           #ifdef __GNUC__
                               .arg("GCC")
                               .arg(__GNUC__)
                               .arg(__GNUC_MINOR__)
                           #elif defined(_MSC_VER)
                               .arg("VC++")
                               .arg(_MSC_VER / 100)
                               .arg(_MSC_VER % 100,2,10,QChar('0'))
                           #else
                               .arg("Compiler")
                               .arg("?")
                               .arg("?")
                           #endif
                               .arg(QSysInfo::prettyProductName())
                               );
    QSettings settings;
    ui->checkBox_DarkMode->setChecked(settings.value("DarkMode").toBool());
}

Welcome::~Welcome()
{
    delete ui;
}

void Welcome::on_pushButton_aboutQt_clicked()
{
    QMessageBox::aboutQt(this, "cb");
}

void Welcome::on_checkBox_DarkMode_stateChanged(int /* arg1*/)
{
    QSettings settings;

    if (ui->checkBox_DarkMode->checkState() == Qt::Checked) {
        qDebug("dark mode");
        settings.setValue("DarkMode", true);

        QFile f(":qdarkstyle/dark/darkstyle.qss");

        if (!f.exists())   {
            printf("Unable to set stylesheet, file not found\n");
        }
        else   {
            f.open(QFile::ReadOnly | QFile::Text);
            QTextStream ts(&f);
            qApp->setStyleSheet(ts.readAll());
        }
    } else {
        qDebug("light mode");
        settings.setValue("DarkMode", false);

        QFile f(":qdarkstyle/light/lightstyle.qss");

        if (!f.exists())   {
            printf("Unable to set stylesheet, file not found\n");
        }
        else   {
            f.open(QFile::ReadOnly | QFile::Text);
            QTextStream ts(&f);
            qApp->setStyleSheet(ts.readAll());
        }
    }
}

