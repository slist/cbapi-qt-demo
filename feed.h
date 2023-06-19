// Copyright 2020-2023 VMware, Inc.
// SPDX-License-Identifier: MIT

#ifndef FEED_H
#define FEED_H

#include <QWidget>
#include <QJsonValue>
#include <QDateTime>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "report.h"
#include "reportobject.h"

namespace Ui {
class Feed;
}

class Feed : public QWidget
{
    Q_OBJECT

public:
    explicit Feed(QWidget *parent = nullptr);
    ~Feed();
    void setName(const QString &);
    void setDescription(const QString &);
    void setAlerts(bool b);
    void setLastUpdate(const QDateTime & dateTime);
    void setReports(int num);
    void setPixmap(const QPixmap & pix);
    void fetch_reports();

    QString name;
    QString description;
    QDateTime timestamp_create;
    QDateTime timestamp_last_update;
    QString server_url;
    QString auth, org_key;
    QString feed_id;        // Link to feed
    bool isFeed; // If true, it's a feed, if false is a Watchlist
    QStringList report_ids; // For custom watchlist -> links to report
    QStringList reports;
    QList<ReportObject *> reportList;
    int IOC_count;

private:
    Ui::Feed *ui;
    int reportNumber;
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    bool network_error;
    bool alerts_enabled;
    QByteArray jsonReports;

private slots:
    void slotError(QNetworkReply::NetworkError);
    void slotSslErrors(QList<QSslError>);
    void slotFinished();
    void on_pushButton_json_view_clicked();
    void on_pushButton_download_clicked();
    void on_pushButton_expand_clicked();

signals:
    void downloadFinished(const QString &);
    void new_report(ReportObject *r);
    void expand(Feed * f);
    void contract(Feed * f);
};

#endif // FEED_H
