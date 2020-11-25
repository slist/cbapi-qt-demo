// Copyright 2020 VMware, Inc.
// SPDX-License-Identifier: MIT

#include "instanceedr.h"
#include "ui_instanceedr.h"

InstanceEdr::InstanceEdr(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InstanceEdr)
{
    ui->setupUi(this);
}

InstanceEdr::~InstanceEdr()
{
    delete ui;
}

void InstanceEdr::set_name(const QString & name)
{
    ui->lineEdit_name->setText(name);
}

void InstanceEdr::set_api(const QString & api)
{
    ui->lineEdit_api->setText(api);
}

void InstanceEdr::set_url(const QString & url)
{
    ui->lineEdit_url->setText(url);
    check_validity();
}

QString  InstanceEdr::get_name()
{
    return ui->lineEdit_name->text();
}

QString InstanceEdr::get_api()
{
    return ui->lineEdit_api->text();
}

QString InstanceEdr::get_url()
{
    return ui->lineEdit_url->text();
}

bool InstanceEdr::isValid()
{
    if (get_name().isEmpty() || get_api().isEmpty() || get_url().isEmpty())
        return false;
    return true;
}


void InstanceEdr::check_validity()
{
    if (!isValid()) {
        ui->label_invalid->show();
    } else {
        ui->label_invalid->hide();
    }
}

void InstanceEdr::on_lineEdit_name_textChanged(const QString & /* arg1 */)
{
    check_validity();
}

void InstanceEdr::on_lineEdit_url_textChanged(const QString & /* arg1 */)
{
    check_validity();
}

void InstanceEdr::on_lineEdit_api_textChanged(const QString & /* arg1 */)
{
    check_validity();
}
