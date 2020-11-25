// Copyright 2020 VMware, Inc.
// SPDX-License-Identifier: MIT

#include "policy.h"
#include "ui_policy.h"

Policy::Policy(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Policy)
{
    ui->setupUi(this);
}

Policy::~Policy()
{
    delete ui;
}

void Policy::setName(const QString n)
{
    name = n;
    ui->label_name->setText(n);
}

void Policy::setDescription(const QString d)
{
    description = d;
    ui->label_name->setToolTip(d);
}

void Policy::setUi(bool b)
{
    if (b) {
        ui->label_ui->setStyleSheet("border: 2px solid gray;border-radius: 5px;background-color: #2AA62A;");
        ui->label_ui->setText("UI\nON");
    } else {
        ui->label_ui->setStyleSheet("border: 2px solid gray;border-radius: 5px;background-color: #FD7979;");
        ui->label_ui->setText("UI\nOFF");
    }
}

void Policy::setGoLive(bool b)
{
    ui->label_go_live->setAlignment(Qt::AlignCenter);
    ui->label_go_live->setMargin(2);

    if (b) {
        ui->label_go_live->setStyleSheet("border: 2px solid gray;border-radius: 5px;background-color: #2AA62A;");
        ui->label_go_live->setText(" Go Live \nON");
    } else {
        ui->label_go_live->setStyleSheet("border: 2px solid gray;border-radius: 5px;background-color: #FD7979;");
        ui->label_go_live->setText(" Go Live \nOFF");
    }
}

void Policy::setRulesNumber(int n)
{
    if (n > 0) {
        ui->label_rules->setStyleSheet("border: 2px solid gray;border-radius: 5px;background-color: #2AA62A;");
    } else {
        ui->label_rules->setStyleSheet("border: 2px solid gray;border-radius: 5px;background-color: #FD7979;");
    }
    QString s = QString("Rules:\n%1").arg(n);
    ui->label_rules->setText(s);

}

void Policy::setLog(bool b)
{
    if (b) {
        ui->label_log->setStyleSheet("border: 2px solid gray;border-radius: 5px;background-color: #2AA62A;");
        ui->label_log->setText("LOG\nON");
    } else {
        ui->label_log->setStyleSheet("border: 2px solid gray;border-radius: 5px;background-color: #FD7979;");
        ui->label_log->setText("LOG\nOFF");
    }
}

void Policy::on_checkBox_stateChanged(int arg1)
{
    switch (arg1) {
    case Qt::Unchecked:
        emit pol_deselected(name);
        break;
    case Qt::Checked:
        emit pol_selected(name);
        break;
    }
}

void Policy::unselect()
{
    ui->checkBox->setChecked(false);
}
