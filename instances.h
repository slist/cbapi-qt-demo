// Copyright 2020 VMware, Inc.
// SPDX-License-Identifier: MIT

#ifndef INSTANCES_H
#define INSTANCES_H

#include <QDialog>

namespace Ui {
class Instances;
}

class Instances : public QDialog
{
    Q_OBJECT

public:
    explicit Instances(QWidget *parent = nullptr);
    ~Instances();

private slots:
    void on_pushButton_add_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::Instances *ui;
};

#endif // INSTANCES_H
