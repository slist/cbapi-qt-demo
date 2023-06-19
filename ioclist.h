// Copyright 2020-2023 VMware, Inc.
// SPDX-License-Identifier: MIT

#ifndef IOCLIST_H
#define IOCLIST_H

#include <QDialog>

namespace Ui {
class IocList;
}

class IocList : public QDialog
{
    Q_OBJECT

public:
    explicit IocList(QWidget *parent = nullptr);
    ~IocList();

    void setBigList(QStringList);

private slots:
    void on_pushButton_find_clicked();
    void on_pushButton_txt_clicked();
    void on_pushButton_odf_clicked();
    void on_pushButton_ok_clicked();

private:
    Ui::IocList *ui;
};

#endif // IOCLIST_H
