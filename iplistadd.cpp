// Copyright 2020 VMware, Inc.
// SPDX-License-Identifier: MIT

#include "iplistadd.h"
#include "ui_iplistadd.h"

ipListAdd::ipListAdd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ipListAdd)
{
    ui->setupUi(this);
}

ipListAdd::~ipListAdd()
{
    delete ui;
}

QString ipListAdd::getServer()
{
    return ui->lineEdit->text();
}
