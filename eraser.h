// Copyright 2020 VMware, Inc.
// SPDX-License-Identifier: MIT

#ifndef ERASER_H
#define ERASER_H

#include <QDialog>
#include <qtablewidget.h>

namespace Ui {
class Eraser;
}

class Eraser : public QDialog
{
    Q_OBJECT

public:
    explicit Eraser(QWidget *parent = 0);
    ~Eraser();
    void setTable(QTableWidget *);

private slots:
    void on_pushButton_delete_clicked();

private:
    Ui::Eraser *ui;
    QTableWidget *table;

    int counter_to_delete;
    int counter_deleted;

    void update_progressbar();
    void delete_sensor(int id);
};

#endif // ERASER_H
