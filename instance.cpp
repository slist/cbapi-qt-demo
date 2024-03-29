// Copyright 2020 VMware, Inc.
// SPDX-License-Identifier: MIT

#include "instance.h"
#include "ui_instance.h"

Instance::Instance(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Instance)
{
    ui->setupUi(this);
}

Instance::~Instance()
{
    delete ui;
}

QString  Instance::get_name()
{
    return ui->customer_name_LineEdit->text();
}

QString Instance::get_api_id()
{
    return ui->api_id_LineEdit->text();
}

QString Instance::get_api_secret()
{
    return ui->api_secret_LineEdit->text();
}

QString Instance::get_org_key()
{
    return ui->org_key_LineEdit->text();
}

QString Instance::get_server_display_name()
{
    return ui->env_comboBox->currentText();
}

void Instance::set_name(const QString & s)
{
    ui->customer_name_LineEdit->setText(s);
}
void Instance::set_api_id(const QString & s)
{
    ui->api_id_LineEdit->setText(s);
}
void Instance::set_api_secret(const QString & s)
{
    ui->api_secret_LineEdit->setText(s);
}

void Instance::set_server(const QString & s)
{
    ui->env_comboBox->setCurrentText(s);
}

void Instance::set_org_key(const QString & s)
{
    ui->org_key_LineEdit->setText(s);
}

bool Instance::isValid()
{
    if ((get_name().isEmpty() || get_api_id().isEmpty() || get_api_secret().isEmpty() || get_org_key().isEmpty()))
        return false;
    return true;
}

void Instance::on_customer_name_LineEdit_textChanged(const QString & arg1)
{
    Q_UNUSED(arg1);
    check_validity();
}

void Instance::on_api_id_LineEdit_textChanged(const QString & arg1)
{
    Q_UNUSED(arg1);
    check_validity();
}

void Instance::on_api_secret_LineEdit_textChanged(const QString & arg1)
{
    Q_UNUSED(arg1);
    check_validity();
}

void Instance::on_org_key_LineEdit_textChanged(const QString & arg1)
{
    Q_UNUSED(arg1);
    check_validity();
}

void Instance::check_validity()
{
    if (!isValid()) {
        ui->label_invalid->show();
    } else {
        ui->label_invalid->hide();
    }
}

void Instance::setInstanceType(Instances::InstanceType type)
{
    instanceType = type;
    check_validity();
}
