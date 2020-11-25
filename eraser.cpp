// Copyright 2020 VMware, Inc.
// SPDX-License-Identifier: MIT

#include "eraser.h"
#include "ui_eraser.h"
#include <qdebug.h>
#include <QTableWidget>

Eraser::Eraser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Eraser)
{
    ui->setupUi(this);
}

Eraser::~Eraser()
{
    delete ui;
}

void Eraser::setTable(QTableWidget *t)
{
    table = t;

    counter_to_delete = 0;
    counter_deleted = 0;

    int nb = table->rowCount();

    for (int i=0; i<nb; i++)
    {
        if (table->item(i,5)->text() == "X") {
            counter_to_delete++;
        }
    }
    ui->lcdNumber_to_delete->display(counter_to_delete);
    update_progressbar();
}

void Eraser::update_progressbar()
{
    if (counter_to_delete == 0) {
        ui->progressBar->setValue(100);
        ui->pushButton_delete->setEnabled(false);
    } else {
        ui->progressBar->setValue(counter_deleted/counter_to_delete);
    }
}

void Eraser::delete_sensor(int id)
{
    qDebug() << "Delete sensor id = " << id;
    counter_deleted--;
}

void Eraser::on_pushButton_delete_clicked()
{
    counter_to_delete = 0;
    counter_deleted = 0;

    int nb = table->rowCount();

    for (int i=0; i<nb; i++)
    {
        if (table->item(i,5)->text() == "X") {
            delete_sensor(table->item(i,0)->text().toInt());
        }
    }
    ui->lcdNumber_to_delete->display(counter_to_delete);
    update_progressbar();
}
