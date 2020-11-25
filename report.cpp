// Copyright 2020 VMware, Inc.
// SPDX-License-Identifier: MIT

#include "report.h"
#include "ui_report.h"

#include <QDesktopServices>

#include <QTreeView>
#include "qjsonmodel.h"

Report::Report(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Report),
    r(nullptr)
{
    ui->setupUi(this);
}

Report::~Report()
{
    delete ui;
}

void Report::on_pushButton_clicked()
{
    if (r == nullptr) {
        qDebug("r not initialized in link button");
        return;
    }
    QString lnk;
    if (r->link.isEmpty()) {
        lnk = "http://www.carbonblack.com";
    } else {
        lnk = r->link;
    }
    QDesktopServices::openUrl(QUrl(lnk));
}


void Report::on_pushButton_json_view_clicked()
{
    if (r == nullptr) {
        qDebug("r not initialized in json button");
        return;
    }

    QTreeView * view   = new QTreeView;
    QJsonModel * model = new QJsonModel;

    view->setModel(model);

    model->loadJson(r->jsonReport);
    view->resize(1024,768);
    view->setColumnWidth(0,400);
    view->expandAll();
    view->setWindowTitle("Watchlist Report for report : " + r->title);
    view->setWindowIcon(QIcon(":/img/cb.jpeg"));
    view->show();
}

void Report::setReportObject(ReportObject *ro)
{
    r = ro;
    ui->label_name->setText(r->title);

    QString slabel;
    slabel = tr("Last updated:") + "\n" + r->timestamp_last_update.toString("ddd MMMM d yyyy");
    QDateTime current;
    current = QDateTime::currentDateTime();
    ui->label_last_updated->setText(slabel);
    if (r->timestamp_last_update.daysTo(current) < 60) {
        ui->label_new->setPixmap(QPixmap(":/img/new.png").scaledToHeight(40));
    } else {
        ui->label_new->setPixmap(QPixmap(""));
    }

    ui->label_severity->setText(QString(tr("Severity\n%0").arg(r->severity)));

    ui->label_description->setText(r->description);
    if (r->description.isEmpty()) {
        ui->label_description->setVisible(false);
    }

    hl = new IocSyntaxHighlighter(ui->textEdit_IOC->document());

    if (hl == nullptr) {
        qDebug("Enable to create Syntax highlighter");
    }
    ui->textEdit_IOC->setText(r->ioc);

    ui->label_tags->setVisible(false); // TODO : add tags...

    if (r->link.isEmpty()) {
        ui->pushButton->setVisible(false);
    }
}
