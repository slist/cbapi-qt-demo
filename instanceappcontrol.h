// Copyright 2020 VMware, Inc.
// SPDX-License-Identifier: MIT

#ifndef INSTANCEAppcontrol_H
#define INSTANCEAppcontrol_H

#include <QWidget>

namespace Ui {
class InstanceAppcontrol;
}

class InstanceAppcontrol : public QWidget
{
    Q_OBJECT

public:
    explicit InstanceAppcontrol(QWidget *parent = nullptr);
    ~InstanceAppcontrol();

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
    Ui::InstanceAppcontrol *ui;
    void check_validity();

};

#endif // INSTANCEAppcontrol_H
