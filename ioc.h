// Copyright 2020 VMware, Inc.
// SPDX-License-Identifier: MIT

#ifndef IOC_H
#define IOC_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "iocsyntaxhighlighter.h"
#include "feed.h"
#include "report.h"

namespace Ui {
class Ioc;
}

class Ioc : public QWidget
{
    Q_OBJECT

public:
    explicit Ioc(QWidget *parent = nullptr);
    ~Ioc();
    void set_default_server(int index);

private:
    Ui::Ioc *ui;
    QStringList get_instances_list(const QString &);
    void refresh();
    void display_watchlists_feeds();

    QString name;
    QString api_id;
    QString api_secret;
    QString server, server_url;
    QString org_key;
    QString auth;
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    bool network_error;
    QList<Feed *> Feedlist;
    int default_server = -1;
    QStringList TheBigList_IOC;
    QStringList TheBigList_IP;
    QByteArray jsonReports;
    int watchlist_count;
    int feed_count;

private slots:
    void slotError(QNetworkReply::NetworkError);
    void slotSslErrors(QList<QSslError>);
    void slotFinished();
    void on_comboBox_server_currentIndexChanged(int index);
    void on_pushButton_list_ioc_clicked();
    void on_pushButton_list_ip_clicked();
    void on_pushButton_json_view_clicked();

public slots:
    void watchlistDownloadFinished(const QString &);
    void expand_feed(Feed * f);
    void contract_feed(Feed * f);
};

#endif // IOC_H
