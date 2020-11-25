// Copyright 2020 VMware, Inc.
// SPDX-License-Identifier: MIT

#ifndef WELCOME_H
#define WELCOME_H

#include <QWidget>

namespace Ui {
class Welcome;
}

class Welcome : public QWidget
{
    Q_OBJECT

public:
    explicit Welcome(QWidget *parent = nullptr);
    ~Welcome();

private slots:
    void on_pushButton_aboutQt_clicked();

private:
    Ui::Welcome *ui;
};

#endif // WELCOME_H
