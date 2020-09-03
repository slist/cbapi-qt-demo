// Copyright 2020 VMware, Inc.
// SPDX-License-Identifier: MIT

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionAbout_QT_triggered();
    void on_actionAbout_cb_triggered();
    void on_action_Quit_triggered();
    void on_pushButton_connect_clicked();
    void on_pushButton_compare_clicked();
    void on_checkBox_stateChanged(int arg1);
    void on_action_Diff_tool_triggered();
    void on_actionCarbon_Black_Cloud_Instances_triggered();
    void on_pushButton_unselect_clicked();
    void on_pushButton_copy_clicked();

private:
    Ui::MainWindow *ui;
    QStringList inst_list;
    QStringList get_instances_list();
    void updateButtons();
    void start();
    void clear();
    QList<QWidget *> Widgets;

public slots:
    void add_pol(const QString &, const QString &);
    void del_pol(const QString &, const QString &);
    void log(const QString &);

signals:
    void unselect_all();
    void refresh();
};

#endif // MAINWINDOW_H
