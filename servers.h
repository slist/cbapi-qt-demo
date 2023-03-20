// Copyright 2023 VMware, Inc.
// SPDX-License-Identifier: MIT

#ifndef SERVERS_H
#define SERVERS_H

#include <QWidget>
#include <QVBoxLayout>

namespace Ui {
class Servers;
}

class Servers : public QWidget
{
    Q_OBJECT

public:
    explicit Servers(QWidget *parent = nullptr);
    ~Servers();

private slots:
    void on_pushButton_cancel_clicked();

private:
    Ui::Servers *ui;
    QVBoxLayout * layout1;
    QVBoxLayout * layout2;
    QVBoxLayout * layout3;
    QVBoxLayout * layout4;
    void load();
};

#endif // SERVERS_H
