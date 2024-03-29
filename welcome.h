// Copyright 2020-2023 VMware, Inc.
// SPDX-License-Identifier: MIT

#ifndef WELCOME_H_
#define WELCOME_H_

#include <QWidget>

namespace Ui {
class Welcome;
}

class Welcome : public QWidget {
    Q_OBJECT

 public:
    explicit Welcome(QWidget *parent = nullptr);
    ~Welcome();

 private slots:
    void on_pushButton_aboutQt_clicked();
    void on_checkBox_DarkMode_stateChanged(int);

 private:
    Ui::Welcome *ui;
};

#endif  // WELCOME_H_
