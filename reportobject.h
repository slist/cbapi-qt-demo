// Copyright 2020 VMware, Inc.
// SPDX-License-Identifier: MIT

#ifndef REPORTOBJECT_H
#define REPORTOBJECT_H

#include <QObject>
#include <QDateTime>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class ReportObject : public QObject
{
    Q_OBJECT
public:
    explicit ReportObject(QObject *parent = nullptr);

    void fetch_report();

    QString id;
    QString title;
    QDateTime timestamp_last_update;
    QString description;
    quint8 severity;
    QString link;
    QStringList tags;
    QString ioc;
    QString visibility;
    QString server_url;
    QString auth, org_key;
    QByteArray jsonReport;

private slots:
    void slotError(QNetworkReply::NetworkError);
    void slotSslErrors(QList<QSslError>);
    void slotFinished();

private:
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    //bool network_error;
};

#endif // REPORTOBJECT_H
