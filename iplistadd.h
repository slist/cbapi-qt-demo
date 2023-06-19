// Copyright 2020-2023 VMware, Inc.
// SPDX-License-Identifier: MIT

#ifndef IPLISTADD_H
#define IPLISTADD_H

#include <QDialog>

namespace Ui {
class ipListAdd;
}

class ipListAdd : public QDialog
{
    Q_OBJECT

public:
    explicit ipListAdd(QWidget *parent = nullptr);
    ~ipListAdd();
    QString getServer();


private:
    Ui::ipListAdd *ui;
};

#endif // IPLISTADD_H
