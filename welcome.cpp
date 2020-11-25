// Copyright 2020 VMware, Inc.
// SPDX-License-Identifier: MIT

#include "welcome.h"
#include "ui_welcome.h"
#include <QMessageBox>

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
}

Welcome::~Welcome()
{
    delete ui;
}

void Welcome::on_pushButton_aboutQt_clicked()
{
    QMessageBox::aboutQt(this, "cb");
}
