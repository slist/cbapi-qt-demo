// Copyright 2020 VMware, Inc.
// SPDX-License-Identifier: MIT

#include "servers.h"
#include "ui_servers.h"

#include "homewindow.h"
#include "instances.h"

Servers::Servers(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Servers),
    layout1(new QVBoxLayout),
    layout2(new QVBoxLayout),
    layout3(new QVBoxLayout),
    layout4(new QVBoxLayout)
{
    ui->setupUi(this);

    ui->frame_ngav->setLayout(layout1);
    ui->frame_eedr->setLayout(layout2);
    ui->frame_edr->setLayout(layout3);
    ui->frame_appcontrol->setLayout(layout4);

    load();
}

Servers::~Servers()
{
    delete ui;
}

void Servers::on_pushButton_cancel_clicked()
{
    // Clear layout, and delete widgets
    clearLayout(layout1, true);
    clearLayout(layout2, true);
    clearLayout(layout3, true);
    clearLayout(layout4, true);
    load();
}

void Servers::load()
{
    auto *w1 = new Instances(this);
    layout1->addWidget(w1);

    auto *w2 = new Instances(this);
    layout2->addWidget(w2);

    auto *w3 = new Instances(this);
    layout3->addWidget(w3);

    auto *w4 = new Instances(this);
    layout4->addWidget(w4);

    w1->setInstanceType(Instances::NgavInstanceType);
    w2->setInstanceType(Instances::EedrInstanceType);
    w3->setInstanceType(Instances::EdrInstanceType);
    w4->setInstanceType(Instances::AppcontrolInstanceType);

    connect(ui->pushButton_save, SIGNAL(clicked()), w1, SLOT(on_save()));
    connect(ui->pushButton_save, SIGNAL(clicked()), w2, SLOT(on_save()));
    connect(ui->pushButton_save, SIGNAL(clicked()), w3, SLOT(on_save()));
    connect(ui->pushButton_save, SIGNAL(clicked()), w4, SLOT(on_save()));

}
