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

    enum InstanceType { NgavInstanceType, EedrInstanceType, EdrInstanceType };
    Q_ENUM(InstanceType)
    void setInstanceType(InstanceType type);

public slots:
    void on_save();

private slots:
    void on_pushButton_add_clicked();

private:
    Ui::Instances *ui;
    InstanceType instanceType;
};

#endif // INSTANCES_H
