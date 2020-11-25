// Copyright 2020 VMware, Inc.
// SPDX-License-Identifier: MIT

#include "iocs.h"
#include "ui_iocs.h"

#include "ioc.h"

Iocs::Iocs(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Iocs)
{
    ui->setupUi(this);

    // Left part
    auto *wl = new Ioc(this);
    wl->set_default_server(0);
    auto layoutl = new QVBoxLayout();
    layoutl->addWidget(wl);
    ui->framel->setLayout(layoutl);

    // Right part
    auto *wr = new Ioc(this);
    wr->set_default_server(1);
    auto layoutr = new QVBoxLayout();
    layoutr->addWidget(wr);
    ui->framer->setLayout(layoutr);
}

Iocs::~Iocs()
{
    delete ui;
}
