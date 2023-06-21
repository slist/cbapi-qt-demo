// Copyright 2020 VMware, Inc.
// SPDX-License-Identifier: MIT

#include "reportobject.h"

#include <QUrl>
#include <QNetworkRequest>

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>

ReportObject::ReportObject(QObject *parent) : QObject(parent),
    manager(new QNetworkAccessManager(this))
{
    reply = NULL;
}

void ReportObject::fetch_report()
{
    // See https://developer.carbonblack.com/reference/carbon-black-cloud/cb-threathunter/latest/watchlist-api/

    QString url = server_url + "/threathunter/watchlistmgr/v3/orgs/" + org_key + "/reports/" + id;

    QNetworkRequest request=QNetworkRequest(QUrl(url));
    request.setRawHeader("X-Auth-Token", auth.toLocal8Bit());

    reply = manager->get(request);

#if QT_VERSION_MAJOR == 5
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotError(QNetworkReply::NetworkError)));
#endif

    connect(reply, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(slotSslErrors(QList<QSslError>)));
    connect(reply, SIGNAL(finished()), this, SLOT(slotFinished()));
}

void ReportObject::slotError(QNetworkReply::NetworkError)
{
    qDebug() << "Network Error";
    qDebug() << "Error CODE : " << reply->error();
    qDebug() << reply->errorString();
}

void ReportObject::slotSslErrors(QList<QSslError>)
{
    qDebug() << "SSL Error";
}

void ReportObject::slotFinished()
{
    // Convert received data from Latin1 to UTF8
    // In Latin1, 'é' character is \xE9

    QByteArray reply_latin1 = reply->readAll();
    reply->deleteLater();
    QString s;
    s = s.fromLatin1(reply_latin1);
    jsonReport = s.toUtf8();

    QJsonParseError *e = new QJsonParseError();

    QJsonDocument json_doc = QJsonDocument::fromJson(jsonReport, e);
    if (json_doc.isNull()) {
        qDebug("Failed parse JSON");
        return;
    }
    if (!json_doc.isObject()) {
        qDebug("JSON is not an object.");
        return;
    }

    QJsonObject obj = json_doc.object();
    title = obj["title"].toString();
    timestamp_last_update.setSecsSinceEpoch(obj["timestamp"].toInt());
    severity = obj["severity"].toInt();
    description = obj["description"].toString();
    ioc = obj["iocs_v2"].toArray()[0].toObject()["values"].toArray()[0].toString();
    link = obj["link"].toString();

    for (int i = 0; i < obj["tags"].toArray().size(); i++)
    {
        tags << obj["tags"].toArray()[i].toString();
    }
}

