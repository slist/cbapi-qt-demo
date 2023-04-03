// Copyright 2020 VMware, Inc.
// SPDX-License-Identifier: MIT

#include "instanceappcontrol.h"
#include "ui_instanceappcontrol.h"

InstanceAppcontrol::InstanceAppcontrol(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InstanceAppcontrol)
{
    ui->setupUi(this);
}

InstanceAppcontrol::~InstanceAppcontrol()
{
    delete ui;
}

void InstanceAppcontrol::set_name(const QString & name)
{
    ui->lineEdit_name->setText(name);
}

void InstanceAppcontrol::set_api(const QString & api)
{
    ui->lineEdit_api->setText(api);
}

void InstanceAppcontrol::set_url(const QString & url)
{
    ui->lineEdit_url->setText(url);
    check_validity();
}

QString  InstanceAppcontrol::get_name()
{
    return ui->lineEdit_name->text();
}

QString InstanceAppcontrol::get_api()
{
    return ui->lineEdit_api->text();
}

QString InstanceAppcontrol::get_url()
{
    return ui->lineEdit_url->text();
}

bool InstanceAppcontrol::isValid()
{
    if (get_name().isEmpty() || get_api().isEmpty() || get_url().isEmpty())
        return false;
    return true;
}


void InstanceAppcontrol::check_validity()
{
    if (!isValid()) {
        ui->label_invalid->show();
    } else {
        ui->label_invalid->hide();
    }
}

void InstanceAppcontrol::on_lineEdit_name_textChanged(const QString & /* arg1 */)
{
    check_validity();
}

void InstanceAppcontrol::on_lineEdit_url_textChanged(const QString & /* arg1 */)
{
    check_validity();
}

void InstanceAppcontrol::on_lineEdit_api_textChanged(const QString & /* arg1 */)
{
    check_validity();
}
