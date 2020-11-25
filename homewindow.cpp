// Copyright 2020 VMware, Inc.
// SPDX-License-Identifier: MIT

#include "homewindow.h"
#include "ui_homewindow.h"

#include "ngav.h"
#include "edr.h"
#include "iocmainwindow.h"
#include "welcome.h"
#include "iocs.h"
#include "servers.h"
#include "iplist.h"
#include "banned.h"

HomeWindow::HomeWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::HomeWindow)
{
    ui->setupUi(this);
    collapsed = false;
    on_pushButton_Welcome_clicked();
}

HomeWindow::~HomeWindow()
{
    delete ui;
}

void HomeWindow::on_pushButton_Cloud_clicked()
{
    reset_pushbuttons();
    ui->pushButton_Cloud->setStyleSheet("text-align:left; border: 1px solid red;");
    clear_frame();

    auto *w = new Ngav(this);
    auto layout = new QVBoxLayout();
    layout->addWidget(w);

    ui->frame->setLayout(layout);
}

void HomeWindow::on_pushButton_Edr_clicked()
{
    reset_pushbuttons();
    ui->pushButton_Edr->setStyleSheet("text-align:left; border: 1px solid red;");
    clear_frame();

    auto *w = new Edr(this);
    auto layout = new QVBoxLayout();
    layout->addWidget(w);

    ui->frame->setLayout(layout);
}

void HomeWindow::on_pushButton_Collapse_clicked()
{
    if (!collapsed) {
        menu_width = ui->frame_menu->width();
        ui->frame_menu->setMaximumWidth(86); // Icons width

        ui->pushButton_Collapse->setText(">>");
        collapsed = true;
    } else {
        ui->frame_menu->setMaximumWidth(menu_width);
        ui->pushButton_Collapse->setText("<< Collapse");
        collapsed = false;
    }
}

void clearLayout(QLayout* layout, bool deleteWidgets = true)
{
    if (layout == nullptr)
        return;

    while (QLayoutItem* item = layout->takeAt(0))
    {
        QWidget* widget;
        if (  (deleteWidgets)
              && (widget = item->widget())  ) {
            delete widget;
        }
        if (QLayout* childLayout = item->layout()) {
            clearLayout(childLayout, deleteWidgets);
        }
        delete item;
    }
}

void HomeWindow::clear_frame()
{
    QLayout* layout = ui->frame->layout ();
    if (layout != nullptr) {
        clearLayout(layout);
        delete layout;
    }
}

void HomeWindow::on_pushButton_Welcome_clicked()
{
    reset_pushbuttons();
    ui->pushButton_Welcome->setStyleSheet("text-align:left; border: 1px solid red;");
    clear_frame();

    QWidget *w = new Welcome(this);

    auto layout = new QVBoxLayout();
    layout->addWidget(w);

    ui->frame->setLayout(layout);
}


void HomeWindow::on_pushButton_IOC_clicked()
{
    reset_pushbuttons();
    ui->pushButton_IOC->setStyleSheet("text-align:left; border: 1px solid red;");
    clear_frame();

    auto *w = new Iocs(this);
    auto layout = new QVBoxLayout();
    layout->addWidget(w);

    ui->frame->setLayout(layout);
}

void HomeWindow::reset_pushbuttons()
{
    ui->pushButton_IOC->setStyleSheet("text-align:left;");
    ui->pushButton_Cloud->setStyleSheet("text-align:left;");
    ui->pushButton_Edr->setStyleSheet("text-align:left;");
    ui->pushButton_Servers->setStyleSheet("text-align:left;");
    ui->pushButton_Welcome->setStyleSheet("text-align:left;");
    ui->pushButton_iplist->setStyleSheet("text-align:left;");
    ui->pushButton_Banned->setStyleSheet("text-align:left;");
}

void HomeWindow::on_pushButton_Servers_clicked()
{
    reset_pushbuttons();
    ui->pushButton_Servers->setStyleSheet("text-align:left; border: 1px solid red;");
    clear_frame();

    auto *w = new Servers(this);
    auto layout = new QVBoxLayout();
    layout->addWidget(w);

    ui->frame->setLayout(layout);
}

void HomeWindow::on_pushButton_iplist_clicked()
{
    reset_pushbuttons();
    ui->pushButton_iplist->setStyleSheet("text-align:left; border: 1px solid red;");
    clear_frame();

    auto *w = new IpList(this);
    auto layout = new QVBoxLayout();
    layout->addWidget(w);

    ui->frame->setLayout(layout);
}

void HomeWindow::on_pushButton_Banned_clicked()
{
    reset_pushbuttons();
    ui->pushButton_Banned->setStyleSheet("text-align:left; border: 1px solid red;");
    clear_frame();

    auto *w = new Banned(this);
    auto layout = new QVBoxLayout();
    layout->addWidget(w);

    ui->frame->setLayout(layout);
}
