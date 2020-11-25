// Copyright 2020 VMware, Inc.
// SPDX-License-Identifier: MIT

#ifndef INSTANCE_H
#define INSTANCE_H

#include <QWidget>
#include "instances.h"

namespace Ui {
class Instance;
}

class Instance : public QWidget
{
    Q_OBJECT

public:
    explicit Instance(QWidget *parent = nullptr);
    ~Instance();
    QString get_name();
    QString get_api_id();
    QString get_api_secret();
    QString get_server();
    QString get_org_key();
    QString get_server_display_name();
    bool isValid();

    void set_name(const QString & s);
    void set_api_id(const QString & s);
    void set_api_secret(const QString & s);
    void set_server(const QString & s);
    void set_org_key(const QString & s);
    void setInstanceType(Instances::InstanceType type);

private slots:
    void on_customer_name_LineEdit_textChanged(const QString &arg1);
    void on_api_id_LineEdit_textChanged(const QString &arg1);
    void on_api_secret_LineEdit_textChanged(const QString &arg1);
    void on_org_key_LineEdit_textChanged(const QString &arg1);

private:
    Ui::Instance *ui;
    void check_validity();
    Instances::InstanceType instanceType;
};

#endif // INSTANCE_H
