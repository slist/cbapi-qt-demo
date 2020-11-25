// Copyright 2020 VMware, Inc.
// SPDX-License-Identifier: MIT

#ifndef REPORT_H
#define REPORT_H

#include <QWidget>
#include <QDateTime>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "iocsyntaxhighlighter.h"
#include "reportobject.h"

namespace Ui {
class Report;
}

class Report : public QWidget
{
    Q_OBJECT

public:
    explicit Report(QWidget *parent = nullptr);
    ~Report();
    void fetch_report();
    void setReportObject(ReportObject *);

private slots:
    void on_pushButton_clicked();
    void on_pushButton_json_view_clicked();

private:
    Ui::Report *ui;
    IocSyntaxHighlighter *hl;
    ReportObject *r;
};

#endif // REPORT_H
