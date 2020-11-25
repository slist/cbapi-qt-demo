// Copyright 2020 VMware, Inc.
// SPDX-License-Identifier: MIT

#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include <QMainWindow>

void clearLayout(QLayout* layout, bool deleteWidgets);

namespace Ui {
class HomeWindow;
}

class HomeWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit HomeWindow(QWidget *parent = 0);
    ~HomeWindow();

private slots:
    void on_pushButton_Cloud_clicked();
    void on_pushButton_Edr_clicked();
    void on_pushButton_IOC_clicked();
    void on_pushButton_Collapse_clicked();
    void on_pushButton_Welcome_clicked();
    void on_pushButton_Servers_clicked();
    void on_pushButton_iplist_clicked();

    void on_pushButton_Banned_clicked();

private:
    void clear_frame();
    void reset_pushbuttons();

    Ui::HomeWindow *ui;
    bool collapsed;
    int menu_width;
};

#endif // HOMEWINDOW_H
