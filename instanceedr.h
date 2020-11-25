// Copyright 2020 VMware, Inc.
// SPDX-License-Identifier: MIT

#ifndef INSTANCEEDR_H
#define INSTANCEEDR_H

#include <QWidget>

namespace Ui {
class InstanceEdr;
}

class InstanceEdr : public QWidget
{
    Q_OBJECT

public:
    explicit InstanceEdr(QWidget *parent = nullptr);
    ~InstanceEdr();

    void set_name(const QString & name);
    void set_api(const QString & api);
    void set_url(const QString & url);

    QString get_name();
    QString get_api();
    QString get_url();

    bool isValid();

private slots:
    void on_lineEdit_name_textChanged(const QString &arg1);
    void on_lineEdit_url_textChanged(const QString &arg1);
    void on_lineEdit_api_textChanged(const QString &arg1);

private:
    Ui::InstanceEdr *ui;
    void check_validity();

};

#endif // INSTANCEEDR_H
